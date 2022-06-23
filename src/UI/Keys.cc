// Copyright (c) 2022 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#include "Keys.h"

#include <thread>

#include "Hacks/Aimbot.h"
#include "Hacks/Glow.h"

namespace ui::keys {

namespace {
CGEventFlags lastFlags = 0;
}  // namespace

void Init() {
  // Create an event tap to retrieve keypresses.
  CGEventMask eventMask =
      CGEventMaskBit(kCGEventKeyDown) | CGEventMaskBit(kCGEventFlagsChanged);
  CFMachPortRef eventTap =
      CGEventTapCreate(kCGSessionEventTap, kCGHeadInsertEventTap, kCGEventTapOptionDefault,
                       eventMask, CGEventCallback, nullptr);

  // Create a run loop source and add enable the event tap.
  CFRunLoopSourceRef runLoopSource =
      CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
  CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);
  CGEventTapEnable(eventTap, true);

  std::thread(CFRunLoopRun).detach();
}

CGEventRef CGEventCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event,
                           void *refcon) {
  if (type != kCGEventKeyDown && type != kCGEventFlagsChanged) {
    return event;
  }

  CGEventFlags flags = CGEventGetFlags(event);

  // Retrieve the incoming keycode.
  CGKeyCode keyCode = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);

  // Calculate key up/down.
  bool down = false;
  if (type == kCGEventKeyDown) {
    down = true;
  } else if (type == kCGEventFlagsChanged) {
    switch (keyCode) {
      case 54:  // [right-cmd]
      case 55:  // [left-cmd]
        down = (flags & kCGEventFlagMaskCommand) && !(lastFlags & kCGEventFlagMaskCommand);
        break;
      case 56:  // [left-shift]
      case 60:  // [right-shift]
        down = (flags & kCGEventFlagMaskShift) && !(lastFlags & kCGEventFlagMaskShift);
        break;
      case 58:  // [left-option]
      case 61:  // [right-option]
        down = (flags & kCGEventFlagMaskAlternate) && !(lastFlags & kCGEventFlagMaskAlternate);
        break;
      case 59:  // [left-ctrl]
      case 62:  // [right-ctrl]
        down = (flags & kCGEventFlagMaskControl) && !(lastFlags & kCGEventFlagMaskControl);
        break;
      case 57:  // [caps]
        down =
            (flags & kCGEventFlagMaskAlphaShift) && !(lastFlags & kCGEventFlagMaskAlphaShift);
        break;
      default:
        break;
    }
  }
  lastFlags = flags;

  if (!down) {
    return event;
  }

  if (keyCode == 9) {  // 'v' or 'V'
    hacks::glow::Toggle();
  } else if (keyCode == 8) {  // 'c' or 'C'
    hacks::aimbot::Toggle();
  }

  return event;
}

}  // namespace ui::keys
