// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_MATH_H_
#define HAVOC_MATH_H_

#include <cmath>
#include <numbers>

inline void SinCos(float radians, float &sin, float &cos) {
  double sinReg, cosReg;
  __asm ("fsincos" : "=t" (cosReg), "=u" (sinReg) : "0" (radians));
  sin = sinReg;
  cos = cosReg;
}

constexpr float Deg2Rad(const float deg) {
  return deg * std::numbers::pi_v<float> / 180.f;
}

#endif  // HAVOC_MATH_H_