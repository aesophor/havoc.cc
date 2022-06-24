// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#include <SDL2/SDL.h>

#include "Core/Memory.h"

Uint8 SDLCALL SDL_GameControllerGetButton(SDL_GameController *gameController,
                                          SDL_GameControllerButton button) {
  return memory::CallFunc<Uint8>("SDL_GameControllerGetButton", gameController, button);
}

Sint16 SDLCALL SDL_GameControllerGetAxis(SDL_GameController *gameController,
                                         SDL_GameControllerAxis axis) {
  return memory::CallFunc<Sint16>("SDL_GameControllerGetAxis", gameController, axis);
}

SDL_GameController *SDLCALL SDL_GameControllerOpen(int joystickIdx) {
  return memory::CallFunc<SDL_GameController *>("SDL_GameControllerOpen", joystickIdx);
}

void SDLCALL SDL_free(void *mem) {
  memory::CallFunc<void>("SDL_free", mem);
}

void SDLCALL SDL_GetWindowPosition(SDL_Window *window, int *x, int *y) {
  memory::CallFunc<void>("SDL_GetWindowPosition", window, x, y);
}

SDL_Window *SDLCALL SDL_GetKeyboardFocus() {
  return memory::CallFunc<SDL_Window *>("SDL_GetKeyboardFocus");
}

const char *SDLCALL SDL_GetCurrentVideoDriver() {
  return memory::CallFunc<const char *>("SDL_GetCurrentVideoDriver");
}

void SDLCALL SDL_GetWindowSize(SDL_Window *window, int *w, int *h) {
  memory::CallFunc<void>("SDL_GetWindowSize", window, w, h);
}

void SDLCALL SDL_WarpMouseInWindow(SDL_Window *window, int x, int y) {
  memory::CallFunc<void>("SDL_WarpMouseInWindow", window, x, y);
}

char *SDLCALL SDL_GetClipboardText() {
  return memory::CallFunc<char *>("SDL_GetClipboardText");
}

void SDLCALL SDL_GL_GetDrawableSize(SDL_Window *window, int *w, int *h) {
  memory::CallFunc<void>("SDL_GL_GetDrawableSize", window, w, h);
}

Uint32 SDLCALL SDL_GetTicks() {
  return memory::CallFunc<Uint32>("SDL_GetTicks");
}

Uint32 SDLCALL SDL_GetWindowFlags(SDL_Window *window) {
  return memory::CallFunc<Uint32>("SDL_GetWindowFlags", window);
}

int SDLCALL SDL_ShowCursor(int toggle) {
  return memory::CallFunc<int>("SDL_ShowCursor", toggle);
}

int SDLCALL SDL_SetClipboardText(const char *text) {
  return memory::CallFunc<int>("SDL_SetClipboardText", text);
}

Uint32 SDLCALL SDL_GetMouseState(int *x, int *y) {
  return memory::CallFunc<Uint32>("SDL_GetMouseState", x, y);
}

Uint32 SDLCALL SDL_GetGlobalMouseState(int *x, int *y) {
  return memory::CallFunc<Uint32>("SDL_GetGlobalMouseState", x, y);
}

SDL_Keymod SDLCALL SDL_GetModState() {
  return memory::CallFunc<SDL_Keymod>("SDL_GetModState");
}

SDL_GLContext SDLCALL SDL_GL_GetCurrentContext() {
  return memory::CallFunc<SDL_GLContext>("SDL_GL_GetCurrentContext");
}

SDL_GLContext SDLCALL SDL_GL_CreateContext(SDL_Window *window) {
  return memory::CallFunc<SDL_GLContext>("SDL_GL_CreateContext", window);
}

int SDLCALL SDL_GL_MakeCurrent(SDL_Window *window, SDL_GLContext context) {
  return memory::CallFunc<int>("SDL_GL_MakeCurrent", window, context);
}

Uint64 SDLCALL SDL_GetPerformanceFrequency() {
  return memory::CallFunc<Uint64>("SDL_GetPerformanceFrequency");
}

Uint64 SDLCALL SDL_GetPerformanceCounter() {
  return memory::CallFunc<Uint64>("SDL_GetPerformanceCounter");
}

int SDLCALL SDL_CaptureMouse(SDL_bool enabled) {
  return memory::CallFunc<int>("SDL_CaptureMouse", enabled);
}

void SDLCALL SDL_SetCursor(SDL_Cursor *cursor) {
  memory::CallFunc<void>("SDL_SetCursor", cursor);
}

void SDLCALL SDL_FreeCursor(SDL_Cursor *cursor) {
  memory::CallFunc<void>("SDL_FreeCursor", cursor);
}

SDL_Cursor *SDLCALL SDL_CreateSystemCursor(SDL_SystemCursor id) {
  return memory::CallFunc<SDL_Cursor *>("SDL_CreateSystemCursor", id);
}

int SDLCALL SDL_GL_SetAttribute(SDL_GLattr attr, int value) {
  return memory::CallFunc<int>("SDL_GL_SetAttribute", attr, value);
}

int SDLCALL SDL_SetRelativeMouseMode(SDL_bool enabled) {
  return memory::CallFunc<int>("SDL_SetRelativeMouseMode", enabled);
}

const Uint8 *SDLCALL SDL_GetKeyboardState(int *numkeys) {
  return memory::CallFunc<const Uint8 *>("SDL_GetKeyboardState", numkeys);
}

SDL_bool SDLCALL SDL_SetHint(const char *name, const char *value) {
  return memory::CallFunc<SDL_bool>("SDL_SetHint", name, value);
}

int SDLCALL SDL_Init(Uint32 flags) {
  return memory::CallFunc<int>("SDL_Init", flags);
}

int SDLCALL SDL_GL_SetSwapInterval(int interval) {
  return memory::CallFunc<int>("SDL_GL_SetSwapInterval", interval);
}

int SDL_GetRendererOutputSize(SDL_Renderer *renderer, int *w, int *h) {
  return memory::CallFunc<int>("SDL_GetRendererOutputSize", renderer, w, h);
}
