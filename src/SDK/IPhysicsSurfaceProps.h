// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_I_PHYSICS_SURFACE_PROPS_H_
#define HAVOC_I_PHYSICS_SURFACE_PROPS_H_

// https://github.com/AimTuxOfficial/AimTux/blob/master/src/SDK/IPhysicsSurfaceProps.h
struct CSurfacePhysicsParams {
  float friction;
  float elasticity;
  float density;
  float thickness;
  float dampening;
};

struct CSurfaceGameProps {
  float penetrationModifier;
  float damageModifier;
  unsigned short material;
};

struct CSurfaceData {
  CSurfacePhysicsParams physics;
  char __pad0[68];
  CSurfaceGameProps game;
};

class IPhysicsSurfaceProps {
 public:
  virtual ~IPhysicsSurfaceProps() = default;

  // Parses a text file containing surface prop keys
  virtual int ParseSurfaceData(const char *filename, const char *textfile) = 0;

  // The current number of entries in the database
  virtual int SurfacePropCount(void) const = 0;

  virtual int GetSurfaceIndex(const char *pSurfacePropName) const = 0;

  virtual void GetPhysicsProperties(int surfaceDataIndex, float *density,
                                    float *thickness, float *friction,
                                    float *elasticity) const = 0;

  virtual CSurfaceData *GetSurfaceData(int surfaceDataIndex) = 0;

  virtual const char *GetString(unsigned short stringTableIndex) const = 0;

  virtual const char *GetPropName(int surfaceDataIndex) const = 0;

  // sets the global index table for world materials
  // UNDONE: Make this per-CPhysCollide
  virtual void SetWorldMaterialIndexTable(int *mapArray, int mapSize) = 0;

  // NOTE: Same as GetPhysicsProperties, but maybe more convenient
  virtual void GetPhysicsParameters(int surfaceDataIndex,
                                    CSurfacePhysicsParams *paramsOut) const = 0;
};

#endif  // I_PHYSICS_SURFACE_PROPS_H_
