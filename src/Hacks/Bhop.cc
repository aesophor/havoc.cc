// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#include "Bhop.h"

#include <atomic>

#include "Core/Hooks.h"
#include "Core/Interfaces.h"
#include "SDK/CEntity.h"

namespace {
std::atomic<bool> isEnabled = true;
}  // namespace

namespace hacks::bhop {

bool Init() {
  return true;
}

void Run(CUserCmd *cmd) {
  if (!(CLocalPlayer::The()->GetFlags() & CEntity::FL_ONGROUND)) {
    cmd->buttons &= ~CUserCmd::IN_JUMP;
  }
}

bool IsEnabled() {
  return isEnabled;
}

void Toggle() {
  isEnabled = !isEnabled;
}

}  // namespace hacks::bhop
