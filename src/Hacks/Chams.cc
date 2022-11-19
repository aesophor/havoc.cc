// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#include "Chams.h"

#include "Settings.h"
#include "Core/Hooks.h"
#include "Core/Interfaces.h"
#include "SDK/CEntity.h"
#include "SDK/IMaterialSystem.h"

namespace hacks::chams {

namespace {

IMaterial *material = nullptr;

}  // namespace

bool Init() {
  // VTable hijacking (IStudioRender::DrawModel()).
  constexpr int kStudioRenderVTableIdx = 29;
  hooks::originalDrawModel
    = memory::GetVFunc<hooks::DrawModelFn>(interfaces::studioRender, kStudioRenderVTableIdx);
  memory::PutVFunc(interfaces::studioRender, kStudioRenderVTableIdx, &hooks::DrawModel);

  // Get our material to override.
  material = interfaces::materialSystem->FindMaterial("debug/debugambientcube");

  return true;
}

void DrawModel(IStudioRender *thisptr, void *results, const CDrawModelInfo &info,
               CMatrix3x4 *bones, float *flexWeights, float *flexDelayedWeights,
               const CVector &modelOrigin, const int flags) {
  if (!settings::chams::isEnabled) {
    hooks::originalDrawModel(thisptr, results, info, bones,
                             flexWeights, flexDelayedWeights, modelOrigin, flags);
    return;
  }

  auto localPlayer = CLocalPlayer::The();

  if (localPlayer && info.renderable) {
    constexpr float kHidden[3] = {119.f / 256, 160.f / 256, 255.f / 256};
    constexpr float kVisible[3] = {238.f / 256, 102.f / 256, 109.f / 256};

    // Alpha modulate
    interfaces::studioRender->SetAlphaModulation(1.f);

    // Show through walls
    material->SetMaterialVarFlag(IMaterial::VarFlag::IGNOREZ, true);
    interfaces::studioRender->SetColorModulation(kHidden);
    interfaces::studioRender->ForcedMaterialOverride(material);
    hooks::originalDrawModel(thisptr, results, info, bones,
                             flexWeights, flexDelayedWeights, modelOrigin, flags);

    // Don't show through walls
    material->SetMaterialVarFlag(IMaterial::VarFlag::IGNOREZ, false);
    interfaces::studioRender->SetColorModulation(kVisible);
    interfaces::studioRender->ForcedMaterialOverride(material);
    hooks::originalDrawModel(thisptr, results, info, bones,
                             flexWeights, flexDelayedWeights, modelOrigin, flags);

    // Reset the material override + return from hook.
    return interfaces::studioRender->ForcedMaterialOverride(nullptr);
  }

  hooks::originalDrawModel(thisptr, results, info, bones,
                           flexWeights, flexDelayedWeights, modelOrigin, flags);
}

}  // namespace hacks::chams
