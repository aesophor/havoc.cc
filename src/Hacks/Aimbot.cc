// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#include "Aimbot.h"

#include "Settings.h"
#include "Core/Hooks.h"
#include "Core/Interfaces.h"
#include "Hacks/AutoWall.h"
#include "SDK/CEntity.h"
#include "SDK/HitGroup.h"
#include "Util/Rand.h"
#include "Util/Time.h"

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

CVector VelocityExtrapolate(CBasePlayer *player, const CVector &aimPos) {
	return aimPos + player->GetVelocity().Scale(interfaces::globalVars->intervalPerTick);
}

void RunLegit(CUserCmd *cmd) {
  // Check if we are trying to shoot.
  if (!(cmd->buttons & CUserCmd::IN_ATTACK)) {
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

  if (settings::aimbot::shouldOnlyAllowAWPandSSG && !activeWeapon->isAWPorSSG()) {
    return;
  }

  CBasePlayer *target = nullptr;
  CVector bestAngle = {};
  float bestFov = 5.f;

  for (auto [idx, player] : localPlayer->GetAllOtherPlayers()) {
    if (player->IsDormant() || player->IsImmune() || !player->IsAlive()) {
      continue;
    }

    if (!settings::aimbot::shouldShootTeammates && !player->IsEnemy()) {
      continue;
    }

    if (!settings::autowall::isEnabled && !player->IsVisible()) {
      continue;
    }

    float damage = 0.f;
    Bone bone = Bone::HEAD;

    if (settings::autowall::isEnabled) {
      GetBestBone(player, damage, bone);
      if (damage == 0.f) {
        continue;
      }
    }

    CVector localEyePosition = localPlayer->GetEyePosition();
    CVector enemyHeadPosition = player->GetBonePosition(bone);
    CVector aimPunch = localPlayer->GetAimPunchAngle();
    CVector enemyAngle
      = (enemyHeadPosition - localEyePosition).ToAngle() - (cmd->viewAngles + aimPunch);

    if (const float fov = std::hypot(enemyAngle.x, enemyAngle.y); fov < bestFov) {
      target = player;
      bestFov = fov;
      bestAngle = enemyAngle;
    }
  }

  cmd->viewAngles += bestAngle.Scale(settings::aimbot::hitChance);

  if (settings::aimbot::shouldAutoScope &&
      target &&
      activeWeapon->CanScope() &&
      !localPlayer->IsScoped()) {
    cmd->buttons |= CUserCmd::IN_SECOND_ATTACK;
  } else if (settings::aimbot::shouldAutoFire && target) {
    uint64_t timestamp = GetCurrentTimestampMs();

    if (timestamp - lastFiredTimestamp >= settings::aimbot::nextFireDelayMs) {
      cmd->buttons |= CUserCmd::IN_ATTACK;
      lastFiredTimestamp = timestamp;
      settings::aimbot::nextFireDelayMs = settings::aimbot::fireDelayMs + RandomInt(-25, 25);
    }
  }
}

void RunRage(CUserCmd *cmd) {
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
  float bestDamage = 0.f;
  Bone bestBone = Bone::HEAD;

  for (auto [idx, player] : localPlayer->GetAllOtherPlayers()) {
    if (player->IsDormant() || player->IsImmune() || !player->IsAlive()) {
      continue;
    }

    if (!settings::aimbot::shouldShootTeammates && !player->IsEnemy()) {
      continue;
    }

    float damage = 0.f;
    Bone bone = Bone::HEAD;

    if (!settings::autowall::isEnabled) {
      if (!player->IsVisible()) {
        continue;
      }
      target = player;
    } else {
      GetBestBone(player, damage, bone);
      if (damage > bestDamage) {
        target = player;
        bestDamage = damage;
        bestBone = bone;
      }
    }
  }

  if (!target) {
    return;
  }

  if (settings::aimbot::shouldAutoScope &&
      activeWeapon->CanScope() &&
      !localPlayer->IsScoped()) {
    cmd->buttons |= CUserCmd::IN_SECOND_ATTACK;
  } else if (settings::aimbot::shouldAutoFire) {
    uint64_t timestamp = GetCurrentTimestampMs();
    if (timestamp - lastFiredTimestamp >= settings::aimbot::nextFireDelayMs) {
      CVector localEyePosition = localPlayer->GetEyePosition();
      CVector targetPosition = target->GetBonePosition(bestBone);

      localEyePosition = VelocityExtrapolate(localPlayer, localEyePosition);
      targetPosition = VelocityExtrapolate(target, targetPosition);

      CVector aimPunch = localPlayer->GetAimPunchAngle();
      CVector enemyAngle = (targetPosition - localEyePosition).ToAngle() - aimPunch.Scale(2);

      cmd->viewAngles = enemyAngle;
      cmd->buttons |= CUserCmd::IN_ATTACK;

      lastFiredTimestamp = timestamp;
      settings::aimbot::nextFireDelayMs = settings::aimbot::fireDelayMs;
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

void CreateMove(CUserCmd *cmd) {
  if (!settings::aimbot::isEnabled) {
    return;
  }

  if (settings::aimbot::shouldRage) {
    RunRage(cmd);
  } else {
    RunLegit(cmd);
  }
}

}  // namespace hacks::aimbot
