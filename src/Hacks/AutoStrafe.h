// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_AUTO_STRAFE_H_
#define HAVOC_AUTO_STRAFE_H_

#include "SDK/CUserCmd.h"

namespace hacks::autostrafe {

enum class AutoStrafeType : int {
  FORWARDS,
  BACKWARDS,
  LEFT_SIDEWAYS,
  RIGHT_SIDEWAYS,
  RAGE,
};

inline bool isEnabled = false;
inline AutoStrafeType autoStrafeType = AutoStrafeType::RAGE;

bool Init();
void Run(CUserCmd *cmd);

}  // namespace hacks::autostrafe

#endif  // HAVOC_AUTO_STRAFE_H_
