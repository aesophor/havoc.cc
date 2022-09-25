// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_I_MATERIAL_SYSTEM_H_
#define HAVOC_I_MATERIAL_SYSTEM_H_

#include <cstdint>

#include "Core/Memory.h"

class IMaterial {
 public:
  enum class VarFlag : int32_t {
    IGNOREZ = 1 << 15,
    WIREFRAME = 1 << 28
  };

  constexpr void SetMaterialVarFlag(const IMaterial::VarFlag flag, const bool on) {
    memory::CallVFunc<void>(this, 29, static_cast<int32_t>(flag), on);
  }
};

class IMaterialSystem {
 public:
  constexpr IMaterial *FindMaterial(const char *name) {
    return memory::CallVFunc<IMaterial *>(this, 84, name, nullptr, true, nullptr);
  }
};

#endif  // HAVOC_I_MATERIAL_SYSTEM_H_
