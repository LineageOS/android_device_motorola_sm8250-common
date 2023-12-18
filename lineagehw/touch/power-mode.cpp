/*
 * Copyright (C) 2023 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <aidl/android/hardware/power/BnPower.h>
#include "MMIGesture.h"

namespace aidl {
namespace android {
namespace hardware {
namespace power {
namespace impl {

bool isDeviceSpecificModeSupported(Mode type, bool* _aidl_return) {
    if (type == Mode::DOUBLE_TAP_TO_WAKE) {
        *_aidl_return = true;
        return true;
    }
    return false;
}

bool setDeviceSpecificMode(Mode type, bool enabled) {
    if (type != Mode::DOUBLE_TAP_TO_WAKE) return false;
    return mmi_gesture::SetEnabled(mmi_gesture::Gesture::kDoubleTap, enabled);
}

}  // namespace impl
}  // namespace power
}  // namespace hardware
}  // namespace android
}  // namespace aidl
