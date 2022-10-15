// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#include "GUI.h"

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl2.h>
#include <imgui/backends/imgui_impl_sdl.h>

#include "Settings.h"
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
    if (ImGui::CollapsingHeader("Global")) {
      ImGui::Checkbox("Set local viewangles##global", &hooks::shouldSetLocal);
    }
    if (ImGui::CollapsingHeader("Aimbot")) {
      ImGui::Checkbox("Enabled##aimbot", &settings::aimbot::isEnabled);
      ImGui::BeginDisabled(!settings::aimbot::isEnabled);
      ImGui::Checkbox("Rage##aimbot", &settings::aimbot::shouldRage);
      ImGui::Checkbox("Automatic fire##aimbot", &settings::aimbot::shouldAutoFire);
      ImGui::SliderInt("Fire delay (ms)##aimbot", &settings::aimbot::fireDelayMs, 0, 1000);
      ImGui::Checkbox("Automatic scope##aimbot", &settings::aimbot::shouldAutoScope);
      ImGui::Checkbox("Autowall##aimbot", &settings::autowall::isEnabled);
      ImGui::Checkbox("Shoot teammates##aimbot", &settings::aimbot::shouldShootTeammates);
      ImGui::Checkbox("AWP and SSG only##aimbot", &settings::aimbot::shouldOnlyAllowAWPandSSG);
      ImGui::SliderFloat("Hit chance##aimbot", &settings::aimbot::hitChance, 0.0f, 1.0f);
      ImGui::EndDisabled();
    }
    if (ImGui::CollapsingHeader("AntiAim")) {
      ImGui::Checkbox("Enabled##antiaim", &settings::antiaim::isEnabled);
      ImGui::BeginDisabled(!settings::antiaim::isEnabled);
      ImGui::Checkbox("Yaw##antiaim", &settings::antiaim::isYawEnabled);
      ImGui::Checkbox("Pitch##antiaim", &settings::antiaim::isPitchEnabled);
      ImGui::Checkbox("Head edge##antiaim", &settings::antiaim::isHeadEdgeEnabled);
      ImGui::Checkbox("LBT breaker##antiaim", &settings::antiaim::isLBYBreakerEnabled);
      ImGui::SliderInt("Real Yaw Type##antiaim", &(int &) settings::antiaim::realYawType, 0, 4);
      ImGui::SliderInt("Fake Yaw Type##antiaim", &(int &) settings::antiaim::fakeYawType, 0, 4);
      ImGui::SliderInt("Pitch Type##antiaim", &(int &) settings::antiaim::pitchType, 0, 3);
      ImGui::EndDisabled();
    }
    if (ImGui::CollapsingHeader("Chams")) {
      ImGui::Checkbox("Enabled##chams", &settings::chams::isEnabled);
    }
    if (ImGui::CollapsingHeader("Glow")) {
      ImGui::Checkbox("Enabled##glow", &settings::glow::isEnabled);
      ImGui::BeginDisabled(!settings::glow::isEnabled);
      ImGui::Checkbox("Show teammates##glow", &settings::glow::shouldShowTeammates);
      ImGui::SliderFloat("Alpha##glow", &settings::glow::userAlpha, 0.0f, 1.0f);
      ImGui::EndDisabled();
    }
    if (ImGui::CollapsingHeader("Bunny Hop")) {
      ImGui::Checkbox("Enabled##bhop", &settings::bhop::isEnabled);
      ImGui::BeginDisabled(!settings::bhop::isEnabled);
      ImGui::Checkbox("Auto strafe##bhop", &settings::autostrafe::isEnabled);
      ImGui::SliderInt("auto strafe type##bhop", &(int &) settings::autostrafe::autoStrafeType, 0, 4);
      ImGui::EndDisabled();
    }
    if (ImGui::CollapsingHeader("Skins")) {
      ImGui::Text("Knife model");
      if (ImGui::BeginCombo("##skins", hacks::skins::kKnifeModels[settings::skins::knifeIdx].name)) {
        for (int i = 0; i < hacks::skins::kNumKnifeModels; i++) {
          const bool isSelected = i == settings::skins::knifeIdx;
          if (ImGui::Selectable(hacks::skins::kKnifeModels[i].name, isSelected)) {
            settings::skins::knifeIdx = i;
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
