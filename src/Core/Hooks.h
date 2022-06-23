// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_HOOKS_H_
#define HAVOC_HOOKS_H_

#include <cstdint>

class CBasePlayer;
class CLocalPlayer;
class CVector;
class IClientMode;
class IBaseClientDLL;
class CUserCmd;
class CRecvProxyData;
class CBaseViewModel;
enum class GlowRenderStyle;
enum class ClientFrameStage;

namespace hooks {

// Hook/Replace the instruction at `func_addr` to `jmp user_func_addr`.
bool Hook(std::uintptr_t func_addr, std::uintptr_t user_func_addr);

bool CreateMove(IClientMode *thisptr, float frameTime, CUserCmd *cmd);
void FrameStageNotify(IBaseClientDLL *thisptr, ClientFrameStage stage);
void SetViewModelSequence(CRecvProxyData *data, CBaseViewModel *viewModel, void *dummy);
bool GlowEffectSpectator(CBasePlayer *player, CLocalPlayer *localPlayer, GlowRenderStyle &glowStyle,
                         CVector &glowColor, float &alphaStart, float &alpha, float &timeStart,
                         float &timeTarget, bool &animate);

using CreateMoveFn = bool (*)(IClientMode *, float, CUserCmd *);
inline CreateMoveFn originalCreateMove = nullptr;

using FrameStageNotifyFn = void (*)(IBaseClientDLL *, ClientFrameStage);
inline FrameStageNotifyFn originalFrameStageNotify = nullptr;

using SetViewModelSequenceFn = void (*)(CRecvProxyData *data, CBaseViewModel *, void *);
inline SetViewModelSequenceFn originalSetViewModelSequence = nullptr;

}  // namespace hooks

#endif  // HAVOC_HOOKS_H_
