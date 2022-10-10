// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_ANTI_AIM_H_
#define HAVOC_ANTI_AIM_H_

#include "SDK/CCSGOAnimState.h"
#include "SDK/CUserCmd.h"
#include "SDK/CVector.h"

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

inline bool isEnabled = false;

inline bool isYawEnabled = true;
inline bool isPitchEnabled = true;
inline AntiAimTypeY yawType = AntiAimTypeY::MAX_DELTA_FLIPPER;
inline AntiAimTypeY yawTypeFake = AntiAimTypeY::NONE;
inline AntiAimTypeX pitchType = AntiAimTypeX::STATIC_DOWN;

inline bool isHeadEdgeEnabled = false;
inline float headEdgeDistance = 25.f;

inline bool isLBYBreakerEnabled = false;
inline float lbyBreakerOffset = 180.f;

inline CVector realAngle{};
inline CVector fakeAngle{};

bool Init();
void Run(CUserCmd *cmd);
float GetMaxDelta(CCSGOAnimState *animState);

}  // namespace hacks::antiaim

#endif  // HAVOC_ANTI_AIM_H_
