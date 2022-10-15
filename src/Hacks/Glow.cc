// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#include "Glow.h"

#include "Settings.h"
#include "Core/Hooks.h"
#include "Core/Interfaces.h"
#include "SDK/CEntity.h"
#include "SDK/IGlowManager.h"

namespace hacks::glow {

bool Init() {
  // The signature of GlowEffectSpectator().
  constexpr std::string_view kSig = "\x55\x48\x89\xe5\x41\x57\x41\x56\x41\x55\x41\x54\x53"
                                    "\x48\x83\xec\x38\x48\x89\xcb\x49\x89\xf4";

  uintptr_t addr = interfaces::clientDylib->ScanSignature(kSig);
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

  if (!settings::glow::isEnabled || (!settings::glow::shouldShowTeammates && !player->IsEnemy())) {
    alpha = 0.f;
    return false;
  }

  glowStyle = GlowRenderStyle::DEFAULT;
  glowColor = {242.0f / 255.0f, 117.0f / 255.0f, 117.0f / 255.0f};
  alpha = settings::glow::userAlpha;
  return true;
}

}  // namespace hacks::glow

