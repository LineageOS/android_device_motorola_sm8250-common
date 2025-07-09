/*
 * SPDX-FileCopyrightText: 2022-2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "TouchscreenGestureService"

#include <android-base/file.h>
#include <android-base/logging.h>

#include "MMIGesture.h"
#include "TouchscreenGesture.h"

namespace {
struct GestureInfo {
    int32_t keycode;
    const char* name;
    const mmi_gesture::Gesture gesture;
};

constexpr GestureInfo kGestureNodes[] = {
        {59, "Single Tap", mmi_gesture::Gesture::kSingleTap},
};
}  // anonymous namespace

namespace aidl {
namespace vendor {
namespace lineage {
namespace touch {

ndk::ScopedAStatus TouchscreenGesture::getSupportedGestures(std::vector<Gesture>* _aidl_return) {
    std::vector<Gesture> gestures;
    for (int32_t i = 0; i < std::size(kGestureNodes); ++i) {
        gestures.push_back({i, kGestureNodes[i].name, kGestureNodes[i].keycode});
    }
    *_aidl_return = gestures;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus TouchscreenGesture::setGestureEnabled(const Gesture& gesture, bool enabled) {
    auto rc = ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    if (gesture.id >= std::size(kGestureNodes)) {
        return rc;
    }
    if (mmi_gesture::SetEnabled(kGestureNodes[gesture.id].gesture, enabled))
        rc = ndk::ScopedAStatus::ok();
    return rc;
}

}  // namespace touch
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
