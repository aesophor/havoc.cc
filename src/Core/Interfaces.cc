// Copyright (c) 2023 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#include "Interfaces.h"

#include "Core/Hooks.h"
#include "Core/Memory.h"

namespace interfaces {

uintptr_t GetClientMode() {
  constexpr std::string_view kSig = "\x48\x8b\xb7????\x48\x8d\x3d????\x5d\xe9";

  constexpr auto kSizeOfInst = 7;
  uintptr_t sigAddr = clientDylib->ScanSignature(kSig);
  uintptr_t offsetAddr = sigAddr + 10;
  return sigAddr + 2 * kSizeOfInst + *reinterpret_cast<uint32_t *>(offsetAddr);
}

uintptr_t GetMoveHelperPtr() {
  // https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/game/shared/gamemovement.cpp#LL3401C3-L3403C60
  //
  // engine->Con_NPrintf(
  //   isServer,
  //   "%s stuck on object %i/%s",
	//   isServer ? "server" : "client",
  //   hitent.GetEntryIndex(),
  //   MoveHelper()->GetName(hitent)
  // );
  //
  // rdi: this
  // rsi: isServer
  // rdx: "%s stuck on object %i/%s",
  // rcx: "server"/"client"
  // r8:  hitent.GetEntryIndex()
  // r9:  MoveHelper()->GetName(hitent)
  //
  // __text:00000000001B5DD3 48 8D 05 7E ED E8 01     lea     rax, [rip+0x1E8ED7E] <-- MoveHelper()
  // __text:00000000001B5DDA 48 8B 38                 mov     rdi, [rax]
  // __text:00000000001B5DDD 48 8B 07                 mov     rax, [rdi]
  // __text:00000000001B5DE0 48 8B 00                 mov     rax, [rax]
  // __text:00000000001B5DE3 89 9D 48 FF FF FF        mov     [rbp+var_B8], ebx
  // __text:00000000001B5DE9 48 8D B5 48 FF FF FF     lea     rsi, [rbp+var_B8]
  // __text:00000000001B5DF0 FF D0                    call    rax
  // __text:00000000001B5DF2 49 89 C1                 mov     r9, rax
  // __text:00000000001B5DF5 48 8D 15 15 2C E4 00     lea     rdx, aSStuckOnObject ; "%s stuck on object %i/%s"
  // __text:00000000001B5DFC 48 8D 0D 1D 7A E2 00     lea     rcx, aClient    ; "client"
  // __text:00000000001B5E03 31 F6                    xor     esi, esi
  // __text:00000000001B5E05 31 C0                    xor     eax, eax
  // __text:00000000001B5E07 4C 89 FF                 mov     rdi, r15
  // __text:00000000001B5E0A 45 89 E0                 mov     r8d, r12d
  // __text:00000000001B5E0D FF 95 30 FF FF FF        call    [rbp+var_D0]
  constexpr std::string_view kSig = "\x48\x8d\x05????\x48\x8b\x38\x48\x8b\x07\x48\x8b?\x89\x9d\x48\xff\xff\xff";

  // rip = the address of the "next" instruction to execute.
  // rip = 0x1B5DDA, *offset = 0x1E8ED7E
  constexpr auto kSizeOfInst = 7;
  uintptr_t sigAddr = clientDylib->ScanSignature(kSig);
  uintptr_t offsetAddr = sigAddr + 3;
  return sigAddr + kSizeOfInst + *reinterpret_cast<uint32_t *>(offsetAddr);
}

uintptr_t GetGlobalVarsPtr() {
  constexpr std::string_view kSig = "\x48\x8d\x05????\x48\x8b?\xf3\x0f\x10??\xf3\x0f\x11\x83";

  constexpr auto kSizeOfInst = 7;
  uintptr_t sigAddr = clientDylib->ScanSignature(kSig);
  uintptr_t offsetAddr = sigAddr + 3;
  return sigAddr + kSizeOfInst + *reinterpret_cast<uint32_t *>(offsetAddr);
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
  moveHelper = **reinterpret_cast<IMoveHelper ***>(GetMoveHelperPtr());
  physics = physicsDylib->GetInterface<IPhysicsSurfaceProps>("VPhysicsSurfaceProps");
  prediction = clientDylib->GetInterface<IPrediction>("VClientPrediction");
  studioRender = studioRenderDylib->GetInterface<IStudioRender>("VStudioRender");

  // Global variables
  globalVars = *reinterpret_cast<CGlobalVars **>(GetGlobalVarsPtr());
  moveData = nullptr;
  predictionRandomSeed = nullptr;
}

}  // namespace interfaces
