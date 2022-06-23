// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef HAVOC_KEYS_H_
#define HAVOC_KEYS_H_

#include <ApplicationServices/ApplicationServices.h>
#include <Carbon/Carbon.h>

namespace ui::keys {

void Init();

CGEventRef CGEventCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event,
                           void *refcon);

}  // namespace ui::keys

#endif  // HAVOC_KEYS_H_
