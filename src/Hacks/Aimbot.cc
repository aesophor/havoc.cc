// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#include "Aimbot.h"

#include <chrono>

#include "Core/Hooks.h"
#include "Core/Interfaces.h"
#include "SDK/CEntity.h"

namespace {
uint64_t lastFiredTimestamp = 0;  // in milliseconds
}  // namespace

namespace hacks::aimbot {

bool Init() {
  // VTable hijacking (ClientModeShared::CreateMove()).
  hooks::originalCreateMove = memory::GetVFunc<hooks::CreateMoveFn>(interfaces::clientMode, 25);
  memory::PutVFunc(interfaces::clientMode, 25, &hooks::CreateMove);
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
    if (player->IsDormant() ||
        player->IsImmune() ||
        !player->IsAlive() ||
        (!shouldShootTeammates && !player->IsEnemy()) ||
        !player->IsVisible(shouldPenetrate)) {
      continue;
    }

    CVector localEyePosition = localPlayer->GetEyePosition();
    CVector enemyHeadPosition = player->GetBonePosition(8);
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
