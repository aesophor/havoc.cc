// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_ANTI_AIM_H_
#define HAVOC_ANTI_AIM_H_

#include "SDK/CCSGOAnimState.h"
#include "SDK/CUserCmd.h"

namespace hacks::antiaim {

enum class AntiAimTypeY : int {
  NONE,
  MAX_DELTA_LEFT,
  MAX_DELTA_RIGHT,
  MAX_DELTA_FLIPPER,
  MAX_DELTA_LBY_AVOID,
};

enum class AntiAimTypeX : int {
  STATIC_UP,
  STATIC_DOWN,
  FRONT,
  DANCE,
};

bool Init();
void CreateMove(CUserCmd *cmd);
float GetMaxDelta(CCSGOAnimState *animState);

}  // namespace hacks::antiaim

#endif  // HAVOC_ANTI_AIM_H_
