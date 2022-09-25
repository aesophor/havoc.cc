// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_SKINS_H_
#define HAVOC_SKINS_H_

#include "SDK/CEntity.h"
#include "SDK/CRecvTable.h"

namespace hacks::skins {

struct KnifeModel {
  const char *name;
  const char *modelPath;
  const CItemDef itemDef;
};

inline constexpr KnifeModel knifeModels[] = {
  {"Default knife",        "",                                             WEAPON_UNDEFINED},
  {"Default knife (CT)",   "models/weapons/v_knife_default_ct.mdl",        WEAPON_KNIFE},
  {"Default knife (T)",    "models/weapons/v_knife_default_t.mdl",         WEAPON_KNIFE_T},
  {"Bayonet",              "models/weapons/v_knife_bayonet.mdl",           WEAPON_BAYONET},
  {"M9 Bayonet",           "models/weapons/v_knife_m9_bay.mdl",            WEAPON_KNIFE_M9_BAYONET},
  {"Flip knife",           "models/weapons/v_knife_flip.mdl",              WEAPON_KNIFE_FLIP},
  {"Gut knife",            "models/weapons/v_knife_gut.mdl",               WEAPON_KNIFE_GUT},
  {"Karambit knife",       "models/weapons/v_knife_karam.mdl",             WEAPON_KNIFE_KARAMBIT},
  {"Huntsman knife",       "models/weapons/v_knife_tactical.mdl",          WEAPON_KNIFE_TACTICAL},
  {"Falchion knife",       "models/weapons/v_knife_falchion_advanced.mdl", WEAPON_KNIFE_FALCHION},
  {"Survival bowie knife", "models/weapons/v_knife_survival_bowie.mdl",    WEAPON_KNIFE_SURVIVAL_BOWIE},
  {"Butterfly knife",      "models/weapons/v_knife_butterfly.mdl",         WEAPON_KNIFE_BUTTERFLY},
  {"Shadow daggers",       "models/weapons/v_knife_push.mdl",              WEAPON_KNIFE_PUSH},
};

inline constexpr int numKnifeModels = std::size(knifeModels);
inline int knifeIdx = 0;

bool Init();
void Run();
void FixAnimations(CRecvProxyData *data, CBaseViewModel *viewModel, void *);

}  // namespace hacks::skins

#endif  // HAVOC_SKINS_H_
