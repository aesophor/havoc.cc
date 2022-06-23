// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_C_UTL_VECTOR_H_
#define HAVOC_C_UTL_VECTOR_H_

#include <cstdint>

template <typename T>
class CUtlVector {
 public:
  // non const indexing
  constexpr T &operator[](size_t index) {
    return memory[index];
  }

  // const indexing
  constexpr const T &operator[](size_t index) const {
    return memory[index];
  }

  T *memory;
  int32_t allocCount;
  int32_t growSize;
  int32_t size;
  T *elments;
};

#endif  // HAVOC_C_UTL_VECTOR_H_
