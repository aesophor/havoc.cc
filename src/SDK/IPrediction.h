// Copyright (c) 2023 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_I_PREDICTION_H_
#define HAVOC_I_PREDICTION_H_

#include "Core/Memory.h"

class CBaseEntity;
class CMoveData;
class CUserCmd;
class IMoveHelper;

class IPrediction {
 public:
  void RunCommand(CBaseEntity *player, CUserCmd *cmd, IMoveHelper *moveHelper) {
    memory::CallVFunc<void>(this, 20, player, cmd, moveHelper);
  }

  void SetupMove(CBaseEntity* player, CUserCmd* cmd, IMoveHelper* helper, CMoveData* move) {
    memory::CallVFunc<void>(this, 21, player, cmd, helper, move);
  }

  void FinishMove(CBaseEntity* player, CUserCmd* cmd, CMoveData* move) {
    memory::CallVFunc<void>(this, 22, player, cmd, move);
  }
};

#endif  // HAVOC_I_PREDICTION_H_
