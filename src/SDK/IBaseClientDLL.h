// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_I_BASE_CLIENT_DLL_H_
#define HAVOC_I_BASE_CLIENT_DLL_H_

#include "Core/Memory.h"
#include "SDK/CClientClass.h"

enum class ClientFrameStage {
  UNDEFINED = -1,
  START,

  NET_UPDATE_START,
  NET_UPDATE_POSTDATAUPDATE_START,
  NET_UPDATE_POSTDATAUPDATE_END,
  NET_UPDATE_END,

  RENDER_START,
  RENDER_END,

  NET_FULL_FRAME_UPDATE_ON_REMOVE
};

class IBaseClientDLL {
 public:
  constexpr CClientClass *GetAllClasses() {
    return memory::CallVFunc<CClientClass *>(this, 8);
  }

  constexpr void FrameStageNotify(ClientFrameStage stage) {
    memory::CallVFunc<void>(this, 37, stage);
  }
};

#endif  // HAVOC_I_BASE_CLIENT_DLL_H_
