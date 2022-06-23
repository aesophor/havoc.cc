// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_I_VPANEL_H_
#define HAVOC_I_VPANEL_H_

#include "Core/Memory.h"

class IVPanel {
 public:
  constexpr const char *GetName(uint32_t panel) {
    return memory::CallVFunc<const char *>(this, 36, panel);
  }
};

#endif  // HAVOC_I_VPANEL_H_
