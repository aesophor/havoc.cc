// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_SKINS_H_
#define HAVOC_SKINS_H_

#include "SDK/CEntity.h"
#include "SDK/CRecvTable.h"

namespace hacks::skins {

bool Init();
void Run();
void FixAnimations(CRecvProxyData *data, CBaseViewModel *viewModel, void *);

bool IsEnabled();
void Toggle();

}  // namespace hacks::skins

#endif  // HAVOC_SKINS_H_
