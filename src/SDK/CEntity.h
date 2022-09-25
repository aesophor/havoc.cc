// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_C_ENTITY_H_
#define HAVOC_C_ENTITY_H_

#include <array>
#include <vector>

#include "Core/Memory.h"
#include "Core/NetVars.h"
#include "SDK/CClientClass.h"
#include "SDK/CItemDefs.h"
#include "SDK/CMatrix.h"
#include "SDK/CVector.h"
#include "SDK/HitGroup.h"
#include "Util/TypeTraits.h"

using CBaseHandle = int;

inline constexpr auto kMaxShootSounds = 16;
inline constexpr auto kMaxWeaponString = 80;
inline constexpr auto kMaxWeaponPrefix = 16;
inline constexpr auto kMaxWeaponAmmoName = 32;

struct Model {
  char name[255];
};

class ICollideable {
 public:
  virtual void Unused_0();
  virtual const CVector &OBBMins() const;
  virtual const CVector &OBBMaxs() const;
};

class IHandleEntity {};

class IClientUnknown;
class IClientRenderable {
 public:
  IClientUnknown* GetIClientUnknown() {
    return memory::CallVFunc<IClientUnknown *>(this + 0x8, 0);
  }

  Model *GetModel() {
    return memory::CallVFunc<Model *>(this + 0x8, 8);
  }

  bool SetupBones(CMatrix3x4 *boneMatrix, int nMaxBones, int nBoneMask,
                  float currentTime = 0) {
    return memory::CallVFunc<bool>(this + 0x8, 13, boneMatrix, nMaxBones, nBoneMask, currentTime);
  }
};

class IClientNetworkable {
 public:
  CClientClass *GetClientClass() {
    return memory::CallVFunc<CClientClass *>(this + 0x10, 2);
  }
};

class IClientThinkable {};

class IClientEntity;
class CEntity;
class CBaseViewModel;
class IClientUnknown : public IHandleEntity {
 public:
  virtual ICollideable *GetCollideable() = 0;
  virtual IClientNetworkable *GetClientNetworkable() = 0;
  virtual IClientRenderable *GetClientRenderable() = 0;
  virtual IClientEntity *GetClientEntity() = 0;

  // XXX: It seems that on macOS, the virtual function at index 4
  // doesn't return a pointer to CEntity. Reverse this and find out why.
  virtual CEntity *GetEntity() = 0;
};

class IClientEntity : public IClientUnknown,
                      public IClientRenderable,
                      public IClientNetworkable,
                      public IClientThinkable {
 public:
  int &GetIndex() {
    return *reinterpret_cast<int *>((uintptr_t(this) + 0x94));
  }
};

class CBaseAttributableItem;
class CEntity : public IClientEntity {
  MAKE_NONCOPYABLE(CEntity);
  MAKE_NONMOVABLE(CEntity);

 public:
  enum EFlags : int32_t {
    FL_ONGROUND = (1 << 0),
    FL_DUCKING = (1 << 1),
    FL_ANIMDUCKING = (1 << 2),
    FL_WATERJUMP = (1 << 3),
    FL_ONTRAIN = (1 << 4),
    FL_INRAIN = (1 << 5),
    FL_FROZEN = (1 << 6),
    FL_ATCONTROLS = (1 << 7),
    FL_CLIENT = (1 << 8),
    FL_FAKECLIENT = (1 << 9),
    FL_INWATER = (1 << 10),
    FL_FLY = (1 << 11),
    FL_SWIM = (1 << 12),
    FL_CONVEYOR = (1 << 13),
    FL_NPC = (1 << 14),
    FL_GODMODE = (1 << 15),
    FL_NOTARGET = (1 << 16),
    FL_AIMTARGET = (1 << 17),
    FL_PARTIALGROUND = (1 << 18),
    FL_STATICPROP = (1 << 19),
    FL_GRAPHED = (1 << 20),
    FL_GRENADE = (1 << 21),
    FL_STEPMOVEMENT = (1 << 22),
    FL_DONTTOUCH = (1 << 23),
    FL_BASEVELOCITY = (1 << 24),
    FL_WORLDBRUSH = (1 << 25),
    FL_OBJECT = (1 << 26),
    FL_KILLME = (1 << 27),
    FL_ONFIRE = (1 << 28),
    FL_DISSOLVING = (1 << 29),
    FL_TRANSRAGDOLL = (1 << 30),
    FL_UNBLOCKABLE_BY_PLAYER = (1 << 31)
  };

