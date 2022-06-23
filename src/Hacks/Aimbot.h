// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_AIMBOT_H_
#define HAVOC_AIMBOT_H_

#include "SDK/CUserCmd.h"

namespace hacks::aimbot {

bool Init();
void Run(CUserCmd *cmd);

bool IsEnabled();
void Toggle();

}  // namespace hacks::aimbot

#endif  // HAVOC_AIMBOT_H_
