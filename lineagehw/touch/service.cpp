/*
 * SPDX-FileCopyrightText: 2022-2025 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "vendor.lineage.touch-service.moto_kona"

#include <android-base/logging.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>

#include "TouchscreenGesture.h"

using aidl::vendor::lineage::touch::TouchscreenGesture;

int main() {
    ABinderProcess_setThreadPoolMaxThreadCount(0);

    auto tg = ndk::SharedRefBase::make<TouchscreenGesture>();
    auto status = AServiceManager_addService(
            tg->asBinder().get(), TouchscreenGesture::makeServiceName("default").c_str());
    CHECK_EQ(status, STATUS_OK) << "Cannot register touchscreen gesture HAL service.";

    LOG(DEBUG) << "Touchscreen Gesture HAL service ready.";

    ABinderProcess_joinThreadPool();

    LOG(ERROR) << "Touchscreen HAL Gesture service failed to join thread pool.";
    return EXIT_FAILURE;  // should not reach
}
