// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_NETVARS_H_
#define HAVOC_NETVARS_H_

#include <string>
#include <unordered_map>

#include "SDK/CRecvTable.h"

#define NETVAR(name, var, ...)                                                           \
  inline __VA_ARGS__ name() {                                                            \
    static const std::uint32_t offset = netvars::table[var];                             \
    return *reinterpret_cast<std::add_pointer_t<__VA_ARGS__>>(uintptr_t(this) + offset); \
  }

namespace netvars {

void Init();
void Dump(const std::string &networkName, CRecvTable *recvTable, int offset);

inline std::unordered_map<std::string, int> table;

}  // namespace netvars

#endif  // HAVOC_NETVARS_H_
