// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_AUTO_WALL_H_
#define HAVOC_AUTO_WALL_H_

#include "SDK/CCSWeaponInfo.h"
#include "SDK/CEntity.h"
#include "SDK/HitGroup.h"
#include "SDK/IEngineTrace.h"

namespace hacks::autowall {

// https://github.com/AimTuxOfficial/AimTux/blob/master/src/Hacks/autowall.h
struct FireBulletData {
  CVector src;
  CTrace enterTrace;
  CVector direction;
  CTraceFilter filter;
  float traceLength;
  float traceLengthRemaining;
  float currentDamage;
  int penetrateCount;
};

bool Init();
float GetDamage(const CVector &dest, bool teamCheck, FireBulletData &fData);

}  // namespace hacks::autowall

#endif  // HAVOC_AUTO_WALL_H_
