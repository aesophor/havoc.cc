// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_DYLIB_H_
#define HAVOC_DYLIB_H_

#include <dlfcn.h>

#include <cstddef>
#include <cstdint>
#include <string>

#include "Util/TypeTraits.h"

class Dylib final {
  MAKE_NONCOPYABLE(Dylib);
  MAKE_NONMOVABLE(Dylib);

  struct InterfaceReg {
    using CreateFn = void *(*) ();

    CreateFn createFn;
    char *name;
    InterfaceReg *next;
  };

 public:
  explicit Dylib(const std::string &name);

  uintptr_t ScanSignature(const uint8_t *sig, const std::string &mask) const;

  template <typename T>
  T *GetInterface(const std::string &symbol, bool exact = false) const {
    InterfaceReg *begin =
        *reinterpret_cast<InterfaceReg **>(dlsym(handle, "s_pInterfaceRegs"));

    for (InterfaceReg *i = begin; i; i = i->next) {
      if (exact && symbol == i->name) {
        return reinterpret_cast<T *>(i->createFn());
      } else if (!exact && strstr(i->name, symbol.c_str()) &&
                 strlen(i->name) - 3 == strlen(symbol.c_str())) {
        return reinterpret_cast<T *>(i->createFn());
      }
    }

    return nullptr;
  }

  uintptr_t GetBase() const {
    return base;
  }
  size_t GetSize() const {
    return size;
  }

 private:
  bool Compare(const uint8_t *data, const uint8_t *sig, const std::string &mask) const;

  std::string name;
  void *handle;
  uintptr_t base;
  size_t size;
};

#endif  // HAVOC_DYLIB_H_
