// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_TYPE_TRAITS_H_
#define HAVOC_TYPE_TRAITS_H_

#define MAKE_NONCOPYABLE(C) \
  C(const C &) = delete;    \
  C &operator=(const C &) = delete

#define MAKE_NONMOVABLE(C) \
  C(C &&) = delete;        \
  C &operator=(C &&) = delete

#endif  // HAVOC_TYPE_TRAITS_H_
