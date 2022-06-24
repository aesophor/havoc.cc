// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#include "Aimbot.h"

#include "Core/Hooks.h"
#include "Core/Interfaces.h"
#include "SDK/CEntity.h"

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
  if (!(cmd->buttons & CUserCmd::IN_ATTACK)) {
    return;
  }

  auto localPlayer = CLocalPlayer::The();

  if (localPlayer->IsDefusing()) {
    return;
  }

  CVector bestAngle;
  float bestFov = 5.f;

  for (auto player : localPlayer->GetAllOtherPlayers()) {
    if (player->IsDormant() ||
        player->IsImmune() ||
        !player->IsAlive() ||
        !player->IsEnemy() ||
        !player->IsVisible()) {
      continue;
    }

    CVector localEyePosition = localPlayer->GetEyePosition();
    CVector enemyHeadPosition = player->GetBonePosition(8);
    CVector aimPunch = localPlayer->GetAimPunchAngle();

    CVector enemyAngle =
        (enemyHeadPosition - localEyePosition).ToAngle() - (cmd->viewAngles + aimPunch);

    if (const float fov = std::hypot(enemyAngle.x, enemyAngle.y); fov < bestFov) {
      bestFov = fov;
      bestAngle = enemyAngle;
    }
  }

  cmd->viewAngles += bestAngle.Scale(0.35f);
}

}  // namespace hacks::aimbot
