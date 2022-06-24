// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#include "Interfaces.h"

#include "Core/Hooks.h"

namespace interfaces {

uintptr_t GetClientMode() {
  // Find IClientMode using signature scanning.
  static const uint8_t sig[] = {0x48, 0x8b, 0xb7, 0x00, 0x00, 0x00, 0x00, 0x48,
                                0x8d, 0x3d, 0x00, 0x00, 0x00, 0x00, 0x5d, 0xe9};

  uintptr_t sigAddr = clientDylib->ScanSignature(sig, "xxx????xxx????xx") + 0xa;
  uintptr_t fileOffset = sigAddr - clientDylib->GetBase();
  uintptr_t offset = *reinterpret_cast<uint32_t *>(sigAddr);
  return clientDylib->GetBase() + (offset + fileOffset) + 0x4;
}

inline uintptr_t* GetAbsAddr(uintptr_t base, uintptr_t memoryPtr) {
  uintptr_t sigAddr = memoryPtr + 0xF;
  uintptr_t fileOffset = sigAddr - base;
  uintptr_t offset = *reinterpret_cast<uint32_t*>(sigAddr);
  return reinterpret_cast<uintptr_t*>(base + (offset + fileOffset) + 0x4);
}

void Init() {
  clientDylib = std::make_unique<Dylib>("client.dylib");
  engineDylib = std::make_unique<Dylib>("engine.dylib");
  sdlDylib = std::make_unique<Dylib>("libSDL2-2.0.0.dylib");

  client = clientDylib->GetInterface<IBaseClientDLL>("VClient");
  entityList = clientDylib->GetInterface<IClientEntityList>("VClientEntityList");
  engine = engineDylib->GetInterface<IVEngineClient>("VEngineClient");
  engineTrace = engineDylib->GetInterface<IEngineTrace>("EngineTraceClient");
  modelInfo = engineDylib->GetInterface<IVModelInfo>("VModelInfoClient");
  clientMode = reinterpret_cast<IClientMode *>(GetClientMode());
}

}  // namespace interfaces
