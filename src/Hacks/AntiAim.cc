// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#include "AntiAim.h"

#include <cmath>

#include "Core/Hooks.h"
#include "Core/Interfaces.h"
#include "Hacks/Aimbot.h"
#include "SDK/CEntity.h"
#include "SDK/IEngineTrace.h"
#include "Util/Math.h"
#include "Util/Time.h"

namespace hacks::antiaim {

namespace {

void CorrectMovement(const CVector &oldAngles, CUserCmd* cmd, float oldForward, float oldSidemove) {
  // side/forward move correction
  float deltaView;
  float f1;
  float f2;

  if (oldAngles.y < 0.f) {
    f1 = 360.f + oldAngles.y;
  } else {
    f1 = oldAngles.y;
  }

  if (cmd->viewAngles.y < 0.f) {
    f2 = 360.f + cmd->viewAngles.y;
  } else {
    f2 = cmd->viewAngles.y;
  }

  if (f2 < f1) {
    deltaView = std::abs(f2 - f1);
  } else {
    deltaView = 360.f - std::abs(f1 - f2);
  }

  deltaView = 360.f - deltaView;

  cmd->forwardMove
    = std::cos(Deg2Rad(deltaView)) * oldForward +
      std::cos(Deg2Rad(deltaView + 90.f)) * oldSidemove;

  cmd->sideMove
    = std::sin(Deg2Rad(deltaView)) * oldForward +
      std::sin(Deg2Rad(deltaView + 90.f)) * oldSidemove;
}

float Distance(CVector a, CVector b) {
  return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2) + std::pow(a.z - b.z, 2));
}

bool GetBestHeadAngle(CVector& angle) {
  auto localPlayer = CLocalPlayer::The();

  const CVector src = localPlayer->GetEyePosition();
  const float radius = hacks::antiaim::headEdgeDistance + 0.1f;
  const float step = std::numbers::pi_v<float> * 2.0 / 8;
  float closestDistance = 100.f;

  for (float i = 0; i < std::numbers::pi_v<float> * 2.0; i += step) {
    const CVector location{
        src.x + radius * std::cos(i),
        src.y + radius * std::sin(i),
        src.z
    };

    CTrace trace;
    interfaces::engineTrace->TraceRay(
        CRay{src, location},
        0x4600400b,
        CTraceFilter{localPlayer},
        trace
    );

    const float distance = Distance(src, trace.endPos);
    if (distance < closestDistance) {
      closestDistance = distance;
      angle.y = Rad2Deg(i);
    }
  }

  return closestDistance < hacks::antiaim::headEdgeDistance;
}

void DoAntiAimY(CBasePlayer *localPlayer, CVector &angle, bool shouldSend) {
  AntiAimTypeY type = shouldSend ? hacks::antiaim::yawTypeFake : hacks::antiaim::yawType;

  float maxDelta = GetMaxDelta(localPlayer->GetAnimState());
  static bool shouldFlip = false;

  switch (type) {
    case AntiAimTypeY::MAX_DELTA_LEFT:
      angle.y = hacks::antiaim::fakeAngle.y - maxDelta;
      break;
    case AntiAimTypeY::MAX_DELTA_RIGHT:
      angle.y = hacks::antiaim::fakeAngle.y + maxDelta;
      break;
    case AntiAimTypeY::MAX_DELTA_FLIPPER:
      shouldFlip = !shouldFlip;
      angle.y -= shouldFlip ? maxDelta : -maxDelta;
      break;
    case AntiAimTypeY::MAX_DELTA_LBY_AVOID:
      break;
    default:
      break;
  }

  if (shouldSend) {
    hacks::antiaim::fakeAngle.y = angle.y;
  } else {
    hacks::antiaim::realAngle.y = angle.y;
  }
}

void DoAntiAimX(CVector &angle) {
  static float dance = 0.f;

  switch (hacks::antiaim::pitchType) {
    case AntiAimTypeX::STATIC_UP:
      angle.x = -89.f;
      break;
    case AntiAimTypeX::STATIC_DOWN:
      angle.x = 89.f;
      break;
    case AntiAimTypeX::FRONT:
      angle.x = 0.f;
      break;
    case AntiAimTypeX::DANCE:
      dance += 45.f;
      if (dance > 100) {
        dance = 0.f;
      } else if (dance > 75.f) {
        angle.x = -89.f;
      } else if (dance < 75.f) {
        angle.x = 89.f;
      }
      break;
    default:
      break;
  }
}

}  // namespace

