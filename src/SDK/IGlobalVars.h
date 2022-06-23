// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_I_GLOBAL_VARS_H_
#define HAVOC_I_GLOBAL_VARS_H_

#include <cstdint>

class IGlobalVars {
 public:
  float realTime;
  int32_t frameCount;
  float absFrameTime;
  float absFrameStartTime;
  float currentTime;
  float frameTime;
  int32_t maxClients;
  int32_t tickCount;
  float intervalPerTick;
  float interpolationAmount;
  int32_t frameSimulationTicks;
  int32_t networkProtocol;
  void *saveData;
  bool client;
  bool remoteClient;
  int32_t timestampNetworkingBase;
  int32_t timestampRandomizeWindow;
};

#endif  // HAVOC_I_GLOBAL_VARS_H_
