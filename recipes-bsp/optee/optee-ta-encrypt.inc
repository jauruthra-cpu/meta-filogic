# SPDX-License-Identifier: GPL-2.0-only
#
# Copyright (C) 2026 MediaTek Inc.
#

inherit mtk_key_derive

# Please set FW_ENC_ROE_KEYNAME becasue TA_ENC key derive from ROE KEY
OPTEE_TA_ENC ?= "0"
FW_ENC_PLAT_KEYNAME ?= "plat_key"
FW_ENC_ROE_KEYNAME ?= "roe_key"
FW_ENC_ROE_SALT ?= "salt/roe_salt.bin"
TA_KEY_SALT ?= "salt/ta_salt.bin"

python () {
	if d.getVar('OPTEE_TA_ENC') == '1':
		ta_key_path = d.getVar("OPTEE_KEYDIR")
		ta_key = "$(cat " + ta_key_path + "/ta_key.bin | od -x --endian=big | cut -c 9-50 | tr -d ' ' | tr -d '\n')";
		d.appendVar('EXTRA_OEMAKE', 'TA_ENC_KEY=' + ta_key)
		d.appendVar('EXTRA_OEMAKE', ' CFG_ENCRYPT_TA=y')
}

do_ta_key_derive() {
	hkdf_key_derive \
		"${OPTEE_KEYDIR}/${FW_ENC_PLAT_KEYNAME}.bin" \
		"${OPTEE_KEYDIR}/${FW_ENC_ROE_SALT}" \
		"${OPTEE_KEYDIR}/${FW_ENC_ROE_KEYNAME}.bin"

	hkdf_key_derive \
		"${OPTEE_KEYDIR}/${FW_ENC_ROE_KEYNAME}.bin" \
		"${OPTEE_KEYDIR}/${TA_KEY_SALT}" \
		"${OPTEE_KEYDIR}/ta_key.bin"
}

do_compile:prepend() {
	if [ "${OPTEE_TA_ENC}" -eq 1 ]; then
		do_ta_key_derive
	fi
}
