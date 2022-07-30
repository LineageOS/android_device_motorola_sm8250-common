/*
 * Copyright (C) 2022 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "HighTouchPollingRateService"

#include "HighTouchPollingRate.h"

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/strings.h>

namespace vendor {
namespace lineage {
namespace touch {
namespace V1_0 {
namespace implementation {

const std::string kInterpolationPath = "/sys/class/touchscreen/primary/interpolation";

Return<bool> HighTouchPollingRate::isEnabled() {
    std::string buf;
    if (!android::base::ReadFileToString(kInterpolationPath, &buf)) {
        LOG(ERROR) << "Failed to read " << kInterpolationPath;
        return false;
    }
    return std::stoi(android::base::Trim(buf)) == 1;
}

Return<bool> HighTouchPollingRate::setEnabled(bool enabled) {
    if (!android::base::WriteStringToFile(std::to_string(enabled), kInterpolationPath)) {
        LOG(ERROR) << "Failed to write " << kInterpolationPath;
        return false;
    }
    return true;
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace touch
}  // namespace lineage
}  // namespace vendor
