FILESEXTRAPATHS:prepend := "${THISDIR}/files:"
SRC_URI:append = " file://init_readonlyfs.sh \
                   file://init_readonlyfs-emmc.sh \
                   file://kernelv6-init_readonlyfs.sh \
                   file://kernelv6-init_readonlyfs-emmc.sh \
                 "
FILES:${PN} += " /overlay \
               /rom \"

do_install:append() {
       if ${@bb.utils.contains('DISTRO_FEATURES','kernel_in_ubi','true','false',d)}; then
              install -d ${D}/overlay
              install -d ${D}/rom
              if ${@bb.utils.contains('DISTRO_FEATURES','kernelv6','true','false',d)}; then
                     if ${@bb.utils.contains('DISTRO_FEATURES','emmc','true','false',d)}; then
                            install -m 0755 ${UNPACKDIR}/kernelv6-init_readonlyfs-emmc.sh ${D}${rootlibexecdir}/init_readonlyfs.sh
                     else
                            install -m 0755 ${UNPACKDIR}/kernelv6-init_readonlyfs.sh ${D}${rootlibexecdir}/init_readonlyfs.sh
                     fi
              else
                     if ${@bb.utils.contains('DISTRO_FEATURES','emmc','true','false',d)}; then
                            install -m 0755 ${UNPACKDIR}/init_readonlyfs-emmc.sh ${D}${rootlibexecdir}/init_readonlyfs.sh
                     else
                            install -m 0755 ${UNPACKDIR}/init_readonlyfs.sh ${D}${rootlibexecdir}/init_readonlyfs.sh
                     fi
              fi
              if ${@bb.utils.contains('DISTRO_FEATURES', 'usrmerge', 'true', 'false', d)}; then
                     [ -e ${D}/usr/sbin/init ] && rm -rf ${D}/usr/sbin/init
                     ln -s ${rootlibexecdir}/init_readonlyfs.sh ${D}/usr/sbin/init
              else
                     [ -e ${D}/sbin/init ] && rm -rf ${D}/sbin/init
                     ln -s ${rootlibexecdir}/init_readonlyfs.sh ${D}/sbin/init
              fi
       fi
}

FILES:${PN} += " ${rootlibexecdir}/init_readonlyfs.sh "
FILES:${PN} += "${sbindir}"
INSANE_SKIP:${PN} += "installed-vs-shipped"
PACKAGECONFIG:remove = "vconsole"
