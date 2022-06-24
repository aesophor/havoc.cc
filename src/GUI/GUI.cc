// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#include "GUI.h"

#include <thread>

#include <imgui/backends/imgui_impl_opengl2.h>
#include <imgui/backends/imgui_impl_sdl.h>
#include <imgui/imgui.h>

#include "Core/Interfaces.h"
#include "Hacks/Aimbot.h"
#include "Hacks/Bhop.h"
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

  ImGuiIO &io = ImGui::GetIO();
  io.IniFilename = nullptr;

  ImGui::StyleColorsDark();
}

void SwapWindow(SDL_Window *window) {
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

  if (ImGui::Begin("Havoc")) {
    ImGui::Checkbox("##aimbot", &hacks::aimbot::isEnabled);
    ImGui::SameLine();
    ImGui::Text("Aimbot");
    ImGui::Separator();

    ImGui::Checkbox("##bhop", &hacks::bhop::isEnabled);
    ImGui::SameLine();
    ImGui::Text("Bunny Hop");
    ImGui::Separator();

    ImGui::Checkbox("##glow", &hacks::glow::isEnabled);
    ImGui::SameLine();
    ImGui::Text("Wallhack");

    ImGui::End();
  }

  ImGui::Render();
  ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

  std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

}  // namespace gui
