#!/usr/bin/env -S PYTHONPATH=../../../tools/extract-utils python3
#
# SPDX-FileCopyrightText: 2024 The LineageOS Project
# SPDX-License-Identifier: Apache-2.0
#
from extract_utils.fixups_blob import (
    blob_fixup,
    blob_fixups_user_type,
)
from extract_utils.main import (
    ExtractUtils,
    ExtractUtilsModule,
)
blob_fixups: blob_fixups_user_type = {
    ('system_ext/lib/libwfdmmsrc_system.so', 'system_ext/lib64/libwfdmmsrc_system.so'): blob_fixup()
        .add_needed('libgui_shim.so'),
    'system_ext/lib64/libwfdnative.so': blob_fixup()
        .remove_needed('android.hidl.base@1.0.so')
        .add_needed('libinput_shim.so'),
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
)
if __name__ == '__main__':
    utils = ExtractUtils.device(module)
    utils.run()
