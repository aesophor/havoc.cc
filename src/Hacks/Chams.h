// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_CHAMS_H_
#define HAVOC_CHAMS_H_

#include "SDK/CMatrix.h"
#include "SDK/IStudioRender.h"

namespace hacks::chams {

bool Init();
void DrawModel(IStudioRender *thisptr, void *results, const CDrawModelInfo &info,
               CMatrix3x4 *bones, float *flexWeights, float *flexDelayedWeights,
               const CVector &modelOrigin, const int flags);

}  // namespace hacks::chams

#endif  // HAVOC_CHAMS_H_
