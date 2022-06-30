// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_AIMBOT_H_
#define HAVOC_AIMBOT_H_

#include "SDK/CUserCmd.h"

namespace hacks::aimbot {

inline bool isEnabled = false;
inline bool isLegit = true;
inline bool shouldAutoFire = false;
inline bool shouldAutoScope = false;
inline bool shouldPenetrate = false;
inline bool shouldShootTeammates = false;
inline float hitChance = 0.35f;
inline int fireDelayMs = 50;

bool Init();
void Run(CUserCmd *cmd);

}  // namespace hacks::aimbot

#endif  // HAVOC_AIMBOT_H_
