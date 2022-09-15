/*
 * Copyright (C) 2022 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "vendor.lineage.livedisplay@2.1-service.moto_kona"

#include <android-base/logging.h>
#include <binder/ProcessState.h>
#include <hidl/HidlTransportSupport.h>
#include <livedisplay/sdm/PictureAdjustment.h>
#include "SimpleMode.h"

using ::android::OK;
using ::android::sp;
using ::android::status_t;
using ::android::hardware::configureRpcThreadpool;
using ::android::hardware::joinRpcThreadpool;

using ::vendor::lineage::livedisplay::V2_0::sdm::PictureAdjustment;
using ::vendor::lineage::livedisplay::V2_0::sdm::SDMController;
using ::vendor::lineage::livedisplay::V2_1::implementation::AdaptiveBacklight;
using ::vendor::lineage::livedisplay::V2_1::implementation::AntiFlicker;
using ::vendor::lineage::livedisplay::V2_1::implementation::SunlightEnhancement;

status_t RegisterAsServices() {
    status_t status = OK;

    std::shared_ptr<SDMController> controller = std::make_shared<SDMController>();
    sp<PictureAdjustment> pa = new PictureAdjustment(controller);
    status = pa->registerAsService();
    if (status != OK) {
        LOG(ERROR) << "Could not register service for LiveDisplay HAL PictureAdjustment Iface ("
                   << status << ")";
        return status;
    }

    if (AdaptiveBacklight::isSupported()) {
        sp<AdaptiveBacklight> ab = new AdaptiveBacklight();
        status = ab->registerAsService();
        if (status != OK) {
            LOG(ERROR) << "Could not register service for LiveDisplay HAL AdaptiveBacklight Iface ("
                       << status << ")";
            return status;
        }
    }

    if (AntiFlicker::isSupported()) {
        sp<AntiFlicker> af = new AntiFlicker();
        status = af->registerAsService();
        if (status != OK) {
            LOG(ERROR) << "Could not register service for LiveDisplay HAL AntiFlicker Iface"
                       << " (" << status << ")";
            return status;
        }
    }

    if (SunlightEnhancement::isSupported()) {
        sp<SunlightEnhancement> se = new SunlightEnhancement();
        status = se->registerAsService();
        if (status != OK) {
            LOG(ERROR) << "Could not register service for LiveDisplay HAL SunlightEnhancement Iface"
                       << " (" << status << ")";
            return status;
        }
    }

    return OK;
}

int main() {
    android::ProcessState::initWithDriver("/dev/vndbinder");

    LOG(DEBUG) << "LiveDisplay HAL service is starting.";

    configureRpcThreadpool(1, true /*callerWillJoin*/);

    if (RegisterAsServices() == OK) {
        LOG(DEBUG) << "LiveDisplay HAL service is ready.";
        joinRpcThreadpool();
    } else {
        LOG(ERROR) << "Could not register service for LiveDisplay HAL";
    }

    // In normal operation, we don't expect the thread pool to shutdown
    LOG(ERROR) << "LiveDisplay HAL service is shutting down.";
    return 1;
}