  enum WeaponType : int {
    WEAPONTYPE_KNIFE = 0,
    WEAPONTYPE_PISTOL = 1,
    WEAPONTYPE_SUBMACHINEGUN = 2,
    WEAPONTYPE_RIFLE = 3,
    WEAPONTYPE_SHOTGUN = 4,
    WEAPONTYPE_SNIPER = 5,
    WEAPONTYPE_MACHINEGUN = 6,
    WEAPONTYPE_C4 = 7,
    WEAPONTYPE_PLACEHOLDER = 8,
    WEAPONTYPE_GRENADE = 9,
    WEAPONTYPE_HEALTHSHOT = 11,
    WEAPONTYPE_FISTS = 12,
    WEAPONTYPE_BREACHCHARGE = 13,
    WEAPONTYPE_BUMPMINE = 14,
    WEAPONTYPE_TABLET = 15,
    WEAPONTYPE_MELEE = 16
  };

  enum WeaponSound {
    WEAPONSOUND_EMPTY,
    WEAPONSOUND_SINGLE,
    WEAPONSOUND_SINGLE_NPC,
    WEAPONSOUND_WPN_DOUBLE, // Can't be "DOUBLE" because windows.h uses it.
    WEAPONSOUND_DOUBLE_NPC,
    WEAPONSOUND_BURST,
    WEAPONSOUND_RELOAD,
    WEAPONSOUND_RELOAD_NPC,
    WEAPONSOUND_MELEE_MISS,
    WEAPONSOUND_MELEE_HIT,
    WEAPONSOUND_MELEE_HIT_WORLD,
    WEAPONSOUND_SPECIAL1,
    WEAPONSOUND_SPECIAL2,
    WEAPONSOUND_SPECIAL3,
    WEAPONSOUND_TAUNT,
    WEAPONSOUND_FAST_RELOAD,
    // Add new shoot sound types here
    WEAPONSOUND_SIZE
  };

  NETVAR(GetTeam, "CBaseEntity->m_iTeamNum", int &);
  NETVAR(GetWeaponHandles, "CBaseCombatCharacter->m_hMyWeapons", std::array<CBaseHandle, 64> &);
  NETVAR(GetActiveWeaponHandle, "CBaseCombatCharacter->m_hActiveWeapon", CBaseHandle &);
  NETVAR(GetVecOrigin, "CBaseEntity->m_vecOrigin", CVector &);
  NETVAR(GetCollideable, "CBaseEntity->m_Collision", ICollideable *);
  NETVAR(GetSpotted, "CBaseEntity->m_bSpotted", bool &);

  int GetId() {
    return *reinterpret_cast<int *>((uintptr_t(this) + 0x94));
  }

  bool IsDormant() {
    return *reinterpret_cast<bool *>((uintptr_t(this) + 0x125));
  }
};

class CBasePlayer : public CEntity {
 public:
  NETVAR(GetHealth, "CBasePlayer->m_iHealth", int &);
  NETVAR(GetArmor, "CBasePlayer->m_ArmorValue", int &);
  NETVAR(GetMoney, "CBasePlayer->m_iAccount", int &);
  NETVAR(HasHelmet, "CBasePlayer->m_bHasHelmet", bool &);
  NETVAR(GetLifeState, "CBasePlayer->m_lifeState", unsigned char &);
  NETVAR(GetViewModelHandle, "CBasePlayer->m_hViewModel[0]", CBaseHandle &);
  NETVAR(GetVecViewOffset, "CBasePlayer->m_vecViewOffset[0]", CVector &);
  NETVAR(GetFlags, "CBasePlayer->m_fFlags", int &);
  NETVAR(GetObserverMode, "CBasePlayer->m_iObserverMode", int &);
  NETVAR(GetObserverTarget, "CBasePlayer->m_hObserverTarget", void *);
  NETVAR(GetLastPlaceName, "CBasePlayer->m_szLastPlaceName", char *);
  NETVAR(GetVelocity, "CBasePlayer->m_vecVelocity[0]", CVector &);
  NETVAR(GetHeadRotation, "CBasePlayer->m_angEyeAngles[0]", CVector &);
  NETVAR(GetAimPunchAngle, "CBasePlayer->m_aimPunchAngle", CVector &);
  NETVAR(GetViewPunchAngle, "CBasePlayer->m_viewPunchAngle", CVector &);
  NETVAR(GetLowerBodyYawTarget, "CBasePlayer->m_flLowerBodyYawTarget", float &);
  NETVAR(GetTickBase, "CBasePlayer->m_nTickBase", unsigned int &);

  NETVAR(GetFOV, "CCSPlayer->m_iDefaultFOV", float &);
  NETVAR(GetFlashDuration, "CCSPlayer->m_flFlashDuration", float &);
  NETVAR(GetFlashMaxAlpha, "CCSPlayer->m_flFlashMaxAlpha", float &);
  NETVAR(IsDefusing, "CCSPlayer->m_bIsDefusing", bool &);
  NETVAR(IsGrabbingHostage, "CCSPlayer->m_bIsGrabbingHostage", bool &);
  NETVAR(IsScoped, "CCSPlayer->m_bIsScoped", bool &);
  NETVAR(IsRescuing, "CCSPlayer->m_bIsRescuing", bool &);
  NETVAR(IsImmune, "CCSPlayer->m_bGunGameImmunity", bool &);
  NETVAR(IsGhost, "CCSPlayer->m_bIsPlayerGhost", bool &);

