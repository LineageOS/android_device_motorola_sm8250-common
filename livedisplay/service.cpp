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

#define LOG_TAG "vendor.lineage.livedisplay@2.1-service.moto_kona"

#include <android-base/logging.h>
#include <binder/ProcessState.h>
#include <hidl/HidlTransportSupport.h>
#include <livedisplay/sdm/PictureAdjustment.h>
#include <vendor/lineage/livedisplay/2.1/IPictureAdjustment.h>

using ::android::OK;
using ::android::sp;
using ::android::status_t;
using ::android::hardware::configureRpcThreadpool;
using ::android::hardware::joinRpcThreadpool;

using ::vendor::lineage::livedisplay::V2_0::sdm::PictureAdjustment;
using ::vendor::lineage::livedisplay::V2_0::sdm::SDMController;
using ::vendor::lineage::livedisplay::V2_1::IPictureAdjustment;

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
