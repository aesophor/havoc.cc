// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#include "Glow.h"

#include <atomic>

#include "Core/Hooks.h"
#include "Core/Interfaces.h"

namespace {
std::atomic<bool> isEnabled = true;
}  // namespace

namespace hacks::glow {

bool Init() {
  // The signature of GlowEffectSpectator().
  static const uint8_t sig[] = {0x55, 0x48, 0x89, 0xe5, 0x41, 0x57, 0x41, 0x56,
                                0x41, 0x55, 0x41, 0x54, 0x53, 0x48, 0x83, 0xec,
                                0x38, 0x48, 0x89, 0xcb, 0x49, 0x89, 0xf4};

  // Obtain the runtime address of GlowEffectSpectator().
  uintptr_t addr = interfaces::clientDylib->ScanSignature(sig, "xxxxxxxxxxxxxxxxxxxxxxx");

  if (!addr) {
    return false;
  }

  return hooks::Hook(addr, reinterpret_cast<uintptr_t>(hooks::GlowEffectSpectator));
}

bool IsEnabled() {
  return isEnabled;
}

void Toggle() {
  isEnabled = !isEnabled;
}

}  // namespace hacks::glow
