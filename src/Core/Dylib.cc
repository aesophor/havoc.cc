// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#include "Dylib.h"

#include <mach-o/dyld.h>
#include <sys/stat.h>

#include <cassert>
#include <string_view>

Dylib::Dylib(const std::string &name) : name(name), handle(), base(), size() {
  for (uint32_t i = 0; i < _dyld_image_count(); i++) {
    std::string image_name = _dyld_get_image_name(i);
    struct stat sb;

    if (image_name.find(name) != image_name.npos && !stat(image_name.c_str(), &sb)) {
      handle = dlopen(image_name.c_str(), RTLD_GLOBAL | RTLD_LAZY | RTLD_NOW);
      base = reinterpret_cast<uintptr_t>(_dyld_get_image_header(i));
      size = sb.st_size;
    }
  }
}

uintptr_t Dylib::ScanSignature(const uint8_t *sig, const std::string &mask) const {
  for (size_t i = 0; i < size; i++) {
    if (Compare((const uint8_t *) (base + i), sig, mask)) {
      return base + i;
    }
  }
  return 0;
}

bool Dylib::Compare(const uint8_t *data, const uint8_t *sig, const std::string &mask) const {
  for (off_t i = 0; i < mask.size(); i++) {
    if (mask[i] == 'x' && data[i] != sig[i]) {
      return false;
    }
  }
  return true;
}
