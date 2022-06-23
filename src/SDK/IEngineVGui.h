// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_I_ENGINE_VGUI_H_
#define HAVOC_I_ENGINE_VGUI_H_

#include "Core/Memory.h"

enum EVGuiPanel {
  PANEL_ROOT = 0,
  PANEL_GAMEUIDLL,
  PANEL_CLIENTDLL,
  PANEL_TOOLS,
  PANEL_INGAMESCREENS,
  PANEL_GAMEDLL,
  PANEL_CLIENTDLL_TOOLS
};

class IEngineVGui {
 public:
  constexpr uintptr_t GetPanel(int32_t type) {
    return memory::CallVFunc<uintptr_t>(this, 1, type);
  }
};

#endif  // HAVOC_I_ENGINE_VGUI_H_
