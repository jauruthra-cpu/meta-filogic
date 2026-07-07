# SPDX-License-Identifier: GPL-2.0-only
#
# Copyright (C) 2026 MediaTek Inc.
#

addtask offline_sign after do_compile before do_install

python () {
    if d.getVar('OPTEE_OFFLINE_SIGN') != '1':
        d.setVarFlag('do_offline_sign', 'noexec', '1')
}

TA_DEV_KIT_DIR = "${STAGING_DIR_HOST}${libdir}/export-ta_arm64"

offline_sign() {
	local ta_path="$1"
	local build_dir="$2"
	local public_key="$3"
	local private_key="$4"
	local ta_uuid=$(echo ${ta_path} | grep -Eo '[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{12}')
	local ta_dir=$(dirname ${ta_path})
	local sign=""

	if [ "${PN}" = "optee-os-tadevkit" ]; then
		sign="${S}/scripts/sign_encrypt.py"
	else
		sign="${TA_DEV_KIT_DIR}/scripts/sign_encrypt.py"
	fi

	#gen dig
	python3 ${sign} digest --key ${public_key} --uuid ${ta_uuid} --in ${ta_path} --dig ${ta_dir}/${ta_uuid}.dig

	#gen sig, please hook this command line
	base64 --decode ${ta_dir}/${ta_uuid}.dig | \
	openssl pkeyutl -sign -inkey ${private_key} \
	-pkeyopt digest:sha256 -pkeyopt rsa_padding_mode:pss \
	-pkeyopt rsa_pss_saltlen:digest -pkeyopt rsa_mgf1_md:sha256 | \
	base64 > ${ta_dir}/${ta_uuid}.sig

	python3 ${sign} stitch --key ${public_key} --uuid ${ta_uuid} \
		--in ${ta_path} --sig ${ta_dir}/${ta_uuid}.sig --out ${ta_dir}/${ta_uuid}.ta
}

do_offline_sign() {
	local build_dir=${S}
	local public_key=${TA_PUBLIC_KEY}
	local private_key=${TA_PRIVATE_KEY}

	for ta in $(find ${build_dir} | grep stripped.elf)
	do
		offline_sign ${ta} ${build_dir} ${public_key} ${private_key}
	done
}
