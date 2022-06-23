// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_I_VENGINE_CLIENT_H_
#define HAVOC_I_VENGINE_CLIENT_H_

#include "Core/Memory.h"
#include "SDK/CMatrix.h"
#include "SDK/CVector.h"
#include "SDK/ISpatialQuery.h"

class IVEngineClient {
 public:
  constexpr void GetScreenSize(std::int32_t &width, std::int32_t &height) {
    memory::CallVFunc<void>(this, 5, std::ref(width), std::ref(height));
  }

  constexpr int32_t GetLocalPlayerIndex() {
    return memory::CallVFunc<std::int32_t>(this, 12);
  }

  constexpr void GetViewAngles(CVector &viewAngles) {
    memory::CallVFunc<void>(this, 18, std::ref(viewAngles));
  }

  constexpr void SetViewAngles(const CVector &viewAngles) {
    memory::CallVFunc<void>(this, 19, std::cref(viewAngles));
  }

  constexpr int32_t GetMaxClients() {
    return memory::CallVFunc<int>(this, 20);
  }

  constexpr bool IsInGame() {
    return memory::CallVFunc<bool>(this, 26);
  }

  constexpr const CMatrix4x4 &WorldToScreenMatrix() {
    return memory::CallVFunc<const CMatrix4x4 &>(this, 37);
  }

  constexpr ISpacialQuery *GetBSPTreeQuery() {
    return memory::CallVFunc<ISpacialQuery *>(this, 43);
  }
};

#endif  // HAVOC_I_VENGINE_CLIENT_H_
