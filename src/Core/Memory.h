// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_MEMORY_H_
#define HAVOC_MEMORY_H_

#include <cstdint>
#include <utility>

namespace memory {

template <typename ReturnType, typename... Args>
constexpr ReturnType CallVFunc(void *vtable, const uint32_t index, Args... args) {
  using Function = ReturnType (*)(void *, decltype(args)...);
  return (*static_cast<Function **>(vtable))[index](vtable, std::forward<Args>(args)...);
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
