# SPDX-License-Identifier: GPL-2.0-only
#
# Copyright (C) 2026 MediaTek Inc.
#

bin2hex() {
	od -An -t x1 -w128 | sed "s/ //g"
}

hkdf_key_derive() {
	key=$(cat $1 | bin2hex)
	salt=$(cat $2 | bin2hex)
	out=$3

	openssl kdf \
		-keylen 32 -binary -out ${out} \
		-kdfopt digest:SHA2-256 \
		-kdfopt hexkey:$key \
		-kdfopt hexsalt:$salt HKDF
}
