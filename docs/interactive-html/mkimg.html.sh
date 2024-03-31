profile_html() {
    profile_standard
    kernel_cmdline="unionfs_size=512M console=tty0 console=ttyS0,115200"
    syslinux_serial="0 115200"
    # podman and docker would be nice, but they're not supported in i386.
    apks="$apks
        build-base bash python3
        lighttpd curl wget links drill
        tmux vim nano
        "
}

# Using the known layout in the alpine.Dockerfile,
#   Copy the local files into where they need to go.
build_source() {
    >&2 echo "Adding precision-shell to the image."
    mkdir -p "${DESTDIR}/git"
    mkdir -p "${DESTDIR}/var/www/localhost"
    mkdir -p "${DESTDIR}/etc"
    mkdir -p "${DESTDIR}/etc/network"
    mkdir -p "${DESTDIR}/etc/initcpio/hooks"
    mkdir -p "${DESTDIR}/etc/initcpio/install"
    >&2 echo "/home/build/precision-shell ${DESTDIR}/git/."
    cp  -R /home/build/precision-shell "${DESTDIR}/git" || exit 1
    cp /home/build/interactive-html/ping.html "${DESTDIR}/var/www/localhost/index.html" || exit 1
    cp /home/build/interactive-html/motd.txt "${DESTDIR}/etc/motd" || exit 1
    cp /home/build/interactive-html/hostname.txt "${DESTDIR}/etc/hostname" || exit 1
    cp /home/build/interactive-html/resolv.conf "${DESTDIR}/etc/resolv.conf" || exit 1
    cp /home/build/interactive-html/network-interfaces.txt "${DESTDIR}/etc/network/interfaces" || exit 1
    cp /home/build/interactive-html/9p_root-hook.sh "${DESTDIR}/etc/initcpio/hooks/9p_root" || exit 1
    cp /home/build/interactive-html/9p_root-install.sh "${DESTDIR}/etc/initcpio/install/9p_root" || exit 1
}

section_source() {
    build_section source $ARCH
}
