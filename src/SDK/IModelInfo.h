// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_I_MODEL_INFO_H_
#define HAVOC_I_MODEL_INFO_H_

#include "Core/Memory.h"
#include "SDK/CVector.h"

enum EHitboxIndex : int {
  HITBOX_INVALID = -1,
  HITBOX_HEAD,
  HITBOX_NECK,
  HITBOX_PELVIS,
  HITBOX_STOMACH,
  HITBOX_THORAX,
  HITBOX_CHEST,
  HITBOX_UPPER_CHEST,
  HITBOX_RIGHT_THIGH,
  HITBOX_LEFT_THIGH,
  HITBOX_RIGHT_CALF,
  HITBOX_LEFT_CALF,
  HITBOX_RIGHT_FOOT,
  HITBOX_LEFT_FOOT,
  HITBOX_RIGHT_HAND,
  HITBOX_LEFT_HAND,
  HITBOX_RIGHT_UPPER_ARM,
  HITBOX_RIGHT_FOREARM,
  HITBOX_LEFT_UPPER_ARM,
  HITBOX_LEFT_FOREARM,
  HITBOX_MAX
};

class CStudioBone {};

class CStudioBoundingBox {
 public:
  int bone;
  int group;
  CVector bbMin;
  CVector bbMax;
  int hitboxNameIndex;
  CVector offsetRotation;
  float capsuleRadius;
  int unused[4];
};

class CStudioHitboxSet {
 public:
  int nameIndex;
  int numHitboxes;
  int hitboxIndex;

  inline const char *GetName() const {
    return reinterpret_cast<char *>(uintptr_t(this) + nameIndex);
  }

  inline CStudioBoundingBox *GetHitbox(int index) {
    if (index < 0 || index >= numHitboxes) return nullptr;

    return reinterpret_cast<CStudioBoundingBox *>(reinterpret_cast<uint8_t *>(this) +
                                                  hitboxIndex) +
        index;
  }
};

class CStudioHdr {
 public:
  inline CStudioBone *GetBone(int index) {
    if (index < 0 || index >= numBones) return nullptr;

    return reinterpret_cast<CStudioBone *>(uintptr_t(this) + boneIndex) + index;
  }

  inline CStudioHitboxSet *GetHitboxSet(int index) {
    if (index < 0 || index >= numHitboxSets) return nullptr;

    return reinterpret_cast<CStudioHitboxSet *>(uintptr_t(this) + hitboxSetIndex) + index;
  }

  int id;
  int version;
  int checksum;
  char name[64];
  int length;

  CVector eyePosition;
  CVector illumPosition;
  CVector hullMin;
  CVector hullMax;
  CVector bbMin;
  CVector bbMax;

  int flags;
  int numBones;
  int boneIndex;
  int numBoneControllers;
  int boneControllerIndex;
  int numHitboxSets;
  int hitboxSetIndex;
};

class CModel {
 public:
  int32_t handle;
  char name[260];
  int32_t loadFlags;
  int32_t serverCount;
  int32_t type;
  int32_t flags;
  CVector mins;
  CVector maxs;
  float radius;
};

class IModelInfo {
 public:
  enum ERenderFlags : uint32_t {
    RENDER_FLAGS_DISABLE_RENDERING = 0x01,
    RENDER_FLAGS_HASCHANGED = 0x02,
    RENDER_FLAGS_ALTERNATE_SORTING = 0x04,
    RENDER_FLAGS_RENDER_WITH_VIEWMODELS = 0x08,
    RENDER_FLAGS_BLOAT_BOUNDS = 0x10,
    RENDER_FLAGS_BOUNDS_VALID = 0x20,
    RENDER_FLAGS_BOUNDS_ALWAYS_RECOMPUTE = 0x40,
    RENDER_FLAGS_IS_SPRITE = 0x80,
    RENDER_FLAGS_FORCE_OPAQUE_PASS = 0x100,
  };

  constexpr CModel *GetModel(int modelIndex) {
    return memory::CallVFunc<CModel *>(this, 2, modelIndex);
  }

  constexpr int GetModelIndex(const char *name) {
    return memory::CallVFunc<int>(this, 3, name);
  }

  constexpr const char *GetModelName(const CModel *model) {
    return memory::CallVFunc<const char *>(this, 4, model);
  }

  constexpr CStudioHdr *GetStudioModel(const CModel *model) {
    return memory::CallVFunc<CStudioHdr *>(this, 31, model);
  }
};

#endif  // HAVOC_I_MODEL_INFO_H_
