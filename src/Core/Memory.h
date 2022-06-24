// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_MEMORY_H_
#define HAVOC_MEMORY_H_

#include <dlfcn.h>

#include <cstdint>
#include <unordered_map>
#include <utility>

namespace memory {

template <typename ReturnType, typename... Args>
ReturnType CallFunc(const char *symbol, Args... args) {
  using Function = ReturnType (*)(decltype(args)...);

  static std::unordered_map<const char *, void *> dlsymCache {};
  auto it = dlsymCache.find(symbol);
  void *addr = nullptr;

  if (it != dlsymCache.end()) [[likely]] {
    addr = it->second;
  } else {
    addr = dlsym(RTLD_DEFAULT, symbol);
    dlsymCache.insert(std::make_pair(symbol, addr));
  }

  return reinterpret_cast<Function>(addr)(std::forward<Args>(args)...);
}

template <typename ReturnType, typename... Args>
constexpr ReturnType CallVFunc(void *vtable, const uint32_t index, Args... args) {
  using Function = ReturnType (*)(void *, decltype(args)...);
  return (*reinterpret_cast<Function **>(vtable))[index](vtable, std::forward<Args>(args)...);
}

// Read the `index`-th entry from the vtable.
template <typename FuncPtr>
constexpr FuncPtr GetVFunc(void *vtable, const uint32_t index) {
  return reinterpret_cast<FuncPtr>((*static_cast<void ***>(vtable))[index]);
}

// Hijack the `index`-th entry in the vtable with `newFunc`.
template <typename FuncPtr>
constexpr void PutVFunc(void *vtable, const uint32_t index, FuncPtr newFunc) {
  (*static_cast<void ***>(vtable))[index] = reinterpret_cast<void *>(newFunc);
}

}  // namespace memory

#endif  // HAVOC_MEMORY_H_
