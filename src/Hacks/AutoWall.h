// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_AUTO_WALL_H_
#define HAVOC_AUTO_WALL_H_

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

inline bool isEnabled = false;

float GetDamage(const CVector &dest, bool teamCheck, FireBulletData &fData);
bool SimulateFireBullet(CBaseCombatWeapon *weapon,
                        bool teamCheck, FireBulletData &data);
bool HandleBulletPenetration(CWeaponInfo *weaponInfo, FireBulletData &data);
bool TraceToExit(CVector &end, CTrace &enterTrace, CVector start,
                 CVector dir, CTrace &exitTrace);
void ScaleDamage(const HitGroup hitGroup, CBasePlayer *enemy,
                 const float weaponArmorRatio, float &currentDamage);
float GetHitgroupDamageMultiplier(const HitGroup hitGroup);

}  // namespace hacks::autowall

#endif  // HAVOC_AUTO_WALL_H_
