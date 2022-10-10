// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_TIME_H_
#define HAVOC_TIME_H_

#include <chrono>

inline uint64_t GetCurrentTimestampMs() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
      std::chrono::system_clock::now().time_since_epoch()).count();
}

#endif  // HAVOC_TIME_H_
