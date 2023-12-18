/*
 * Copyright (C) 2023 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <android-base/file.h>
#include <android-base/strings.h>

namespace mmi_gesture {

constexpr auto kGestureNode = "/sys/class/touchscreen/primary/gesture";
constexpr auto kGestureModeTypeNode = "/sys/class/touchscreen/primary/gesture_mode_type";

// Keep them in sync with the kernel
enum class Gesture { kSingleTap = 0x20, kDoubleTap = 0x30 };

inline bool IsEnabled(Gesture gesture) {
    std::string buf;
    if (!android::base::ReadFileToString(kGestureModeTypeNode, &buf)) {
        return false;
    }

    // Bits 0-2 correspond to zero tap, single tap, and double tap
    std::bitset<3> gesture_mode_type = std::stoi(android::base::Trim(buf));
    switch (gesture) {
        case Gesture::kSingleTap:
            return gesture_mode_type[1];
        case Gesture::kDoubleTap:
            return gesture_mode_type[2];
        default:
            return false;
    }
}

inline bool SetEnabled(Gesture gesture, bool enabled) {
    int code = static_cast<int>(gesture);
    if (enabled) ++code;
    if (!android::base::WriteStringToFile(std::to_string(code), kGestureNode)) {
        return false;
    }
    return true;
}

}  // namespace mmi_gesture
