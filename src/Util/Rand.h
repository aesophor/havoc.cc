// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_RAND_H_
#define HAVOC_RAND_H_

#include <cstdlib>

template <typename T>
int RandomInt(T min, T max) {
  int iMin = static_cast<int>(min);
  int iMax = static_cast<int>(max);
  return rand() % (iMax - iMin + 1) + iMin;
}

#endif  // HAVOC_RAND_H_
