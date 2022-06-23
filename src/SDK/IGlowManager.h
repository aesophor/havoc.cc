// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_I_GLOW_MANAGER_H_
#define HAVOC_I_GLOW_MANAGER_H_

#include "SDK/CUtlVector.h"

enum class GlowRenderStyle {
  DEFAULT = 0,
  RIMGLOW3D,
  EDGE_HIGHLIGHT,
  EDGE_HIGHLIGHT_PULSE,
  LAST,
};

class CEntity;

class IGlowManager {
 public:
  class CGlowObject {
   public:
    int nextFreeSlot;

    // entity the object belongs to
    CEntity *entity;

    struct {
      float r;
      float g;
      float b;
      float a;
    } color;

    bool glowAlphaCappedByRenderAlpha;
    float glowAlphaFunctionOfMaxVelocity;
    float glowAlphaMax;
    float glowPulseOverdrive;

    bool renderWhenOccluded;
    bool renderWhenUnoccluded;

    bool fullBloomRender;
    int fullBloomStencilTestValue;
    int glowStyle;
    int splitScreenSlot;

    static constexpr int END_OF_FREE_LIST = -1;
    static constexpr int ENTRY_IN_USE = -2;

    constexpr bool IsUnused() const {
      return nextFreeSlot != ENTRY_IN_USE;
    }

    // helper function to set the glow color
    constexpr void SetColor(float r, float g, float b, float a = 1.f) {
      color.r = r;
      color.g = g;
      color.b = b;
      color.a = a;

      renderWhenOccluded = true;
      renderWhenUnoccluded = false;
    }
  };

  CUtlVector<CGlowObject> glowObjects;
};

#endif  // HAVOC_I_GLOW_MANAGER_H_
