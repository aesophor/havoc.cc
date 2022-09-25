// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#include "Aimbot.h"

#include <chrono>

#include "Core/Hooks.h"
#include "Core/Interfaces.h"
#include "Hacks/AutoWall.h"
#include "SDK/CEntity.h"
#include "SDK/HitGroup.h"

namespace hacks::aimbot {

namespace {

uint64_t lastFiredTimestamp = 0;  // in milliseconds

void GetBestBone(CBasePlayer *player, float &bestDamage, Bone &bestBone) {
  for (int i = 0; i < static_cast<int>(Bone::SIZE); i++) {
    Bone bone = static_cast<Bone>(i);
    CVector bonePos = player->GetBonePosition(bone);

    hacks::autowall::FireBulletData data;
    float damage = hacks::autowall::GetDamage(bonePos, true, data);

    if (damage > bestDamage) {
      bestDamage = damage;
      bestBone = bone;
    }
  }
}

}  // namespace

bool Init() {
  // VTable hijacking (ClientModeShared::CreateMove()).
  constexpr int kCreateMoveVTableIdx = 25;
  hooks::originalCreateMove
    = memory::GetVFunc<hooks::CreateMoveFn>(interfaces::clientMode, kCreateMoveVTableIdx);
  memory::PutVFunc(interfaces::clientMode, kCreateMoveVTableIdx, &hooks::CreateMove);

  return true;
}

void Run(CUserCmd *cmd) {
  if (!isEnabled) {
    return;
  }

  // Check if we are trying to shoot.
  if (isLegit && !(cmd->buttons & CUserCmd::IN_ATTACK)) {
    return;
  }

  auto localPlayer = CLocalPlayer::The();

  if (localPlayer->IsDefusing()) {
    return;
  }

  auto activeWeapon = interfaces::entityList->GetEntityFromHandle<CBaseCombatWeapon>(
      localPlayer->GetActiveWeaponHandle());

  if (!activeWeapon ||
      activeWeapon->IsGrenade() ||
      activeWeapon->IsBomb() ||
      activeWeapon->IsKnife()) {
    return;
  }

  CBasePlayer *target = nullptr;
  CVector bestAngle = {};
  float bestFov = 5.f;

  for (auto player : localPlayer->GetAllOtherPlayers()) {
    if (player->IsDormant() || player->IsImmune() || !player->IsAlive()) {
      continue;
    }

    if (!shouldShootTeammates && !player->IsEnemy()) {
      continue;
    }

    if (!hacks::autowall::isEnabled && !player->IsVisible()) {
      continue;
    }

    float bestDamage = 0.f;
    Bone bone = Bone::HEAD;

    if (hacks::autowall::isEnabled) {
      GetBestBone(player, bestDamage, bone);
      if (bestDamage == 0.f) {
        continue;
      }
    }

    CVector localEyePosition = localPlayer->GetEyePosition();
    CVector enemyHeadPosition = player->GetBonePosition(bone);
    CVector aimPunch = localPlayer->GetAimPunchAngle();

    CVector enemyAngle =
        (enemyHeadPosition - localEyePosition).ToAngle() - (cmd->viewAngles + aimPunch);

    if (const float fov = std::hypot(enemyAngle.x, enemyAngle.y); fov < bestFov) {
      target = player;
      bestFov = fov;
      bestAngle = enemyAngle;
    }
  }

  cmd->viewAngles += bestAngle.Scale(hitChance);

  if (shouldAutoScope &&
      target &&
      activeWeapon->CanScope() &&
      !localPlayer->IsScoped()) {
    cmd->buttons |= CUserCmd::IN_SECOND_ATTACK;
  } else if (shouldAutoFire && target) {
    uint64_t timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();

    if (timestamp - lastFiredTimestamp >= fireDelayMs) {
      cmd->buttons |= CUserCmd::IN_ATTACK;
      lastFiredTimestamp = timestamp;
    }
  }
}

}  // namespace hacks::aimbot
