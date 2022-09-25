// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_INTERFACES_H_
#define HAVOC_INTERFACES_H_

#include <memory>

#include "Core/Dylib.h"
#include "SDK/IBaseClientDLL.h"
#include "SDK/IClientEntityList.h"
#include "SDK/IClientMode.h"
#include "SDK/IEngineTrace.h"
#include "SDK/IEngineClient.h"
#include "SDK/IModelInfo.h"

namespace interfaces {

void Init();

inline std::unique_ptr<Dylib> clientDylib = nullptr;
inline std::unique_ptr<Dylib> engineDylib = nullptr;
inline std::unique_ptr<Dylib> sdlDylib = nullptr;

// interface pointers
inline IBaseClientDLL *client = nullptr;
inline IClientEntityList *entityList = nullptr;
inline IClientMode *clientMode = nullptr;
inline IEngineClient *engine = nullptr;
inline IEngineTrace *engineTrace = nullptr;
inline IModelInfo *modelInfo = nullptr;

}  // namespace interfaces

#endif  // HAVOC_INTERFACES_H_
