// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_AIMBOT_H_
#define HAVOC_AIMBOT_H_

#include "SDK/CEntity.h"
#include "SDK/CUserCmd.h"

namespace hacks::aimbot {

bool Init();
void CreateMove(CLocalPlayer *localPlayer, CUserCmd *cmd);

}  // namespace hacks::aimbot

#endif  // HAVOC_AIMBOT_H_
