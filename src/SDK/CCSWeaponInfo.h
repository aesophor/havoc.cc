// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_C_CS_WEAPON_INFO_H_
#define HAVOC_C_CS_WEAPON_INFO_H_

#include <cstdint>

#include "SDK/CItemDefs.h"

inline constexpr auto kMaxShootSounds = 16;
inline constexpr auto kMaxWeaponString = 80;
inline constexpr auto kMaxWeaponPrefix = 16;
inline constexpr auto kMaxWeaponAmmoName = 32;

// https://github.com/SwagSoftware/Kisak-Strike/blob/ddc0a5e2f2c63b4c22a04ceadfd80f6c713f2603/game/shared/weapon_parse.h
class CHudTexture;
class CFileWeaponInfo {
 public:
  virtual ~CFileWeaponInfo() = default;

  // Each game can override this to get whatever values it wants from the script.
  virtual void Parse(void *pKeyValuesData, const char *weaponName) = 0;
  virtual void unused0() = 0;

  bool isScriptParsed;
  bool isHudElementsLoaded;

  char classNames[kMaxWeaponString];
  char printNames[kMaxWeaponString];

  char viewModel[kMaxWeaponString];
  char worldModel[kMaxWeaponString];
  char ammo1[kMaxWeaponAmmoName];
  char worldDroppedModel[kMaxWeaponString];

  char animationPrefix[kMaxWeaponPrefix];
  int slot;
  int position;
  int maxClip1;
  int maxClip2;
  int defaultClip1;
  int defaultClip2;
  int weight;
  int rumbleEffect;
  bool autoSwitchTo;
  bool autoSwitchFrom;
  int flags;

  char ammo2[kMaxWeaponAmmoName];
  char aiAddOn[kMaxWeaponString];

  // Sound blocks
  char shootSounds[CWeaponSound::WEAPONSOUND_SIZE][kMaxWeaponString];

  int ammoType;
  int ammo2Type;

  bool isMeleeWeapon;

  // This tells if the weapon was built right-handed (defaults to true).
  // This helps cl_righthand make the decision about whether to flip the model or not.
  bool isBuiltRightHanded;
  bool isFlippingAllowed;

  // Sprite data, read from the data file
  int iSpriteCount;
  CHudTexture *iconActive;
  CHudTexture *iconInactive;
  CHudTexture *iconAmmo;
  CHudTexture *iconAmmo2;
  CHudTexture *iconCrosshair;
  CHudTexture *iconAutoaim;
  CHudTexture *iconZoomedCrosshair;
  CHudTexture *iconZoomedAutoaim;
  CHudTexture *iconSmall;
};

// https://github.com/LWSS/Fuzion/blob/master/src/SDK/IClientEntity.h#L638
class CCSWeaponInfo : public CFileWeaponInfo {
 public:
  char *GetConsoleName() {
    return *reinterpret_cast<char **>(uintptr_t(this) + 0x8);
  }

  int GetClipSize() {
    return *reinterpret_cast<int *>(uintptr_t(this) + 0x20);
  }

  CWeaponType GetWeaponType() {
    return *reinterpret_cast<CWeaponType *>(uintptr_t(this) + 0x140);
  }

  int GetDamage() {
    return *reinterpret_cast<int *>(uintptr_t(this) + 0x16c);
  }

  float GetWeaponArmorRatio() {
    return *reinterpret_cast<float *>(uintptr_t(this) + 0x174);
  }

  float GetPenetration() {
    return *reinterpret_cast<float *>(uintptr_t(this) + 0x17c);
  }

  float GetRange() {
    return *reinterpret_cast<float *>(uintptr_t(this) + 0x188);
  }

  float GetRangeModifier() {
    return *reinterpret_cast<float *>(uintptr_t(this) + 0x18c);
  }

  float GetMaxPlayerSpeed() {
    return *reinterpret_cast<float *>(uintptr_t(this) + 0x1b8);
  }

  // i haven't updated the offset of this since 22 september 2021 (shark operation or whatever)
  // should be 0x8 to 0x10 higher
  int GetZoomLevels() {  // Doesn't work correctly on some weapons.
    return *reinterpret_cast<int *>(uintptr_t(this) + 0x23c);  // DT_WeaponCSBaseGun.m_zoomLevel ?
  }

  char *GetTracerEffect() {
    return *reinterpret_cast<char **>(uintptr_t(this) + 0x290);
  }

  int *GetTracerFrequency() {
    return reinterpret_cast<int *>(uintptr_t(this) + 0x298);
  }
};

#endif  // HAVOC_C_CS_WEAPON_INFO_H_
