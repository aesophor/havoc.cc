// Copyright (c) 2023 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_I_GAME_MOVEMENT_H_
#define HAVOC_I_GAME_MOVEMENT_H_

#include "Core/Memory.h"
#include "SDK/CVector.h"

// https://github.com/pwnedboi/csgobase/blob/master/SDK/IGameMovement.h
struct CMoveData {
  bool firstRunOfFunctions : 1;
  bool gameCodeMovedPlayer : 1;
  int playerHandle;      // edict index on server, client entity handle on client
  int impulseCommand;    // Impulse command issued.
  CVector viewAngles;     // Command view angles (local space)
  CVector absViewAngles;  // Command view angles (world space)
  int buttons;           // Attack buttons.
  int oldButtons;        // From host_client->oldbuttons;
  float forwardMove;
  float sideMove;
  float upMove;
  float maxSpeed;
  float clientMaxSpeed;

  // Variables from the player edict (sv_player) or entvars on the client.
  // These are copied in here before calling and copied out after calling.
  CVector velocity;  // edict::velocity  // Current movement direction.
  CVector angles;    // edict::angles
  CVector oldAngles;

  // Output only
  float outStepHeight;  // how much you climbed this move
  CVector outWishVel;    // This is where you tried
  CVector outJumpVel;    // This is your jump velocity

  // Movement constraints (radius 0 means no constraint)
  CVector constraintCenter;
  float constraintRadius;
  float constraintWidth;
  float constraintSpeedFactor;

  float unused[5];

  CVector absOrigin;  // edict::origin
};

class CBaseEntity;

class IGameMovement {
 public:
  void ProcessMovement(CBaseEntity* player, CMoveData* move) {
    memory::CallVFunc<void>(this, 2, player, move);
  }

  void StartTrackPredictionErrors(CBaseEntity* player) {
    memory::CallVFunc<void>(this, 4, player);
  }

  void FinishTrackPredictionErrors(CBaseEntity* player) {
    memory::CallVFunc<void>(this, 5, player);
  }
};

class IMoveHelper {
 public:
  void SetHost(CBaseEntity* host) {
    memory::CallVFunc<void>(this, 1, host);
  }
};

#endif  // HAVOC_I_GAME_MOVEMENT_H_
