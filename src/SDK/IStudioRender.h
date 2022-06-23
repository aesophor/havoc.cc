// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_I_STUDIO_RENDER_H_
#define HAVOC_I_STUDIO_RENDER_H_

#include "Core/Memory.h"

// https://gitlab.com/KittenPopo/csgo-2018-source/-/blob/main/public/istudiorender.h#L201
class CStudioHdr;
class IClientRenderable;

class CDrawModelInfo {
 public:
  CStudioHdr *studioHdr;
  void *hardwareData;
  int32_t decals;
  int32_t skin;
  int32_t body;
  int32_t hitboxSet;
  IClientRenderable *renderable;
};

// https://gitlab.com/KittenPopo/csgo-2018-source/-/blob/main/public/istudiorender.h#L324
class IMaterial;
class IStudioRender {
 public:
  enum {
    STUDIORENDER_DRAW_ENTIRE_MODEL = 0,
    STUDIORENDER_DRAW_OPAQUE_ONLY = 0x01,
    STUDIORENDER_DRAW_TRANSLUCENT_ONLY = 0x02,
    STUDIORENDER_DRAW_GROUP_MASK = 0x03,
    STUDIORENDER_DRAW_NO_FLEXES = 0x04,
    STUDIORENDER_DRAW_STATIC_LIGHTING = 0x08,
    STUDIORENDER_DRAW_ACCURATETIME = 0x10,  // Use accurate timing when drawing the model.
    STUDIORENDER_DRAW_NO_SHADOWS = 0x20,
    STUDIORENDER_DRAW_GET_PERF_STATS = 0x40,
    STUDIORENDER_DRAW_WIREFRAME = 0x80,
    STUDIORENDER_DRAW_ITEM_BLINK = 0x100,
    STUDIORENDER_SHADOWDEPTHTEXTURE = 0x200,
    STUDIORENDER_UNUSED = 0x400,
    STUDIORENDER_SKIP_DECALS = 0x800,
    STUDIORENDER_MODEL_IS_CACHEABLE = 0x1000,
    STUDIORENDER_SHADOWDEPTHTEXTURE_INCLUDE_TRANSLUCENT_MATERIALS = 0x2000,
    STUDIORENDER_NO_PRIMARY_DRAW = 0x4000,
    STUDIORENDER_SSAODEPTHTEXTURE = 0x8000,
  };

  enum EOverrideType : int32_t {
    OVERRIDE_NORMAL = 0,
    OVERRIDE_BUILD_SHADOWS,
    OVERRIDE_DEPTH_WRITE,
    OVERRIDE_SELECTIVE,
    OVERRIDE_SSAO_DEPTH_WRITE,
  };

  constexpr void SetColorModulation(const float *color) {
    memory::CallVFunc<void>(this, 27, color);
  }

  constexpr void SetAlphaModulation(const float alpha) {
    memory::CallVFunc<void>(this, 28, alpha);
  }

  constexpr void ForcedMaterialOverride(IMaterial *material,
                                        const int32_t type = OVERRIDE_NORMAL,
                                        const int32_t index = -1) {
    memory::CallVFunc<void>(this, 33, material, type, index);
  }

  constexpr bool IsForcedMaterialOverride() {
    return memory::CallVFunc<bool>(this, 34);
  }
};

#endif  // HAVOC_I_STUDIO_RENDER_H_
