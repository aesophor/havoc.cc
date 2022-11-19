// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_SETTINGS_H_
#define HAVOC_SETTINGS_H_

#include "Hacks/AntiAim.h"
#include "Hacks/AutoStrafe.h"
#include "SDK/CVector.h"

namespace settings {

namespace aimbot {

inline bool isEnabled = true;
inline bool shouldRage = false;
inline bool shouldAutoFire = false;
inline bool shouldAutoScope = false;
inline bool shouldShootTeammates = false;
inline bool shouldOnlyAllowAWPandSSG = true;
inline float hitChance = 0.4f;
inline int fireDelayMs = 50;
inline int nextFireDelayMs = 50;

}  // namespace aimbot

namespace autowall {

inline bool isEnabled = false;

} // namespace autowall

namespace antiaim {

inline bool isEnabled = false;
inline bool isYawEnabled = true;
inline bool isPitchEnabled = true;
inline hacks::antiaim::AntiAimTypeY realYawType = hacks::antiaim::AntiAimTypeY::MAX_DELTA_FLIPPER;
inline hacks::antiaim::AntiAimTypeY fakeYawType = hacks::antiaim::AntiAimTypeY::NONE;
inline hacks::antiaim::AntiAimTypeX pitchType = hacks::antiaim::AntiAimTypeX::STATIC_DOWN;
inline CVector realAngle{};
inline CVector fakeAngle{};

inline bool isHeadEdgeEnabled = false;
inline float headEdgeDistance = 25.f;
inline bool isLBYBreakerEnabled = false;
inline float lbyBreakerOffset = 180.f;

}  // namespace antiaim

namespace autostrafe {

inline bool isEnabled = false;
inline hacks::autostrafe::AutoStrafeType autoStrafeType = hacks::autostrafe::AutoStrafeType::RAGE;

}  // namespace autostrafe

namespace bhop {

inline bool isEnabled = false;

}  // namespace bhop

namespace chams {

inline bool isEnabled = false;

}  // namespace chams

namespace glow {

inline bool isEnabled = false;
inline bool shouldShowTeammates = false;
inline float userAlpha = 0.35f;

}  // namespace glow

namespace skins {

inline int knifeIdx = 0;

}  // namespace skins

}  // namespace settings

#endif  // HAVOC_SETTINGS_H_