  bool IsAlive() {
    return GetHealth() > 0;
  }

  CVector GetEyePosition() {
    return GetVecOrigin() + GetVecViewOffset();
  }

  CVector GetBonePosition(const Bone bone) {
    constexpr size_t kMaxStudioBones = 128;
    constexpr size_t kBoneUsedByHitbox = 256;
    CMatrix3x4 bones[kMaxStudioBones];

    if (!SetupBones(bones, kMaxStudioBones, kBoneUsedByHitbox, 0)) {
      return GetVecOrigin();
    }
    return bones[static_cast<int>(bone)].Origin();
  }

  bool IsEnemy();
  bool IsVisible();
  float GetDistanceFrom(CBasePlayer *player);
};

class CLocalPlayer : public CBasePlayer {
 public:
  static CLocalPlayer *The();

  std::vector<CBasePlayer *> GetAllOtherPlayers();
};

class CBasePlantedC4 : public CEntity {
 public:
  NETVAR(IsBombTicking, "CPlantedC4->m_bBombTicking", bool &);
  NETVAR(GetBombTime, "CPlantedC4->m_flC4Blow", float &);
  NETVAR(IsBombDefused, "CPlantedC4->m_bBombDefused", bool &);
  NETVAR(GetBombDefuser, "CPlantedC4->m_hBombDefuser", int &);
};

class CBaseAttributableItem : public CEntity {
 public:
  NETVAR(GetItemDefinitionIndex, "CBaseAttributableItem->m_iItemDefinitionIndex", short &);
  NETVAR(GetItemIDHigh, "CBaseAttributableItem->m_iItemIDHigh", int &);
  NETVAR(GetAccountID, "CBaseAttributableItem->m_iAccountID", int &);
  NETVAR(GetEntityQuality, "CBaseAttributableItem->m_iEntityQuality", int &);
  NETVAR(GetCustomName, "CBaseAttributableItem->m_szCustomName", const char *);
  NETVAR(GetFallbackPaintKit, "CBaseAttributableItem->m_nFallbackPaintKit", int &);
  NETVAR(GetFallbackSeed, "CBaseAttributableItem->m_nFallbackSeed", int &);
  NETVAR(GetFallbackWear, "CBaseAttributableItem->m_flFallbackWear", float &);
  NETVAR(GetFallbackStatTrak, "CBaseAttributableItem->m_nFallbackStatTrak", int &);

  bool IsPistol() {
    switch (GetItemDefinitionIndex()) {
      case WEAPON_DEAGLE:
      case WEAPON_ELITE:
      case WEAPON_FIVESEVEN:
      case WEAPON_GLOCK:
      case WEAPON_TEC9:
      case WEAPON_HKP2000:
      case WEAPON_USP_SILENCER:
      case WEAPON_P250:
      case WEAPON_CZ75A:
      case WEAPON_REVOLVER:
        return true;
      default:
        return false;
    }
  }

  bool IsAutomatic() {
    switch (GetItemDefinitionIndex()) {
      case WEAPON_AK47:
      case WEAPON_AUG:
      case WEAPON_FAMAS:
      case WEAPON_GALILAR:
      case WEAPON_M249:
      case WEAPON_M4A1:
      case WEAPON_M4A1_SILENCER:
      case WEAPON_MAC10:
      case WEAPON_P90:
      case WEAPON_MP5SD:
      case WEAPON_UMP45:
      case WEAPON_BIZON:
      case WEAPON_NEGEV:
      case WEAPON_MP7:
      case WEAPON_MP9:
      case WEAPON_SG556:
        return true;
      default:
        return false;
    }
  }

  bool IsKnife() {
    switch (GetItemDefinitionIndex()) {
      case WEAPON_BAYONET:
      case WEAPON_KNIFE:
      case WEAPON_KNIFE_T:
      case WEAPON_KNIFE_GUT:
      case WEAPON_KNIFEGG:
      case WEAPON_KNIFE_CSS:
      case WEAPON_KNIFE_CORD:
      case WEAPON_KNIFE_FLIP:
      case WEAPON_KNIFE_PUSH:
      case WEAPON_KNIFE_CANIS:
      case WEAPON_KNIFE_GHOST:
      case WEAPON_KNIFE_URSUS:
      case WEAPON_KNIFE_OUTDOOR:
      case WEAPON_KNIFE_FALCHION:
      case WEAPON_KNIFE_KARAMBIT:
      case WEAPON_KNIFE_SKELETON:
      case WEAPON_KNIFE_STILETTO:
      case WEAPON_KNIFE_TACTICAL:
      case WEAPON_KNIFE_BUTTERFLY:
      case WEAPON_KNIFE_WIDOWMAKER:
      case WEAPON_KNIFE_M9_BAYONET:
      case WEAPON_KNIFE_SURVIVAL_BOWIE:
      case WEAPON_KNIFE_GYPSY_JACKKNIFE:
        return true;
      default:
        return false;
    }
  }

