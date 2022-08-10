/*
 * Copyright (C) 2022 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "vendor.lineage.touch@1.0-service.moto_kona"

#include <android-base/logging.h>
#include <hidl/HidlTransportSupport.h>

#include "HighTouchPollingRate.h"
#include "TouchscreenGesture.h"

using ::android::OK;
using ::android::sp;

using ::vendor::lineage::touch::V1_0::ITouchscreenGesture;
using ::vendor::lineage::touch::V1_0::implementation::TouchscreenGesture;
#ifdef USE_TOUCH_POLLING_RATE
using ::vendor::lineage::touch::V1_0::IHighTouchPollingRate;
using ::vendor::lineage::touch::V1_0::implementation::HighTouchPollingRate;
#endif

int main() {
#ifdef USE_TOUCH_POLLING_RATE
    sp<IHighTouchPollingRate> highTouchPollingRate = new HighTouchPollingRate();
#endif
    sp<ITouchscreenGesture> gesture_service = new TouchscreenGesture();

    android::hardware::configureRpcThreadpool(1, true /*callerWillJoin*/);

    if (gesture_service->registerAsService() != OK) {
        LOG(ERROR) << "Cannot register touchscreen gesture HAL service.";
        return 1;
    }

#ifdef USE_TOUCH_POLLING_RATE
    if (highTouchPollingRate->registerAsService() != OK) {
        LOG(ERROR) << "Cannot register touchscreen high polling rate HAL service.";
        return 1;
    }
#endif

    LOG(DEBUG) << "Touchscreen HAL service ready.";

    android::hardware::joinRpcThreadpool();

    LOG(ERROR) << "Touchscreen HAL service failed to join thread pool.";
    return 1;
}
