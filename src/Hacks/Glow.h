// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_GLOW_H_
#define HAVOC_GLOW_H_

#include "SDK/CEntity.h"
#include "SDK/CVector.h"
#include "SDK/IGlowManager.h"

namespace hacks::glow {

bool Init();
bool GlowEffectSpectator(CBasePlayer *player, CLocalPlayer *localPlayer, GlowRenderStyle &glowStyle,
                         CVector &glowColor, float &alphaStart, float &alpha, float &timeStart,
                         float &timeTarget, bool &animate);

}  // namespace hacks::glow

#endif  // HAVOC_GLOW_H_
