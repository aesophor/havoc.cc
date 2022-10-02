// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#include "AutoWall.h"

#include <cmath>
#include <cstdio>

#include "Core/Hooks.h"
#include "Core/Interfaces.h"
#include "SDK/CEntity.h"
#include "SDK/IPhysicsSurfaceProps.h"

namespace hacks::autowall {

float GetDamage(const CVector &dest, bool teamCheck, FireBulletData &fData) {
  auto localPlayer = CLocalPlayer::The();
  float damage = 0.f;

  FireBulletData data = {
    .src = localPlayer->GetEyePosition(),
    .filter = {localPlayer},
  };

  data.direction = (dest - data.src).ToAngle();

  auto activeWeapon
    = interfaces::entityList->GetEntityFromHandle<CBaseCombatWeapon>(
        localPlayer->GetActiveWeaponHandle());

  if (!activeWeapon) {
    return -1.0f;
  }

  if (SimulateFireBullet(activeWeapon, teamCheck, data)) {
    damage = data.currentDamage;
  }
  printf("SimulateFireBullet() done. damage: %f\n", damage);

  fData = data;
  return damage;
}

bool SimulateFireBullet(CBaseCombatWeapon* weapon,
                        bool teamCheck, FireBulletData &data) {
  auto localPlayer = CLocalPlayer::The();
  CCSWeaponInfo *weaponInfo = weapon->GetCSWpnData();

  printf("CCSWeaponInfo = {weaponType: %d, damage: %d}\n",
      weaponInfo->GetWeaponType(), weaponInfo->GetDamage());

  data.penetrateCount = 4;
  data.traceLength = 0.0f;
  data.currentDamage = weaponInfo->GetDamage();

  while (data.penetrateCount > 0 && data.currentDamage >= 1.0f) {
    data.traceLengthRemaining = weaponInfo->GetRange() - data.traceLength;

    CVector end = data.src + data.direction.Scale(data.traceLengthRemaining);

    // data.enterTrace
    interfaces::engineTrace->TraceRay(
        CRay{data.src, end},
        MASK_SHOT,
        CTraceFilter{localPlayer},
        data.enterTrace
    );

    interfaces::engineTrace->TraceRay(
        CRay{data.src, end + data.direction.Scale(40.f)},
        MASK_SHOT,
        data.filter,
        data.enterTrace
    );

    interfaces::engineTrace->TraceRay(
        CRay{data.src, end + data.direction.Scale(40.f)},
        MASK_SHOT,
        CTraceFilter{localPlayer},
        data.enterTrace
    );

    if (data.enterTrace.fraction == 1.0f) {
      break;
    }

    if (static_cast<int>(data.enterTrace.hitgroup) <= static_cast<int>(HitGroup::RIGHTLEG) &&
        static_cast<int>(data.enterTrace.hitgroup) > static_cast<int>(HitGroup::GENERIC)) {
      data.traceLength += data.enterTrace.fraction * data.traceLengthRemaining;
      data.currentDamage *= std::powf(weaponInfo->GetRangeModifier(), data.traceLength * 0.002f);

      CBasePlayer* player = (CBasePlayer*) data.enterTrace.entity;
      if (teamCheck && player->GetTeam() == localPlayer->GetTeam()) {
        return false;
      }

      ScaleDamage(data.enterTrace.hitgroup, player,
                  weaponInfo->GetWeaponArmorRatio(), data.currentDamage);
      return true;
    }

    if (!HandleBulletPenetration(weaponInfo, data)) {
      break;
    }
  }

  return false;
}

bool HandleBulletPenetration(CCSWeaponInfo* weaponInfo, FireBulletData &data) {
  CSurfaceData *enterSurfaceData
    = interfaces::physics->GetSurfaceData(data.enterTrace.surface.surfaceProps);

  int enterMaterial = enterSurfaceData->game.material;
  float enterSurfPenetrationMod = *(float*)((uint8_t*) enterSurfaceData + 76);

  data.traceLength += data.enterTrace.fraction * data.traceLengthRemaining;
  data.currentDamage *= std::powf(weaponInfo->GetRangeModifier(), data.traceLength * 0.002f);

  if (data.traceLength > 3000.f || enterSurfPenetrationMod < 0.1f) {
    data.penetrateCount = 0;
  }

  if (data.penetrateCount <= 0) {
    return false;
  }

  CVector dummy;
  CTrace traceExit;

  if (!TraceToExit(dummy, data.enterTrace, data.enterTrace.endPos, data.direction, traceExit)) {
    return false;
  }

  CSurfaceData *exitSurfaceData
    = interfaces::physics->GetSurfaceData(traceExit.surface.surfaceProps);
  int exitMaterial = exitSurfaceData->game.material;

  float exitSurfPenetrationMod = *(float*)((uint8_t*) exitSurfaceData + 76);
  float finalDamageModifier = 0.16f;
  float combinedPenetrationModifier = 0.0f;

  if ((data.enterTrace.contents & CONTENTS_GRATE) != 0 ||
      enterMaterial == 89 ||
      enterMaterial == 71) {
    combinedPenetrationModifier = 3.0f;
    finalDamageModifier = 0.05f;
  } else {
    combinedPenetrationModifier = (enterSurfPenetrationMod + exitSurfPenetrationMod) * 0.5f;
  }

  if (enterMaterial == exitMaterial) {
    if (exitMaterial == 87 || exitMaterial == 85) {
      combinedPenetrationModifier = 3.0f;
    } else if (exitMaterial == 76) {
      combinedPenetrationModifier = 2.0f;
    }
  }

  float v34 = fmaxf(0.f, 1.0f / combinedPenetrationModifier);
  float v35 = (data.currentDamage * finalDamageModifier) +
    v34 * 3.0f * std::fmaxf(0.0f, (3.0f / weaponInfo->GetPenetration()) * 1.25f);

  CVector delta = traceExit.endPos - data.enterTrace.endPos;
  float thickness = std::hypot(delta.x, delta.y, delta.z);

  thickness *= thickness;
  thickness *= v34;
  thickness /= 24.0f;

  float lostDamage = std::fmaxf(0.0f, v35 + thickness);

  if (lostDamage > data.currentDamage) {
    return false;
  }

  if (lostDamage >= 0.0f) {
    data.currentDamage -= lostDamage;
  }

  if (data.currentDamage < 1.0f) {
    return false;
  }

  data.src = traceExit.endPos;
  data.penetrateCount--;

  return true;
}

bool TraceToExit(CVector &end, CTrace &enterTrace, CVector start,
                 CVector dir, CTrace &exitTrace) {
  float distance = 0.0f;

  while (distance <= 90.0f) {
    distance += 4.0f;
    end = start + dir.Scale(distance);

    auto pointContents
      = interfaces::engineTrace->GetPointContents(
          end, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr);

    if (pointContents & MASK_SHOT_HULL && !(pointContents & CONTENTS_HITBOX)) {
      continue;
    }

    auto newEnd = end - dir.Scale(4.0f);

    interfaces::engineTrace->TraceRay(
        CRay{end, newEnd},
        MASK_SHOT,
        CTraceFilter{},
        exitTrace
    );

    if (exitTrace.startSolid && exitTrace.surface.flags & SURF_HITBOX) {
      interfaces::engineTrace->TraceRay(
          CRay{end, start},
          0x600400b,
          CTraceFilter{exitTrace.entity},
          exitTrace
      );

      if ((exitTrace.fraction < 1.0f || exitTrace.allSolid) && !exitTrace.startSolid) {
        end = exitTrace.endPos;
        return true;
      }
      continue;
    }

    if (!(exitTrace.fraction < 1.0 || exitTrace.allSolid || exitTrace.startSolid) || exitTrace.startSolid) {
      if (exitTrace.entity) {
        if (enterTrace.entity &&
            enterTrace.entity == interfaces::entityList->GetEntityFromIndex(0)) {
          return true;
        }
      }
      continue;
    }

    if (exitTrace.surface.flags >> 7 & 1 && !(enterTrace.surface.flags >> 7 & 1)) {
      continue;
    }

    if (exitTrace.plane.normal.Dot(dir) <= 1.0f) {
      auto fraction = exitTrace.fraction * 4.0f;
      end -= dir.Scale(fraction);
      return true;
    }
	}

	return false;
}

void ScaleDamage(const HitGroup hitGroup, CBasePlayer* enemy,
                 const float weaponArmorRatio, float &currentDamage) {
  currentDamage *= GetHitgroupDamageMultiplier(hitGroup);

  if (enemy->GetArmor() <= 0) {
    return;
  }

  if (hitGroup != HitGroup::HEAD || enemy->HasHelmet()) {
    currentDamage *= weaponArmorRatio * 0.5f;
	}
}

float GetHitgroupDamageMultiplier(const HitGroup hitGroup) {
  switch (hitGroup) {
    case HitGroup::HEAD:
      return 4.0f;
    case HitGroup::CHEST:
    case HitGroup::LEFTARM:
    case HitGroup::RIGHTARM:
      return 1.0f;
    case HitGroup::STOMACH:
      return 1.25f;
    case HitGroup::LEFTLEG:
    case HitGroup::RIGHTLEG:
      return 0.75f;
    default:
      return 1.0f;
  }
}

}  // namespace hacks::autowall
