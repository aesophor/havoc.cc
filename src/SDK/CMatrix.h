// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_C_MATRIX_H_
#define HAVOC_C_MATRIX_H_

#include "SDK/CVector.h"

class CMatrix3x4 {
 public:
  constexpr float *operator[](size_t index) {
    return data[index];
  }

  constexpr CVector Origin() {
    return {data[0][3], data[1][3], data[2][3]};
  }

  float data[3][4];
};

class CMatrix4x4 {
 public:
  float data[4][4];
};

#endif  // HAVOC_C_MATRIX_H_
