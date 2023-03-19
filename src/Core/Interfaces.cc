// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#include "Interfaces.h"

#include "Core/Hooks.h"
#include "Core/Memory.h"

namespace interfaces {

uintptr_t GetClientMode() {
  constexpr std::string_view kSig = "\x48\x8b\xb7????\x48\x8d\x3d????\x5d\xe9";

  uintptr_t sigAddr = clientDylib->ScanSignature(kSig) + 0xa;
  uintptr_t fileOffset = sigAddr - clientDylib->GetBase();
  uintptr_t offset = *reinterpret_cast<uint32_t *>(sigAddr);
  return clientDylib->GetBase() + (offset + fileOffset) + 0x4;
}

uintptr_t GetGlobalVarsPtr() {
  constexpr std::string_view kSig = "\x48\x8d\x05????\x48\x8b?\xf3\x0f\x10??\xf3\x0f\x11\x83";

  uintptr_t sigAddr = clientDylib->ScanSignature(kSig) + 0x3;
  uintptr_t fileOffset = sigAddr - clientDylib->GetBase();
  uintptr_t offset = *reinterpret_cast<uint32_t *>(sigAddr);
  return clientDylib->GetBase() + (offset + fileOffset) + 0x4;
}

void Init() {
  // Internal dylibs
  clientDylib = std::make_unique<Dylib>("client.dylib");
  engineDylib = std::make_unique<Dylib>("engine.dylib");
  materialSystemDylib = std::make_unique<Dylib>("materialsystem.dylib");
  physicsDylib = std::make_unique<Dylib>("vphysics.dylib");
  studioRenderDylib = std::make_unique<Dylib>("studiorender.dylib");

  // External dylibs
  sdlDylib = std::make_unique<Dylib>("libSDL2-2.0.0.dylib");

  // Interface pointers
  client = clientDylib->GetInterface<IBaseClientDLL>("VClient");
  clientMode = reinterpret_cast<IClientMode *>(GetClientMode());
  entityList = clientDylib->GetInterface<IClientEntityList>("VClientEntityList");
  engine = engineDylib->GetInterface<IEngineClient>("VEngineClient");
  engineTrace = engineDylib->GetInterface<IEngineTrace>("EngineTraceClient");
  gameMovement = clientDylib->GetInterface<IGameMovement>("GameMovement");
  materialSystem = materialSystemDylib->GetInterface<IMaterialSystem>("VMaterialSystem");
  modelInfo = engineDylib->GetInterface<IModelInfo>("VModelInfoClient");
  physics = physicsDylib->GetInterface<IPhysicsSurfaceProps>("VPhysicsSurfaceProps");
  prediction = clientDylib->GetInterface<IPrediction>("VClientPrediction");
  studioRender = studioRenderDylib->GetInterface<IStudioRender>("VStudioRender");

  globalVars = *reinterpret_cast<CGlobalVars **>(GetGlobalVarsPtr());
}

}  // namespace interfaces
