// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#include "Math.h"

#include "SDK/CUserCmd.h"
#include "SDK/CVector.h"

void CorrectMovement(const CVector &oldAngles, CUserCmd* cmd, float oldForward, float oldSidemove) {
  float deltaView;
  float f1;
  float f2;

  if (oldAngles.y < 0.f) {
    f1 = 360.f + oldAngles.y;
  } else {
    f1 = oldAngles.y;
  }

  if (cmd->viewAngles.y < 0.f) {
    f2 = 360.f + cmd->viewAngles.y;
  } else {
    f2 = cmd->viewAngles.y;
  }

  if (f2 < f1) {
    deltaView = std::abs(f2 - f1);
  } else {
    deltaView = 360.f - std::abs(f1 - f2);
  }

  deltaView = 360.f - deltaView;

  cmd->forwardMove
    = std::cos(Deg2Rad(deltaView)) * oldForward +
      std::cos(Deg2Rad(deltaView + 90.f)) * oldSidemove;

  cmd->sideMove
    = std::sin(Deg2Rad(deltaView)) * oldForward +
      std::sin(Deg2Rad(deltaView + 90.f)) * oldSidemove;
}
