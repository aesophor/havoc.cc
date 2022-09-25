// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#include "Interfaces.h"

#include "Core/Hooks.h"

namespace interfaces {

uintptr_t GetClientMode() {
  // Find IClientMode using signature scanning.
  constexpr uint8_t kSig[] = {0x48, 0x8b, 0xb7, 0x00, 0x00, 0x00, 0x00, 0x48,
                              0x8d, 0x3d, 0x00, 0x00, 0x00, 0x00, 0x5d, 0xe9};
  constexpr char kMask[] = "xxx????xxx????xx";

  uintptr_t sigAddr = clientDylib->ScanSignature(kSig, kMask) + 0xa;
  uintptr_t fileOffset = sigAddr - clientDylib->GetBase();
  uintptr_t offset = *reinterpret_cast<uint32_t *>(sigAddr);
  return clientDylib->GetBase() + (offset + fileOffset) + 0x4;
}

void Init() {
  clientDylib = std::make_unique<Dylib>("client.dylib");
  engineDylib = std::make_unique<Dylib>("engine.dylib");
  materialSystemDylib = std::make_unique<Dylib>("materialsystem.dylib");
  physicsDylib = std::make_unique<Dylib>("vphysics.dylib");
  studioRenderDylib = std::make_unique<Dylib>("studiorender.dylib");
  sdlDylib = std::make_unique<Dylib>("libSDL2-2.0.0.dylib");

  client = clientDylib->GetInterface<IBaseClientDLL>("VClient");
  clientMode = reinterpret_cast<IClientMode *>(GetClientMode());
  entityList = clientDylib->GetInterface<IClientEntityList>("VClientEntityList");
  engine = engineDylib->GetInterface<IEngineClient>("VEngineClient");
  engineTrace = engineDylib->GetInterface<IEngineTrace>("EngineTraceClient");
  materialSystem = materialSystemDylib->GetInterface<IMaterialSystem>("VMaterialSystem");
  modelInfo = engineDylib->GetInterface<IModelInfo>("VModelInfoClient");
  physics = physicsDylib->GetInterface<IPhysicsSurfaceProps>("VPhysicsSurfaceProps");
  studioRender = studioRenderDylib->GetInterface<IStudioRender>("VStudioRender");
}

}  // namespace interfaces
