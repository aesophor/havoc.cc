// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_HOOKS_H_
#define HAVOC_HOOKS_H_

#include <cstdint>

#include "SDK/CVector.h"

class CBasePlayer;
class CBaseViewModel;
class CDrawModelInfo;
class CLocalPlayer;
class CMatrix3x4;
class CRecvProxyData;
class CUserCmd;
class CVector;
class IClientMode;
class IBaseClientDLL;
class IStudioRender;
enum class GlowRenderStyle;
enum class ClientFrameStage;

class SDL_Window;
union SDL_Event;

namespace hooks {

// Hook/Replace the instruction at `func_addr` to `jmp user_func_addr`.
bool Hook(std::uintptr_t func_addr, std::uintptr_t user_func_addr);

bool CreateMove(IClientMode *thisptr, float frameTime, CUserCmd *cmd);
void FrameStageNotify(IBaseClientDLL *thisptr, ClientFrameStage stage);
void SetViewModelSequence(CRecvProxyData *data, CBaseViewModel *viewModel, void *dummy);
bool GlowEffectSpectator(CBasePlayer *player, CLocalPlayer *localPlayer, GlowRenderStyle &glowStyle,
                         CVector &glowColor, float &alphaStart, float &alpha, float &timeStart,
                         float &timeTarget, bool &animate);
void DrawModel(IStudioRender *thisptr, void *results, const CDrawModelInfo &info,
               CMatrix3x4 *bones, float *flexWeights, float *flexDelayedWeights,
               const CVector &modelOrigin, const int flags);
void SwapWindow(SDL_Window *window);
int PollEvent(SDL_Event *event);

using CreateMoveFn = bool (*)(IClientMode *, float, CUserCmd *);
inline CreateMoveFn originalCreateMove = nullptr;

using FrameStageNotifyFn = void (*)(IBaseClientDLL *, ClientFrameStage);
inline FrameStageNotifyFn originalFrameStageNotify = nullptr;

using SetViewModelSequenceFn = void (*)(CRecvProxyData *data, CBaseViewModel *, void *);
inline SetViewModelSequenceFn originalSetViewModelSequence = nullptr;

using DrawModelFn = void (*)(IStudioRender *, void *, const CDrawModelInfo &,
                             CMatrix3x4 *, float *, float *, const CVector &, const int);
inline DrawModelFn originalDrawModel = nullptr;

using SwapWindowFn = void (*)(SDL_Window *window);
inline SwapWindowFn originalSwapWindow = nullptr;

using PollEventFn = int (*)(SDL_Event *event);
inline PollEventFn originalPollEvent = nullptr;

inline bool shouldSetLocal = true;
inline bool createMoveShouldSendPacket = false;
inline CVector createMoveLastTickViewAngles{};

}  // namespace hooks

#endif  // HAVOC_HOOKS_H_
