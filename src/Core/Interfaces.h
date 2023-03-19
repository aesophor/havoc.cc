// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_INTERFACES_H_
#define HAVOC_INTERFACES_H_

#include <memory>

#include "Core/Dylib.h"
#include "SDK/CGlobalVars.h"
#include "SDK/IBaseClientDLL.h"
#include "SDK/IClientEntityList.h"
#include "SDK/IClientMode.h"
#include "SDK/IEngineClient.h"
#include "SDK/IEngineTrace.h"
#include "SDK/IGameMovement.h"
#include "SDK/IMaterialSystem.h"
#include "SDK/IModelInfo.h"
#include "SDK/IPrediction.h"
#include "SDK/IPhysicsSurfaceProps.h"
#include "SDK/IStudioRender.h"

namespace interfaces {

void Init();

// Internal dylibs
inline std::unique_ptr<Dylib> clientDylib;
inline std::unique_ptr<Dylib> engineDylib;
inline std::unique_ptr<Dylib> materialSystemDylib;
inline std::unique_ptr<Dylib> physicsDylib;
inline std::unique_ptr<Dylib> studioRenderDylib;

// External dylibs
inline std::unique_ptr<Dylib> sdlDylib;

// Interface pointers
inline IBaseClientDLL *client = nullptr;
inline IClientEntityList *entityList = nullptr;
inline IClientMode *clientMode = nullptr;
inline IEngineClient *engine = nullptr;
inline IEngineTrace *engineTrace = nullptr;
inline IGameMovement *gameMovement = nullptr;
inline IMaterialSystem *materialSystem = nullptr;
inline IModelInfo *modelInfo = nullptr;
inline IPhysicsSurfaceProps *physics = nullptr;
inline IPrediction *prediction = nullptr;
inline IStudioRender *studioRender = nullptr;

inline CGlobalVars *globalVars = nullptr;

}  // namespace interfaces

#endif  // HAVOC_INTERFACES_H_
