// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#include "Bhop.h"

#include <atomic>

#include "Core/Hooks.h"
#include "Core/Interfaces.h"
#include "SDK/CEntity.h"
#include "SDK/CSequences.h"
#include "Util/Rand.h"

namespace {
std::atomic<bool> isEnabled = true;
}  // namespace

namespace hacks::skins {

bool Init() {
  // VTable hijacking (IBaseClientDLL::FrameStageNotify()).
  hooks::originalFrameStageNotify = memory::GetVFunc<hooks::FrameStageNotifyFn>(interfaces::client, 37);
  memory::PutVFunc(interfaces::client, 37, &hooks::FrameStageNotify);

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
    auto weapon = interfaces::entityList->GetEntityFromHandle<CBaseAttributableItem>(handle);

    if (!weapon) {
      continue;
    }

    switch (weapon->GetItemDefinitionIndex()) {
      case WEAPON_KNIFE:
      case WEAPON_KNIFE_T: {
        weapon->GetItemDefinitionIndex() = WEAPON_KNIFE_BUTTERFLY;
        weapon->GetFallbackWear() = 0.0001f;
        weapon->GetEntityQuality() = 3;
        weapon->GetItemIDHigh() = -1;
        break;
      }
      default:
        break;
    }
  }

  auto viewModel = interfaces::entityList->GetEntityFromHandle<CBaseViewModel>(
      localPlayer->GetViewModelHandle());

  if (!viewModel) {
    return;
  }

  auto activeWeapon = interfaces::entityList->GetEntityFromHandle<CBaseCombatWeapon>(
      viewModel->GetWeaponHandle());

  if (!activeWeapon) {
    return;
  }

  switch (activeWeapon->GetItemDefinitionIndex()) {
    case WEAPON_KNIFE_BUTTERFLY: {
      int idx = interfaces::modelInfo->GetModelIndex("models/weapons/v_knife_butterfly.mdl");
      viewModel->GetModelIndex() = idx;
      break;
    }
    default:
      break;
  }
}

void FixAnimations(CRecvProxyData *data, CBaseViewModel *viewModel, void *) {
  CEntity *owner = interfaces::entityList->GetEntityFromHandle(viewModel->GetOwnerHandle());

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
    }
  }
}

bool IsEnabled() {
  return isEnabled;
}

void Toggle() {
  isEnabled = !isEnabled;
}

}  // namespace hacks::skins
