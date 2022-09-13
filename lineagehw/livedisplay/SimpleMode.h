/*
 * Copyright (C) 2022 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <livedisplay/sysfs/SimpleMode.h>
#include <vendor/lineage/livedisplay/2.1/IAntiFlicker.h>

namespace vendor {
namespace lineage {
namespace livedisplay {

// need 2.0 namespace for template specialization
namespace V2_0 {
namespace sysfs {

template <>
struct SimpleModeTrait<V2_1::IAntiFlicker> {
    static constexpr const char* kNode =
            "/sys/devices/platform/soc/soc:qcom,dsi-display-primary/dc";
};

}  // namespace sysfs
}  // namespace V2_0

namespace V2_1 {
namespace implementation {

using V2_0::sysfs::SimpleMode;

using AntiFlicker = SimpleMode<IAntiFlicker>;

}  // namespace implementation
}  // namespace V2_1

}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
