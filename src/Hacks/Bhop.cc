// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#include "Bhop.h"

#include "Core/Hooks.h"
#include "Core/Interfaces.h"
#include "SDK/CEntity.h"

namespace {

void strafe(CUserCmd *cmd) {
  auto localPlayer = CLocalPlayer::The(); 

  if (localPlayer->GetFlags() & CEntity::FL_ONGROUND) {
    return;
  }

  /*
  if (cmd->buttons & CUserCmd::IN_FORWARD ||
      cmd->buttons & CUserCmd::IN_BACK ||
      cmd->buttons & CUserCmd::IN_MOVELEFT ||
      cmd->buttons & CUserCmd::IN_MOVERIGHT) {
    return;
  }
  */

  if (cmd->mouseDx <= 1 && cmd->mouseDx >= -1) {
    return;
  }

	//switch (Settings::AutoStrafe::type) {
	//	case AutostrafeType::AS_FORWARDS:
  cmd->sideMove = cmd->mouseDx < 0.f ? -450.f : 450.f;
  /*
			break;
		case AutostrafeType::AS_BACKWARDS:
			cmd->sidemove = cmd->mousedx < 0.f ? 450.f : -450.f;
			break;
		case AutostrafeType::AS_LEFTSIDEWAYS:
			cmd->forwardmove = cmd->mousedx < 0.f ? -450.f : 450.f;
			break;
		case AutostrafeType::AS_RIGHTSIDEWAYS:
			cmd->forwardmove = cmd->mousedx < 0.f ? 450.f : -450.f;
			break;
		default:
			break;
	}
  */
}

}  // namespace


namespace hacks::bhop {

bool Init() {
  return true;
}

void Run(CUserCmd *cmd) {
  if (!isEnabled) {
    return;
  }

  if (!(CLocalPlayer::The()->GetFlags() & CEntity::FL_ONGROUND)) {
    cmd->buttons &= ~CUserCmd::IN_JUMP;
  }

  if (shouldStrafe) {
    strafe(cmd);
  }
}

}  // namespace hacks::bhop
