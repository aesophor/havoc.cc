// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_C_ENTITY_H_
#define HAVOC_C_ENTITY_H_

#include <array>
#include <vector>

#include "Core/Memory.h"
#include "Core/NetVars.h"
#include "SDK/CClientClass.h"
#include "SDK/CCSWeaponInfo.h"
#include "SDK/CItemDefs.h"
#include "SDK/CMatrix.h"
#include "SDK/CVector.h"
#include "SDK/HitGroup.h"
#include "Util/TypeTraits.h"

using CBaseHandle = int;

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
    return *reinterpret_cast<int *>(uintptr_t(this) + 0x94);
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

  NETVAR(GetTeam, "CBaseEntity->m_iTeamNum", int &);
  NETVAR(GetWeaponHandles, "CBaseCombatCharacter->m_hMyWeapons", std::array<CBaseHandle, 64> &);
  NETVAR(GetActiveWeaponHandle, "CBaseCombatCharacter->m_hActiveWeapon", CBaseHandle &);
  NETVAR(GetVecOrigin, "CBaseEntity->m_vecOrigin", CVector &);
  NETVAR(GetCollideable, "CBaseEntity->m_Collision", ICollideable *);
  NETVAR(GetSpotted, "CBaseEntity->m_bSpotted", bool &);

  int GetId() {
    return *reinterpret_cast<int *>(uintptr_t(this) + 0x94);
  }

  bool IsDormant() {
    return *reinterpret_cast<bool *>(uintptr_t(this) + 0x125);
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
  NETVAR(GetItemDefinitionIndex, "CBaseAttributableItem->m_iItemDefinitionIndex", CItemDef &);
  NETVAR(GetItemIDHigh, "CBaseAttributableItem->m_iItemIDHigh", int &);
  NETVAR(GetAccountID, "CBaseAttributableItem->m_iAccountID", int &);
  NETVAR(GetEntityQuality, "CBaseAttributableItem->m_iEntityQuality", int &);
  NETVAR(GetCustomName, "CBaseAttributableItem->m_szCustomName", const char *);
  NETVAR(GetFallbackPaintKit, "CBaseAttributableItem->m_nFallbackPaintKit", int &);
  NETVAR(GetFallbackSeed, "CBaseAttributableItem->m_nFallbackSeed", int &);
  NETVAR(GetFallbackWear, "CBaseAttributableItem->m_flFallbackWear", float &);
  NETVAR(GetFallbackStatTrak, "CBaseAttributableItem->m_nFallbackStatTrak", int &);

  bool IsPistol() { return ::IsPistol(GetItemDefinitionIndex()); }
  bool IsAutomatic() { return ::IsAutomatic(GetItemDefinitionIndex()); }
  bool IsKnife() { return ::IsKnife(GetItemDefinitionIndex()); }
  bool IsGrenade() { return ::IsGrenade(GetItemDefinitionIndex()); }
  bool IsBomb() { return ::IsBomb(GetItemDefinitionIndex()); }
  bool CanScope() { return ::CanScope(GetItemDefinitionIndex()); }
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

class CBaseCombatWeapon : public CBaseAttributableItem {
 public:
  NETVAR(GetNextPrimaryAttack, "CBaseCombatWeapon->m_flNextPrimaryAttack", float &);
  NETVAR(GetOwner, "CBaseCombatWeapon->m_hOwner", CBaseHandle &);
  NETVAR(GetAmmo, "CBaseCombatWeapon->m_iClip1", unsigned int &);
  NETVAR(GetWeaponWorldModelHandle, "CBaseCombatWeapon->m_hWeaponWorldModel", CBaseHandle &);

  // https://github.com/LWSS/Fuzion/blob/master/src/SDK/IClientEntity.h#L738
  CCSWeaponInfo *GetCSWpnData() {
    return memory::CallVFunc<CCSWeaponInfo *>(this, 529);
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

#endif  // HAVOC_C_ENTITY_H_