bool Init() {
  // In C_CSPlayer::Spawn, it references the offset of each player's animation state so it can do a reset.
  //
  // Sig:
  // __text:000000000063F4E0 55                     push    rbp
  // __text:000000000063F4E1 48 89 E5               mov     rbp, rsp
  // __text:000000000063F4E4 41 56                  push    r14
  // __text:000000000063F4E6 53                     push    rbx
  // __text:000000000063F4E7 48 83 EC 20            sub     rsp, 20h
  // __text:000000000063F4EB 48 89 FB               mov     rbx, rdi
  // __text:000000000063F4EE 48 8D 05 FB FB F5 00   lea     rax, off_159F0F0
  // __text:000000000063F4F5 48 8B 00               mov     rax, [rax]
  // __text:000000000063F4F8 F3 0F 10 40 10         movss   xmm0, dword ptr [rax+10h]
  // ...
  // __text:000000000063F513 48 8B BB 50 A2 00 00   mov     rdi, [rbx+0A250h] <---- 0xA250 is what we need.
  // __text:000000000063F51A 48 85 FF               test    rdi, rdi
  // __text:000000000063F51D 74 41                  jz      short loc_63F560
  // __text:000000000063F51F E8 AC 2B 06 00         call    CCSGOPlayerAnimState::Reset()
  constexpr std::string_view kSig = "\x55\x48\x89\xe5\x41\x56\x53\x48\x83\xec\x20\x48\x89"
                                    "\xfb\x48\x8d\x05????\x48\x8b?\xf3\x0f\x10\x40\x10";

  uintptr_t ccsPlayerSpawn = interfaces::clientDylib->ScanSignature(kSig);
  if (!ccsPlayerSpawn) {
    return false;
  }

  CBasePlayer::playerAnimStateOffset = *reinterpret_cast<unsigned int *>(ccsPlayerSpawn + 54);
  return true;
}

void Run(CUserCmd* cmd) {
  if (!hacks::antiaim::isEnabled) {
    return;
  }

  auto localPlayer = CLocalPlayer::The();
  CVector oldAngle = cmd->viewAngles;
  float oldForward = cmd->forwardMove;
  float oldSideMove = cmd->sideMove;

  hacks::antiaim::realAngle = hacks::antiaim::fakeAngle = hooks::createMoveLastTickViewAngles;

  CVector angle = cmd->viewAngles;

  auto activeWeapon
    = interfaces::entityList->GetEntityFromHandle<CBaseCombatWeapon>(
        localPlayer->GetActiveWeaponHandle());

  if (!activeWeapon || activeWeapon->IsKnife() || activeWeapon->IsGrenade()) {
    return;
  }

  if (cmd->buttons & CUserCmd::IN_USE ||
      cmd->buttons & CUserCmd::IN_ATTACK ||
      (cmd->buttons & CUserCmd::IN_SECOND_ATTACK && activeWeapon->IsRevolver())) {
    return;
  }

  if (localPlayer->GetMoveType() == CEntity::MOVETYPE_LADDER ||
      localPlayer->GetMoveType() == CEntity::MOVETYPE_NOCLIP) {
    return;
  }

  CVector edgeAngle = angle;
  bool edgingHead = hacks::antiaim::isHeadEdgeEnabled && GetBestHeadAngle(edgeAngle);

  static bool shouldSend = true;
  shouldSend = !shouldSend;

  bool shouldClamp = true;
  bool needToFlick = false;
  static bool lbyBreak = false;
  static uint64_t lastCheck = 0;

  const CVector velocity = localPlayer->GetVelocity();
  float vel2D = std::hypot(velocity.x, velocity.y);

  if (hacks::antiaim::isLBYBreakerEnabled) {
    if (vel2D >= 0.1f ||
        !(localPlayer->GetFlags() & CEntity::FL_ONGROUND) ||
        localPlayer->GetFlags() & CEntity::FL_FROZEN) {
      lbyBreak = false;
      lastCheck = GetCurrentTimestampMs();
    } else {
      if (!lbyBreak && GetCurrentTimestampMs() - lastCheck > 22) {
        angle.y -= hacks::antiaim::lbyBreakerOffset;
        lbyBreak = true;
        lastCheck = GetCurrentTimestampMs();
        needToFlick = true;
      } else if (lbyBreak && GetCurrentTimestampMs() - lastCheck > 110) {
        angle.y -= hacks::antiaim::lbyBreakerOffset;
        lbyBreak = true;
        lastCheck = GetCurrentTimestampMs();
        needToFlick = true;
      }
    }
  }

  if (hacks::antiaim::isYawEnabled && !needToFlick) {
    DoAntiAimY(localPlayer, angle, shouldSend);

    hooks::createMoveShouldSendPacket = shouldSend;
    if (hacks::antiaim::isHeadEdgeEnabled && edgingHead && !shouldSend) {
      angle.y = edgeAngle.y;
    }
  }

  if (hacks::antiaim::isPitchEnabled) {
    DoAntiAimX(angle);
  }

  if (shouldClamp) {
    angle = angle.NormalizeAngle().Clamp();
  }

  cmd->viewAngles = angle;

  CorrectMovement(oldAngle, cmd, oldForward, oldSideMove);
}

float GetMaxDelta(CCSGOAnimState *animState) {
  float speedFraction = std::max(0.f, std::min(animState->feetShuffleSpeed, 1.f));
  float speedFactor = std::max(0.f, std::min(1.f, animState->feetShuffleSpeed2));

  float unk1 = ((animState->runningAccelProgress * -0.30000001) - 0.19999999) * speedFraction;
  float unk2 = unk1 + 1.f;
  float delta;

  if (animState->duckProgress > 0) {
    unk2 += ((animState->duckProgress * speedFactor) * (0.5f - unk2));
  }

  delta = *(float*)((uintptr_t) animState + 0x3A4) * unk2;
  return delta - 0.5f;
}

}  // namespace hacks::antiaim
