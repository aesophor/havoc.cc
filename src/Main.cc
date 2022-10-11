// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#include "Core/Interfaces.h"
#include "Core/NetVars.h"
#include "GUI/GUI.h"
#include "Hacks/Aimbot.h"
#include "Hacks/AntiAim.h"
#include "Hacks/AutoStrafe.h"
#include "Hacks/AutoWall.h"
#include "Hacks/Bhop.h"
#include "Hacks/Chams.h"
#include "Hacks/Glow.h"
#include "Hacks/Skins.h"

void __attribute__((constructor)) OnDylibLoad() {
  interfaces::Init();
  netvars::Init();
  gui::Init();

  hacks::aimbot::Init();
  hacks::antiaim::Init();
  hacks::autostrafe::Init();
  hacks::autowall::Init();
  hacks::bhop::Init();
  hacks::chams::Init();
  hacks::glow::Init();
  hacks::skins::Init();
}
