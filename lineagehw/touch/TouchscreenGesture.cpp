/*
 * Copyright (C) 2022-2023 The LineageOS Project
 *
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

namespace vendor {
namespace lineage {
namespace touch {
namespace V1_0 {
namespace implementation {

using ::android::hardware::Void;

Return<void> TouchscreenGesture::getSupportedGestures(getSupportedGestures_cb resultCb) {
    std::vector<Gesture> gestures;
    for (int32_t i = 0; i < std::size(kGestureNodes); ++i) {
        gestures.push_back({i, kGestureNodes[i].name, kGestureNodes[i].keycode});
    }
    resultCb(gestures);
    return Void();
}

Return<bool> TouchscreenGesture::setGestureEnabled(
        const ::vendor::lineage::touch::V1_0::Gesture& gesture, bool enabled) {
    if (gesture.id >= std::size(kGestureNodes)) {
        return false;
    }
    return mmi_gesture::SetEnabled(kGestureNodes[gesture.id].gesture, enabled);
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace touch
}  // namespace lineage
}  // namespace vendor
