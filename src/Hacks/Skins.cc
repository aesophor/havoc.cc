// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#include "Skins.h"

#include "Core/Hooks.h"
#include "Core/Interfaces.h"
#include "SDK/CEntity.h"
#include "SDK/CSequences.h"
#include "Util/Rand.h"

namespace hacks::skins {

bool Init() {
  // VTable hijacking (IBaseClientDLL::FrameStageNotify()).
  constexpr int kFrameStageNotifyVTableIdx = 37;
  hooks::originalFrameStageNotify
    = memory::GetVFunc<hooks::FrameStageNotifyFn>(interfaces::client, kFrameStageNotifyVTableIdx);
  memory::PutVFunc(interfaces::client, kFrameStageNotifyVTableIdx, &hooks::FrameStageNotify);

  // Hook m_nSequence proxy function.
  for (CClientClass *cClass = interfaces::client->GetAllClasses(); cClass;
       cClass = cClass->next) {
    if (!strcmp(cClass->networkName, "CBaseViewModel")) {
      CRecvTable *table = cClass->table;

      for (int i = 0; i < table->count; i++) {
        CRecvProp *prop = &table->props[i];

        if (!prop || strcmp(prop->name, "m_nSequence")) {
          continue;
        }

        hooks::originalSetViewModelSequence = (hooks::SetViewModelSequenceFn) prop->proxyFn;
        prop->proxyFn = (void *) &hooks::SetViewModelSequence;
        break;
      }

      break;
    }
  }

  return true;
}

void Run() {
  auto localPlayer = CLocalPlayer::The();

  if (!localPlayer) {
    return;
  }

  for (const auto handle : localPlayer->GetWeaponHandles()) {
    auto weapon
      = interfaces::entityList->GetEntityFromHandle<CBaseAttributableItem>(handle);

    if (!weapon || knifeIdx == 0) {
      continue;
    }

    if (weapon->IsKnife()) {
      weapon->GetItemDefinitionIndex() = knifeModels[knifeIdx].itemDef;
      weapon->GetFallbackWear() = 0.0001f;
      weapon->GetEntityQuality() = 3;
      weapon->GetItemIDHigh() = -1;
    }
  }

  auto viewModel
    = interfaces::entityList->GetEntityFromHandle<CBaseViewModel>(
        localPlayer->GetViewModelHandle());

  if (!viewModel) {
    return;
  }

  auto activeWeapon
    = interfaces::entityList->GetEntityFromHandle<CBaseCombatWeapon>(
        viewModel->GetWeaponHandle());

  if (!activeWeapon) {
    return;
  }

  if (activeWeapon->IsKnife()) {
    viewModel->GetModelIndex()
        = interfaces::modelInfo->GetModelIndex(knifeModels[knifeIdx].modelPath);
  }
}

void FixAnimations(CRecvProxyData *data, CBaseViewModel *viewModel, void *) {
  auto owner
    = interfaces::entityList->GetEntityFromHandle(
        viewModel->GetOwnerHandle());

  // Compare the owner entity of this view model to the local player entity.
  if (owner && owner->GetIndex() == interfaces::engine->GetLocalPlayerIndex()) {
    // Get the filename of the current view model.
    CModel *model = interfaces::modelInfo->GetModel(viewModel->GetModelIndex());
    std::string modelName = interfaces::modelInfo->GetModelName(model);

    int &nSequence = data->value.Int;

    // Fix animations for butterfly knives.
    if (modelName == "models/weapons/v_knife_butterfly.mdl") {
      switch (nSequence) {
        case CSequences::DEFAULT_DRAW:
          nSequence = RandomInt(CSequences::BUTTERFLY_DRAW, CSequences::BUTTERFLY_DRAW2);
          break;
        case CSequences::DEFAULT_LOOKAT01:
          nSequence = RandomInt(CSequences::BUTTERFLY_LOOKAT01, CSequences::BUTTERFLY_LOOKAT03);
          break;
        default:
          nSequence++;
          break;
      }
    } else if (modelName == "models/weapons/v_knife_falchion_advanced.mdl") {
      switch (nSequence) {
        case CSequences::DEFAULT_IDLE1:
        case CSequences::DEFAULT_IDLE2:
          nSequence = CSequences::FALCHION_IDLE1;
          break;
        case CSequences::DEFAULT_HEAVY_MISS1:
          nSequence = RandomInt(CSequences::FALCHION_HEAVY_MISS1, CSequences::FALCHION_HEAVY_MISS1_NOFLIP);
          break;
        case CSequences::DEFAULT_LOOKAT01:
          nSequence = RandomInt(CSequences::FALCHION_LOOKAT01, CSequences::FALCHION_LOOKAT02);
          break;
        default:
          nSequence++;
          break;
      }
    } else if (modelName == "models/weapons/v_knife_survival_bowie.mdl") {
      switch (nSequence) {
        case CSequences::DEFAULT_IDLE1:
        case CSequences::DEFAULT_IDLE2:
          nSequence = CSequences::BOWIE_IDLE1;
          break;
        default:
          nSequence++;
          break;
      }
    }
  }
}

}  // namespace hacks::skins
