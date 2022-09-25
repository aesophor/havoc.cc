// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#include "Glow.h"

#include "Core/Hooks.h"
#include "Core/Interfaces.h"
#include "SDK/CEntity.h"
#include "SDK/IGlowManager.h"

namespace hacks::glow {

bool Init() {
  // The signature of GlowEffectSpectator().
  constexpr uint8_t kSig[] = {0x55, 0x48, 0x89, 0xe5, 0x41, 0x57, 0x41, 0x56,
                              0x41, 0x55, 0x41, 0x54, 0x53, 0x48, 0x83, 0xec,
                              0x38, 0x48, 0x89, 0xcb, 0x49, 0x89, 0xf4};
  constexpr char kMask[] = "xxxxxxxxxxxxxxxxxxxxxxx";

  // Obtain the runtime address of GlowEffectSpectator().
  uintptr_t addr = interfaces::clientDylib->ScanSignature(kSig, kMask);

  if (!addr) {
    return false;
  }

  return hooks::Hook(addr, reinterpret_cast<uintptr_t>(hooks::GlowEffectSpectator));
}


bool GlowEffectSpectator(CBasePlayer *player, CLocalPlayer *localPlayer, GlowRenderStyle &glowStyle,
                         CVector &glowColor, float &alphaStart, float &alpha, float &timeStart,
                         float &timeTarget, bool &animate) {
  if (!localPlayer) {
    return false;
  }

  if (!isEnabled || (!shouldShowTeammates && !player->IsEnemy())) {
    alpha = 0.f;
    return false;
  }

  glowStyle = GlowRenderStyle::DEFAULT;
  glowColor = {242.0f / 255.0f, 117.0f / 255.0f, 117.0f / 255.0f};
  alpha = userAlpha;
  return true;
}

}  // namespace hacks::glow

