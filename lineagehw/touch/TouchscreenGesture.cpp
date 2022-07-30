/*
 * Copyright (C) 2022 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "TouchscreenGestureService"

#include <android-base/file.h>
#include <android-base/logging.h>

#include "TouchscreenGesture.h"

namespace {
struct GestureInfo {
    int32_t keycode;
    const char* name;
    const char* path;
};

constexpr GestureInfo kGestureNodes[] = {
        {250, "Single Tap", SINGLE_TAP_PATH},
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

    if (!android::base::WriteStringToFile(std::to_string(enabled),
                                          kGestureNodes[gesture.id].path)) {
        LOG(ERROR) << "Wrote file " << kGestureNodes[gesture.id].path << " failed";
        return false;
    }

    return true;
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace touch
}  // namespace lineage
}  // namespace vendor
