// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_AUTO_STRAFE_H_
#define HAVOC_AUTO_STRAFE_H_

#include "SDK/CEntity.h"
#include "SDK/CUserCmd.h"

namespace hacks::autostrafe {

enum class AutoStrafeType : int {
  FORWARDS,
  BACKWARDS,
  LEFT_SIDEWAYS,
  RIGHT_SIDEWAYS,
  RAGE,
};

bool Init();
void CreateMove(CLocalPlayer *localPlayer, CUserCmd *cmd);

}  // namespace hacks::autostrafe

#endif  // HAVOC_AUTO_STRAFE_H_
