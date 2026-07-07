# SPDX-License-Identifier: GPL-2.0-only
#
# Copyright (C) 2026 MediaTek Inc.
#

OPTEE_KEYDIR ?= "${SECURE_BOOT_KEYDIR_secureboot}"
TA_SIGN_KEYNAME ?= "default.pem"

TA_SIGN_KEY = "${OPTEE_KEYDIR}/${TA_SIGN_KEYNAME}"
TA_PUBLIC_KEY = "${OPTEE_KEYDIR}/${TA_SIGN_KEYNAME}"

# OFFLINE_SIGN enable.
# Set TA_PRIVATE_KEY that is hook key that use in command line and it's just for test.
# Please replace the command signing ta.
# Set TA_SIGN_KEY that is dummy key for build
# Set TA_PUBLIC_KEY that is public key embbedded in tee.bin
OPTEE_OFFLINE_SIGN ?= "0"
TA_PRIVATE_KEY = "${TA_PUBLIC_KEY}"

include optee-offline-sign.inc

EXTRA_OEMAKE += " \
	TA_SIGN_KEY=${TA_SIGN_KEY} \
	TA_PUBLIC_KEY=${TA_PUBLIC_KEY} \
"
