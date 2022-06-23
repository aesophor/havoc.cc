// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#include "NetVars.h"

#include <cassert>

#include "Core/Interfaces.h"

namespace netvars {

void Init() {
  assert(interfaces::client);

  for (auto c = interfaces::client->GetAllClasses(); c; c = c->next) {
    if (c->table) {
      Dump(c->networkName, c->table, 0);
    }
  }
}

void Dump(const std::string &networkName, CRecvTable *recvTable, int offset) {
  for (auto i = 0; i < recvTable->count; i++) {
    const CRecvProp *prop = &recvTable->props[i];

    if (!prop) {
      continue;
    }

    if (std::isdigit(prop->name[0])) {
      continue;
    }

    if (std::string(prop->name) == "baseclass") {
      continue;
    }

    if (prop->type == ESendPropType::DATATABLE &&
        prop->table &&
        prop->table->name[0] == 'D') {
      Dump(networkName, prop->table, offset + prop->offset);
    }

    std::string key = networkName + "->" + prop->name;
    table[key] = offset + prop->offset;
  }
}

}  // namespace netvars
