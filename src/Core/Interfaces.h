// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_INTERFACES_H_
#define HAVOC_INTERFACES_H_

#include <memory>

#include "Core/Dylib.h"
#include "SDK/IBaseClientDLL.h"
#include "SDK/IClientEntityList.h"
#include "SDK/IClientMode.h"
#include "SDK/IEngineTrace.h"
#include "SDK/IVEngineClient.h"
#include "SDK/IVModelInfo.h"

namespace interfaces {

void Init();

// capture an individual interface by name & module
template <typename Interface>
Interface *Capture(const char *moduleName, const char *interfaceName);

inline std::unique_ptr<Dylib> clientDylib = nullptr;
inline std::unique_ptr<Dylib> engineDylib = nullptr;
inline std::unique_ptr<Dylib> sdlDylib = nullptr;

// interface pointers
inline IBaseClientDLL *client = nullptr;
inline IClientEntityList *entityList = nullptr;
inline IVEngineClient *engine = nullptr;
inline IEngineTrace *engineTrace = nullptr;
inline IVModelInfo *modelInfo = nullptr;
inline IClientMode *clientMode = nullptr;

}  // namespace interfaces

#endif  // HAVOC_INTERFACES_H_
