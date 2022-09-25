// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_HIT_GROUP_H_
#define HAVOC_HIT_GROUP_H_

enum class Bone : int {
  INVALID = -1,
  PELVIS = 0,
  LEAN_ROOT,
  CAM_DRIVER,
  HIP,
  LOWER_SPINAL_COLUMN,
  MIDDLE_SPINAL_COLUMN,
  UPPER_SPINAL_COLUMN,
  NECK,
  HEAD,
  SIZE
};

enum class Hitbox : int {
  HEAD = 0,
  NECK,
  PELVIS,
  SPINE,
  LEGS,
  ARMS,
};

enum class HitGroup : int {
  GENERIC = 0,
  HEAD,
  CHEST,
  STOMACH,
  LEFTARM,
  RIGHTARM,
  LEFTLEG,
  RIGHTLEG,
  GEAR
};

#endif  // HAVOC_HIT_GROUP_H_
