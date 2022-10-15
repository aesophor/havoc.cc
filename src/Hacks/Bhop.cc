// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#include "Bhop.h"

#include "Settings.h"
#include "SDK/CEntity.h"

namespace hacks::bhop {

bool Init() {
  return true;
}

void CreateMove(CUserCmd *cmd) {
  if (!settings::bhop::isEnabled) {
    return;
  }

  if (!(CLocalPlayer::The()->GetFlags() & CEntity::FL_ONGROUND)) {
    cmd->buttons &= ~CUserCmd::IN_JUMP;
  }
}

}  // namespace hacks::bhop
