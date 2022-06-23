// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#include "Core/Interfaces.h"
#include "Core/NetVars.h"
#include "Hacks/Aimbot.h"
#include "Hacks/Bhop.h"
#include "Hacks/Glow.h"
#include "Hacks/Skins.h"
#include "UI/Keys.h"

void __attribute__((constructor)) OnDylibLoad() {
  interfaces::Init();
  netvars::Init();
  ui::keys::Init();

  hacks::aimbot::Init();
  hacks::bhop::Init();
  hacks::glow::Init();
  hacks::skins::Init();
}
