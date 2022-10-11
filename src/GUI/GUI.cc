// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#include "GUI.h"

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl2.h>
#include <imgui/backends/imgui_impl_sdl.h>

#include "Core/Hooks.h"
#include "Core/Interfaces.h"
#include "Hacks/Aimbot.h"
#include "Hacks/AntiAim.h"
#include "Hacks/AutoStrafe.h"
#include "Hacks/AutoWall.h"
#include "Hacks/Bhop.h"
#include "Hacks/Chams.h"
#include "Hacks/Glow.h"
#include "Hacks/Skins.h"

namespace {
SDL_GLContext havocGlCtx = nullptr;
}  // namespace

namespace gui {

inline uintptr_t *GetAbsAddr(uintptr_t base, uintptr_t memoryPtr) {
  uintptr_t sigAddr = memoryPtr + 0xF;
  uintptr_t fileOffset = sigAddr - base;
  uintptr_t offset = *reinterpret_cast<uint32_t *>(sigAddr);
  return reinterpret_cast<uintptr_t *>(base + (offset + fileOffset) + 0x4);
}

void Init() {
  uintptr_t swapWindowFn = (uintptr_t) dlsym(RTLD_DEFAULT, "SDL_GL_SwapWindow");
  uintptr_t *swapWindowPtr = GetAbsAddr(interfaces::sdlDylib->GetBase(), swapWindowFn);
  hooks::originalSwapWindow = reinterpret_cast<hooks::SwapWindowFn>(*swapWindowPtr);
  *swapWindowPtr = reinterpret_cast<uintptr_t>(&hooks::SwapWindow);

  uintptr_t pollEventFn = (uintptr_t) dlsym(RTLD_DEFAULT, "SDL_PollEvent");
  uintptr_t *pollEventPtr = GetAbsAddr(interfaces::sdlDylib->GetBase(), pollEventFn);
  hooks::originalPollEvent = reinterpret_cast<hooks::PollEventFn>(*pollEventPtr);
  *pollEventPtr = reinterpret_cast<uintptr_t>(&hooks::PollEvent);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();
}

void Render(SDL_Window *window) {
  originalGlCtx = SDL_GL_GetCurrentContext();

  if (!havocGlCtx) {
    havocGlCtx = SDL_GL_CreateContext(window);
    ImGui_ImplSDL2_InitForOpenGL(window, havocGlCtx);
    ImGui_ImplOpenGL2_Init();
  }

  SDL_GL_MakeCurrent(window, havocGlCtx);

  // Disable SDL Vsync
  SDL_GL_SetSwapInterval(0);

  ImGui_ImplOpenGL2_NewFrame();
  ImGui_ImplSDL2_NewFrame(window);
  ImGui::NewFrame();

  if (ImGui::Begin("havoc.cc")) {
    if (ImGui::CollapsingHeader("Aimbot")) {
      ImGui::Checkbox("Enabled##aimbot", &hacks::aimbot::isEnabled);
      ImGui::BeginDisabled(!hacks::aimbot::isEnabled);
      ImGui::Checkbox("Rage##aimbot", &hacks::aimbot::shouldRage);
      ImGui::Checkbox("Automatic fire##aimbot", &hacks::aimbot::shouldAutoFire);
      ImGui::SliderInt("Fire delay (ms)##aimbot", &hacks::aimbot::fireDelayMs, 0, 1000);
      ImGui::Checkbox("Automatic scope##aimbot", &hacks::aimbot::shouldAutoScope);
      ImGui::Checkbox("Autowall##aimbot", &hacks::autowall::isEnabled);
      ImGui::Checkbox("Shoot teammates##aimbot", &hacks::aimbot::shouldShootTeammates);
      ImGui::SliderFloat("Hit chance##aimbot", &hacks::aimbot::hitChance, 0.0f, 1.0f);
      ImGui::EndDisabled();
    }
    if (ImGui::CollapsingHeader("AntiAim")) {
      ImGui::Checkbox("Enabled##antiaim", &hacks::antiaim::isEnabled);
      ImGui::BeginDisabled(!hacks::antiaim::isEnabled);
      ImGui::Checkbox("Set local viewangles##antiaim", &hooks::shouldSetLocal);
      ImGui::Checkbox("Yaw##antiaim", &hacks::antiaim::isYawEnabled);
      ImGui::Checkbox("Pitch##antiaim", &hacks::antiaim::isPitchEnabled);
      ImGui::Checkbox("Head edge##antiaim", &hacks::antiaim::isHeadEdgeEnabled);
      ImGui::Checkbox("LBT breaker##antiaim", &hacks::antiaim::isLBYBreakerEnabled);
      ImGui::SliderInt("yawType##antiaim", &(int &) hacks::antiaim::yawType, 0, 4);
      ImGui::SliderInt("pitchType##antiaim", &(int &) hacks::antiaim::pitchType, 0, 3);
      ImGui::EndDisabled();
    }
    if (ImGui::CollapsingHeader("Chams")) {
      ImGui::Checkbox("Enabled##chams", &hacks::chams::isEnabled);
    }
    if (ImGui::CollapsingHeader("Glow")) {
      ImGui::Checkbox("Enabled##glow", &hacks::glow::isEnabled);
      ImGui::BeginDisabled(!hacks::glow::isEnabled);
      ImGui::Checkbox("Show teammates##glow", &hacks::glow::shouldShowTeammates);
      ImGui::SliderFloat("Alpha##glow", &hacks::glow::userAlpha, 0.0f, 1.0f);
      ImGui::EndDisabled();
    }
    if (ImGui::CollapsingHeader("Bunny Hop")) {
      ImGui::Checkbox("Enabled##bhop", &hacks::bhop::isEnabled);
      ImGui::BeginDisabled(!hacks::bhop::isEnabled);
      ImGui::Checkbox("Auto strafe##bhop", &hacks::autostrafe::isEnabled);
      ImGui::SliderInt("auto strafe type##bhop", &(int &) hacks::autostrafe::autoStrafeType, 0, 4);
      ImGui::EndDisabled();
    }
    if (ImGui::CollapsingHeader("Skins")) {
      ImGui::Text("Knife model");
      const auto &knifeModels = hacks::skins::knifeModels;

      if (ImGui::BeginCombo("##skins", knifeModels[hacks::skins::knifeIdx].name)) {
        for (int i = 0; i < hacks::skins::numKnifeModels; i++) {
          const bool isSelected = i == hacks::skins::knifeIdx;
          if (ImGui::Selectable(knifeModels[i].name, isSelected)) {
            hacks::skins::knifeIdx = i;
          }
          if (isSelected) {
            ImGui::SetItemDefaultFocus();
          }
        }
        ImGui::EndCombo();
      }
    }
  }

  ImGui::End();
  ImGui::Render();
  ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}

}  // namespace gui
