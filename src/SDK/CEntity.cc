// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#include "CEntity.h"

#include <cmath>

#include "Core/Interfaces.h"
#include "SDK/IEngineTrace.h"

bool CBasePlayer::IsEnemy() {
  auto localPlayer = CLocalPlayer::The();
  return this != localPlayer && GetTeam() != localPlayer->GetTeam();
}

bool CBasePlayer::IsVisible(bool shouldPenetrate) {
  auto localPlayer = CLocalPlayer::The();
  if (!localPlayer) {
    return false;
  }

  CVector enemyHeadPosition = GetBonePosition(8);
  if (!enemyHeadPosition) {
    return false;
  }

  auto mask = MASK_SHOT;

  if (shouldPenetrate) {
    mask |= CONTENTS_GRATE;
  }

  CTrace trace;
  interfaces::engineTrace->TraceRay(
      CRay{ localPlayer->GetEyePosition(), enemyHeadPosition },
      mask,
      CTraceFilter { localPlayer },
      trace
  );

  constexpr auto kValidTraceDist = 0.97f;
  return trace.entity && trace.fraction >= kValidTraceDist;
}

float CBasePlayer::GetDistanceFrom(CBasePlayer *other) {
  CVector delta = other->GetEyePosition() - GetEyePosition();
  return std::hypot(delta.x, delta.y, delta.z);
}


CLocalPlayer *CLocalPlayer::The() {
  int idx = interfaces::engine->GetLocalPlayerIndex();
  return interfaces::entityList->GetEntityFromIndex<CLocalPlayer>(idx);
}

std::vector<CBasePlayer *> CLocalPlayer::GetAllOtherPlayers() {
  auto localPlayer = CLocalPlayer::The();
  size_t maxClients = interfaces::engine->GetMaxClients();

  std::vector<CBasePlayer *> ret;
  ret.reserve(maxClients);

  for (size_t i = 0; i < maxClients; i++) {
    auto player = interfaces::entityList->GetEntityFromIndex<CBasePlayer>(i);
    if (player && player != localPlayer) {
      ret.push_back(player);
    }
  }
  return ret;
}
