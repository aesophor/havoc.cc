// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_GUI_H_
#define HAVOC_GUI_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>

namespace gui {

void Init();
void Render(SDL_Window *window);

inline SDL_GLContext originalGlCtx = nullptr;

}  // namespace gui

#endif  // HAVOC_GUI_H_
