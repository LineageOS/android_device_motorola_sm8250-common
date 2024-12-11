#!/usr/bin/env -S PYTHONPATH=../../../tools/extract-utils python3
#
# SPDX-FileCopyrightText: 2024 The LineageOS Project
# SPDX-License-Identifier: Apache-2.0
#
from extract_utils.fixups_blob import (
    blob_fixup,
    blob_fixups_user_type,
)
from extract_utils.fixups_lib import (
    lib_fixup_remove,
    lib_fixup_remove_arch_suffix,
    lib_fixup_vendorcompat,
    lib_fixups_user_type,
    libs_clang_rt_ubsan,
    libs_proto_3_9_1,
)
from extract_utils.main import (
    ExtractUtils,
    ExtractUtilsModule,
)

namespace_imports = [
    'device/motorola/sm8250-common',
    'hardware/qcom-caf/sm8250',
    'hardware/qcom-caf/wlan',
    'vendor/qcom/opensource/display',
    'vendor/qcom/opensource/commonsys/display',
    'vendor/qcom/opensource/commonsys-intf/display',
    'vendor/qcom/opensource/dataservices',
]

libs_add_vendor_suffix = (
    'com.qualcomm.qti.dpm.api@1.0',
    'libmmosal',
    'vendor.qti.hardware.wifidisplaysession@1.0',
    'vendor.qti.imsrtpservice@3.0',
)

libs_remove = (
    'libqsap_sdk',
    'libril',
    'libwpa_client',
)

def lib_fixup_vendor_suffix(lib: str, partition: str, *args, **kwargs):
    return f'{lib}-{partition}' if partition == 'vendor' else None

lib_fixups: lib_fixups_user_type = {
    libs_clang_rt_ubsan: lib_fixup_remove_arch_suffix,
    libs_proto_3_9_1: lib_fixup_vendorcompat,
    libs_add_vendor_suffix: lib_fixup_vendor_suffix,
    libs_remove: lib_fixup_remove,
}

blob_fixups: blob_fixups_user_type = {
    ('system_ext/lib/libwfdmmsrc_system.so', 'system_ext/lib64/libwfdmmsrc_system.so'): blob_fixup()
        .add_needed('libgui_shim.so'),
    'system_ext/lib64/libwfdnative.so': blob_fixup()
        .remove_needed('android.hidl.base@1.0.so')
        .add_needed('libbinder_shim.so')
        .add_needed('libinput_shim.so'),
    'system_ext/lib/libwfdservice.so': blob_fixup()
        .replace_needed('android.media.audio.common.types-V2-cpp.so', 'android.media.audio.common.types-V4-cpp.so'),
    'system_ext/etc/permissions/moto-telephony.xml': blob_fixup()
        .regex_replace('system', 'system_ext'),
    ('vendor/bin/thermal-engine', 'vendor/bin/rmt_storage', 'vendor/lib64/libril-qc-hal-qmi.so'): blob_fixup()
        .binary_regex_replace(b'ro.mot.build.customerid', b'vendor.build.customerid'),
    'vendor/etc/init/vendor.qti.hardware.alarm@1.0-service.rc': blob_fixup()
        .regex_replace('    disabled', ''),
    'vendor/lib64/libwvhidl.so': blob_fixup()
        .add_needed('libcrypto_shim.so'),
}

module = ExtractUtilsModule(
    'sm8250-common',
    'motorola',
    blob_fixups=blob_fixups,
    lib_fixups=lib_fixups,
    namespace_imports=namespace_imports,
)

if __name__ == '__main__':
    utils = ExtractUtils.device(module)
    utils.run()
