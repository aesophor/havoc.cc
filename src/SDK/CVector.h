// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_C_VECTOR_H_
#define HAVOC_C_VECTOR_H_

#include <cmath>
#include <cstdint>
#include <numbers>

#define CVECTOR_DEF_OPERATOR_CONST(op)                        \
  constexpr CVector operator op(const CVector &other) const { \
    return {x op other.x, y op other.y, z op other.z};        \
  }

#define CVECTOR_DEF_OPERATOR(op)                         \
  constexpr CVector &operator op(const CVector &other) { \
    x op other.x;                                        \
    y op other.y;                                        \
    z op other.z;                                        \
    return *this;                                        \
  }

struct CVector {
  float x{}, y{}, z{};

  CVECTOR_DEF_OPERATOR_CONST(+)
  CVECTOR_DEF_OPERATOR_CONST(-)
  CVECTOR_DEF_OPERATOR_CONST(*)
  CVECTOR_DEF_OPERATOR_CONST(/)

  CVECTOR_DEF_OPERATOR(+=)
  CVECTOR_DEF_OPERATOR(-=)
  CVECTOR_DEF_OPERATOR(*=)
  CVECTOR_DEF_OPERATOR(/=)

  constexpr bool operator==(const CVector &other) const {
    return x == other.x && y == other.y && z == other.z;
  }

  constexpr bool operator!=(const CVector &other) const {
    return !(*this == other);
  }
  
  constexpr bool operator!() const {
    return *this == CVector{};
  }

  constexpr CVector Scale(float factor) const {
    return {x * factor, y * factor, z * factor};
  }

  inline CVector ToAngle() const {
    return {
      std::atan2(-z, std::hypot(x, y)) * (180.0f / std::numbers::pi_v<float>),
      std::atan2(y, x) * (180.0f / std::numbers::pi_v<float>),
      0.0f
    };
  }
};

struct __attribute__((aligned(16))) CVectorAligned : public CVector {
  float w{};

  constexpr CVectorAligned operator-(const CVectorAligned &other) const {
    return {x - other.x, y - other.y, z - other.z, w - other.w};
  }
};

#endif  // HAVOC_C_VECTOR_H_
