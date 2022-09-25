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

struct CSurfaceAudioParams {
  float reflectivity;
  float hardnessFactor;
  float roughnessFactor;
  float roughThreshold;
  float hardThreshold;
  float hardVelocityThreshold;
};

struct CSurfaceSoundNames {
  unsigned short walkStepLeft;
  unsigned short walkStepRight;
  unsigned short runStepLeft;
  unsigned short runStepRight;
  unsigned short impactSoft;
  unsigned short impactHard;
  unsigned short scrapeSmooth;
  unsigned short scrapeRough;
  unsigned short bulletImpact;
  unsigned short rolling;
  unsigned short breakSound;
  unsigned short strainSound;
};

struct CSurfaceSoundHandles {
  short walkStepLeft;
  short walkStepRight;
  short runStepLeft;
  short runStepRight;
  short impactSoft;
  short impactHard;
  short scrapeSmooth;
  short scrapeRough;
  short bulletImpact;
  short rolling;
  short breakSound;
  short strainSound;
};

struct CSurfaceGameProps {
  float maxSpeedFactor;
  float jumpFactor;
  char pad00[0x4];
  float flPenetrationModifier;
  float flDamageModifier;
  unsigned short material;
  char pad01[0x3];
};

struct CSurfaceData {
  CSurfacePhysicsParams physics;
  CSurfaceAudioParams audio;
  CSurfaceSoundNames sounds;
  CSurfaceGameProps game;
  CSurfaceSoundHandles soundhandles;
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
