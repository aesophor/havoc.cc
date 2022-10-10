// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#include "Dylib.h"

#include <string_view>

#include <mach-o/dyld.h>
#include <sys/stat.h>

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

uintptr_t Dylib::ScanSignature(const std::string_view sig) const {
  for (size_t i = 0; i < size; i++) {
    if (Compare(reinterpret_cast<const char *>(base + i), sig)) {
      return base + i;
    }
  }
  return 0;
}

bool Dylib::Compare(const char *data, const std::string_view sig) const {
  for (off_t i = 0; i < sig.size(); i++) {
    if (sig[i] != '?' && sig[i] != data[i]) {
      return false;
    }
  }
  return true;
}
