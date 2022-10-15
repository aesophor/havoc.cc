// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_BHOP_H_
#define HAVOC_BHOP_H_

#include "SDK/CUserCmd.h"

namespace hacks::bhop {

bool Init();
void CreateMove(CUserCmd *cmd);

}  // namespace hacks::bhop

#endif  // HAVOC_BHOP_H_
