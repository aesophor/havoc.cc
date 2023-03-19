// Copyright (c) 2023 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_C_GLOBAL_VARS_H_
#define HAVOC_C_GLOBAL_VARS_H_

struct CGlobalVars {
  float realtime;
  int frameCount;
  float absoluteFrameTime;
  float absoluteFrameStartTimeStdDev;
  float curTime;
  float frameTime;
  int maxClients;
  int tickCount;
  float intervalPerTick;
  float interpolationAmount;
  int simTicksThisFrame;
  int networkProtocol;
  void* saveData;
  bool client;
  bool remoteClient;
  int timestampNetworkingBase;
  int timestampRandomizeWindow;
};

#endif  // HAVOC_C_GLOBAL_VARS_H_
