// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#include "Hooks.h"

#include <mach/mach.h>
#include <sys/mman.h>

#include <cstring>

#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <imgui/backends/imgui_impl_sdl.h>

#include "Core/Interfaces.h"
#include "Hacks/Aimbot.h"
#include "Hacks/AntiAim.h"
#include "Hacks/AutoStrafe.h"
#include "Hacks/Bhop.h"
#include "Hacks/Chams.h"
#include "Hacks/Glow.h"
#include "Hacks/Skins.h"
#include "GUI/GUI.h"
#include "SDK/CEntity.h"
#include "SDK/CRecvTable.h"
#include "SDK/CUserCmd.h"
#include "SDK/IBaseClientDLL.h"
#include "SDK/IClientMode.h"

namespace hooks {

bool Hook(uintptr_t func_addr, uintptr_t user_func_addr) {
  constexpr mach_msg_type_number_t kSizeOfJmp = (sizeof(uintptr_t) * 2);

  void *func = reinterpret_cast<void *>(func_addr);
  void *page = reinterpret_cast<void *>(func_addr & ~vm_page_mask);

  uint8_t opcodes[kSizeOfJmp] = {0xff, 0x25};
  *((int *) &opcodes[2]) = 0;
  *((uintptr_t *) &opcodes[6]) = user_func_addr;

  // Enable write permission on the __TEXT segment.
  if (mprotect(page, vm_page_size, PROT_READ | PROT_WRITE | PROT_EXEC)) {
    return false;
  }

  // Rewrite the first instruction to `jmp user_func_addr`.
  memcpy(func, opcodes, kSizeOfJmp);

  // Unset write permission.
  return mprotect(page, vm_page_size, PROT_READ | PROT_EXEC) == 0;
}

bool CreateMove(IClientMode *thisptr, float frameTime, CUserCmd *cmd) {
  // Make sure this function is being called from CInput::CreateMove().
  if (!cmd || !cmd->commandNumber) {
    return originalCreateMove(interfaces::clientMode, frameTime, cmd);
  }

  auto localPlayer = CLocalPlayer::The();

  if (localPlayer && localPlayer->IsAlive()) {
    hacks::bhop::Run(cmd);
    hacks::aimbot::Run(cmd);
    hacks::antiaim::Run(cmd);
    hacks::autostrafe::Run(cmd);
  }

  if (createMoveShouldSendPacket) {
    createMoveLastTickViewAngles = cmd->viewAngles;
  }

  if (originalCreateMove(interfaces::clientMode, frameTime, cmd)) {
    if (shouldSetLocal) {
      interfaces::engine->SetViewAngles(cmd->viewAngles);
    }
  }

  return false;
}

void FrameStageNotify(IBaseClientDLL *thisptr, ClientFrameStage stage) {
  if (stage == ClientFrameStage::NET_UPDATE_POSTDATAUPDATE_START) {
    hacks::skins::Run();
  }

  originalFrameStageNotify(interfaces::client, stage);
}

void SetViewModelSequence(CRecvProxyData *data, CBaseViewModel *viewModel, void *dummy) {
  if (viewModel) {
    hacks::skins::FixAnimations(data, viewModel, dummy);
  }

  originalSetViewModelSequence(data, viewModel, dummy);
}

bool GlowEffectSpectator(CBasePlayer *player, CLocalPlayer *localPlayer, GlowRenderStyle &glowStyle,
                         CVector &glowColor, float &alphaStart, float &alpha, float &timeStart,
                         float &timeTarget, bool &animate) {
  return hacks::glow::GlowEffectSpectator(player, localPlayer, glowStyle, glowColor,
                                          alphaStart, alpha, timeStart, timeTarget, animate);
}

void DrawModel(IStudioRender *thisptr, void *results, const CDrawModelInfo &info,
               CMatrix3x4 *bones, float *flexWeights, float *flexDelayedWeights,
               const CVector &modelOrigin, const int flags) {
  return hacks::chams::DrawModel(thisptr, results, info, bones,
                                 flexWeights, flexDelayedWeights, modelOrigin, flags);
}

void SwapWindow(SDL_Window *window) {
  gui::Render(window);

  originalSwapWindow(window);
  SDL_GL_MakeCurrent(window, gui::originalGlCtx);
}

int PollEvent(SDL_Event *event) {
  ImGui_ImplSDL2_ProcessEvent(event);

  return originalPollEvent(event);
}

}  // namespace hooks
