// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_I_CLIENT_ENTITY_LIST_H_
#define HAVOC_I_CLIENT_ENTITY_LIST_H_

#include "SDK/CEntity.h"

class IClientEntityList {
 public:
  template <typename T = CEntity>
  constexpr std::add_pointer_t<T> GetEntityFromIndex(const int index) {
    return memory::CallVFunc<std::add_pointer_t<T>>(this, 3, index);
  }

  template <typename T = CEntity>
  constexpr std::add_pointer_t<T> GetEntityFromHandle(CBaseHandle handle) {
    // This doesn't work with 64-bit clients on linux and macOS, not sure why :(
    // return memory::CallVFunc<T *>(this, 4, handle);

    // So we'll use the following workaround at the moment.
    return GetEntityFromIndex<T>(handle & 0xfff);
  }
};

#endif  // HAVOC_I_CLIENT_ENTITY_LIST_H_
