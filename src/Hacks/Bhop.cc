// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#include "Bhop.h"

#include "Core/Hooks.h"
#include "Core/Interfaces.h"
#include "SDK/CEntity.h"

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
}

}  // namespace hacks::bhop
