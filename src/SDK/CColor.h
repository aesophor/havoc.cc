// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_C_COLOR_H_
#define HAVOC_C_COLOR_H_

#include <cstdint>

class CColor {
 public:
  constexpr CColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
      : r(r), g(g), b(b), a(a) {}

  static CColor Red(uint8_t a = 255) {
    return {255, 0, 0, a};
  }

  static CColor Green(uint8_t a = 255) {
    return {0, 255, 0, a};
  }

  static CColor Blue(uint8_t a = 255) {
    return {0, 0, 255, a};
  }

  static CColor Black(uint8_t a = 255) {
    return {0, 0, 0, a};
  }

  static CColor White(uint8_t a = 255) {
    return {255, 255, 255, a};
  }

  uint8_t r{}, g{}, b{}, a{};
};

#endif  // HAVOC_C_COLOR_H_