  bool IsGrenade() {
    switch (GetItemDefinitionIndex()) {
      case WEAPON_SMOKEGRENADE:
      case WEAPON_HEGRENADE:
      case WEAPON_INCGRENADE:
      case WEAPON_FLASHBANG:
      case WEAPON_MOLOTOV:
      case WEAPON_DECOY:
        return true;
      default:
        return false;
    }
  }

  bool IsBomb() {
    return GetItemDefinitionIndex() == WEAPON_C4;
  }

  bool CanScope() {
    switch (GetItemDefinitionIndex()) {
      case WEAPON_AUG:
      case WEAPON_AWP:
      case WEAPON_G3SG1:
      case WEAPON_SCAR20:
      case WEAPON_SG556:
      case WEAPON_SSG08:
        return true;
      default:
        return false;
    }
  }
};

class CBaseViewModel : public CEntity {
 public:
  NETVAR(GetWeaponHandle, "CBaseViewModel->m_hWeapon", CBaseHandle &);
  NETVAR(GetOwnerHandle, "CBaseViewModel->m_hOwner", CBaseHandle &);
  NETVAR(GetModelIndex, "CBaseViewModel->m_nModelIndex", int &);
};

class CBaseWeaponWorldModel : public CEntity {
 public:
};

class CWeaponInfo;
class CBaseCombatWeapon : public CBaseAttributableItem {
 public:
  NETVAR(GetNextPrimaryAttack, "CBaseCombatWeapon->m_flNextPrimaryAttack", float &);
  NETVAR(GetOwner, "CBaseCombatWeapon->m_hOwner", CBaseHandle &);
  NETVAR(GetAmmo, "CBaseCombatWeapon->m_iClip1", unsigned int &);
  NETVAR(GetWeaponWorldModelHandle, "CBaseCombatWeapon->m_hWeaponWorldModel", CBaseHandle &);

  CWeaponInfo* GetWeaponInfo() {
    return memory::CallVFunc<CWeaponInfo *>(this, 524);
	}

  float GetInaccuracy() {
    return memory::CallVFunc<float>(this, 551);
  }

  float GetSpread() {
    return memory::CallVFunc<float>(this, 552);
  }
};

class CWeaponC4 : public CBaseCombatWeapon {
 public:
  NETVAR(IsStartedArming, "CWeaponC4->m_bStartedArming", bool &);
};

class CChicken : public CEntity {
 public:
  NETVAR(ShouldGlow, "CDynamicProp->m_bShouldGlow", bool &);
};

class CHudTexture;
class CFileWeaponInfo {
 public:
  // Each game can override this to get whatever values it wants from the script.
  //virtual void Parse(KeyValues *pKeyValuesData, const char *szWeaponName);

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
  char shootSounds[CEntity::WEAPONSOUND_SIZE][kMaxWeaponString];

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

class CWeaponInfo : public CFileWeaponInfo {
 public:
   CEntity::WeaponType GetWeaponType() {
    return *reinterpret_cast<CEntity::WeaponType *>((uintptr_t(this) + 0x864));
  }

  bool IsFullAuto() {
    return *reinterpret_cast<bool *>((uintptr_t(this) + 0x870));
  }

  float GetWeaponArmorRatio() {
    return *reinterpret_cast<float *>((uintptr_t(this) + 0x87c));
  }

  float GetMaxPlayerSpeed() {
    return *reinterpret_cast<float *>((uintptr_t(this) + 0x880));
  }

  float GetMaxPlayerSpeedAlt() {
    return *reinterpret_cast<float *>((uintptr_t(this) + 0x884));
  }

  float GetPenetration() {
    return *reinterpret_cast<float *>((uintptr_t(this) + 0x890));
  }

  int GetDamage() {
    return *reinterpret_cast<int *>((uintptr_t(this) + 0x894));
  }

  float GetRange() {
    return *reinterpret_cast<float *>((uintptr_t(this) + 0x898));
  }

  float GetRangeModifier() {
    return *reinterpret_cast<float *>((uintptr_t(this) + 0x89c));
  }

  float GetSpread() {
    return *reinterpret_cast<float *>((uintptr_t(this) + 0xa4c));
  }

  int GetZoomLevels() {
    return *reinterpret_cast<int *>((uintptr_t(this) + 0xee0));
  }
};

#endif  // HAVOC_C_ENTITY_H_
