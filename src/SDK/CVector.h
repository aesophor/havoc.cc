// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_C_VECTOR_H_
#define HAVOC_C_VECTOR_H_

#include <cmath>
#include <cstdint>
#include <numbers>

#include "Util/Math.h"

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

  constexpr operator bool() const {
    return *this != CVector{};
  }

  constexpr float Length() const {
    return std::hypot(x, y, z);
  }

  constexpr CVector Scale(const float factor) const {
    return {x * factor, y * factor, z * factor};
  }

  constexpr float Dot(const CVector &other) const {
    return x * other.x + y * other.y + z * other.z;
  }

  inline CVector ToAngle() const {
    return {
      std::atan2(-z, std::hypot(x, y)) * (180.0f / std::numbers::pi_v<float>),
      std::atan2(y, x) * (180.0f / std::numbers::pi_v<float>),
      0.0f
    };
  }

  inline CVector ToDirection() const {
    float sp, sy, cp, cy;
    SinCos(Deg2Rad(y), sy, cy);
    SinCos(Deg2Rad(x), sp, cp);
    return CVector{cp * cy, cp * sy, -sp}.Normalize();
  }

  inline CVector Clamp() const {
    CVector ret{x, y, 0};

    if (ret.y > 180.0f) {
      ret.y = 180.0f;
    } else if (ret.y < -180.0f) {
      ret.y = -180.0f;
    }

    if (ret.x > 89.0f) {
      ret.x = 89.0f;
    } else if (ret.x < -89.0f) {
      ret.x = -89.0f;
    }

    return ret;
  }

  inline CVector NormalizeAngle() const {
    CVector ret{x, y, 0};

    while (ret.x > 89.0f) {
      ret.x -= 180.f;
    }

    while (ret.x < -89.0f) {
      ret.x += 180.f;
    }

    while (ret.y > 180.f) {
      ret.y -= 360.f;
    }

    while (ret.y < -180.f) {
      ret.y += 360.f;
    }

    return ret;
  }

  inline CVector Normalize() const {
    CVector ret;
    float length = std::hypot(x, y, z);

    if (length != 0.0f) {
      ret.x = x / length;
      ret.y = y / length;
      ret.z = z / length;
    } else {
      ret.x = ret.y = 0.0f;
      ret.z = 1.0f;
    }

    return ret;
  }
};

struct __attribute__((aligned(16))) CVectorAligned : public CVector {
  float w{};

  constexpr CVectorAligned operator-(const CVectorAligned &other) const {
    return {x - other.x, y - other.y, z - other.z, w - other.w};
  }
};

#endif  // HAVOC_C_VECTOR_H_
