// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#include "AutoStrafe.h"

#include "Settings.h"
#include "Core/Interfaces.h"
#include "SDK/CEntity.h"
#include "Util/Math.h"

namespace hacks::autostrafe {

namespace {

void RunLegit(CLocalPlayer *localPlayer, CUserCmd* cmd) {
  if (localPlayer->GetFlags() & CEntity::FL_ONGROUND) {
    return;
  }

  if (cmd->buttons & CUserCmd::IN_FORWARD ||
      cmd->buttons & CUserCmd::IN_BACK ||
      cmd->buttons & CUserCmd::IN_MOVELEFT ||
      cmd->buttons & CUserCmd::IN_MOVERIGHT) {
    return;
  }

  if (cmd->mouseDx <= 1 && cmd->mouseDx >= -1) {
    return;
  }

  switch (settings::autostrafe::autoStrafeType) {
    case AutoStrafeType::FORWARDS:
      cmd->sideMove = cmd->mouseDx < 0.f ? -250.f : 250.f;
      break;
    case AutoStrafeType::BACKWARDS:
      cmd->sideMove = cmd->mouseDx < 0.f ? 250.f : -250.f;
      break;
    case AutoStrafeType::LEFT_SIDEWAYS:
      cmd->forwardMove = cmd->mouseDx < 0.f ? -250.f : 250.f;
      break;
    case AutoStrafeType::RIGHT_SIDEWAYS:
      cmd->forwardMove = cmd->mouseDx < 0.f ? 250.f : -250.f;
      break;
    default:
      break;
  }
}

void RunRage(CLocalPlayer *localPlayer, CUserCmd* cmd) {
  static bool leftRight;
  bool inMove
    = cmd->buttons & CUserCmd::IN_FORWARD ||
      cmd->buttons & CUserCmd::IN_BACK ||
      cmd->buttons & CUserCmd::IN_MOVELEFT ||
      cmd->buttons & CUserCmd::IN_MOVERIGHT;

  if (cmd->buttons & CUserCmd::IN_FORWARD &&
      localPlayer->GetVelocity().Length() <= 50.f) {
    cmd->forwardMove = 250.f;
  }

  float yawChange = 0.f;
  if (localPlayer->GetVelocity().Length() > 50.f) {
    yawChange = 30.0f * std::fabsf(30.0f / localPlayer->GetVelocity().Length());
  }

  auto activeWeapon
    = interfaces::entityList->GetEntityFromHandle<CBaseCombatWeapon>(
        localPlayer->GetActiveWeaponHandle());

  if (activeWeapon &&
      activeWeapon->GetAmmo() != 0 &&
      cmd->buttons & CUserCmd::IN_ATTACK) {
    yawChange = 0.f;
  }

  CVector viewAngles;
  interfaces::engine->GetViewAngles(viewAngles);

  if (!(localPlayer->GetFlags() & CEntity::FL_ONGROUND) && !inMove) {
    if (leftRight || cmd->mouseDx > 1) {
      viewAngles.y += yawChange;
      cmd->sideMove = 250.f;
    } else if (!leftRight || cmd->mouseDx < 1) {
      viewAngles.y -= yawChange;
      cmd->sideMove = -250.f;
    }
    leftRight = !leftRight;
  }

  viewAngles = viewAngles.NormalizeAngle().Clamp();

  CorrectMovement(viewAngles, cmd, cmd->forwardMove, cmd->sideMove);

  cmd->viewAngles = viewAngles;
}

}  // namespace

bool Init() {
  return true;
}

void CreateMove(CLocalPlayer *localPlayer, CUserCmd *cmd) {
  if (!settings::autostrafe::isEnabled) {
    return;
  }

  if (localPlayer->GetMoveType() == CEntity::MOVETYPE_LADDER ||
      localPlayer->GetMoveType() == CEntity::MOVETYPE_NOCLIP) {
    return;
  }

  switch (settings::autostrafe::autoStrafeType) {
    case AutoStrafeType::FORWARDS:
    case AutoStrafeType::BACKWARDS:
    case AutoStrafeType::LEFT_SIDEWAYS:
    case AutoStrafeType::RIGHT_SIDEWAYS:
      RunLegit(localPlayer, cmd);
      break;
    case AutoStrafeType::RAGE:
      RunRage(localPlayer, cmd);
      break;
  }
}

}  // namespace hacks::autostrafe
