// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_GUI_H_
#define HAVOC_GUI_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>

namespace gui {

void Init();
void SwapWindow(SDL_Window *window);

inline SDL_GLContext originalGlCtx = nullptr;

}  // namespace gui

namespace hooks {

void SwapWindow(SDL_Window *window);
int PollEvent(SDL_Event *event);

using SwapWindowFn = void (*)(SDL_Window *window);
inline SwapWindowFn originalSwapWindow = nullptr;

using PollEventFn = int (*)(SDL_Event *event);
inline PollEventFn originalPollEvent = nullptr;

}  // namespace hooks

#endif  // HAVOC_GUI_H_
