cmd_libbb/run_shell.o := arm-none-linux-gnueabi-gcc -Wp,-MD,libbb/.run_shell.o.d   -std=gnu99 -Iinclude -Ilibbb  -I/home/lei-zhang/Desktop/at91sam9260/busybox-1.7.0/libbb -include include/autoconf.h -D_GNU_SOURCE -DNDEBUG -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -D"BB_VER=KBUILD_STR(1.7.0)" -DBB_BT=AUTOCONF_TIMESTAMP -D_FORTIFY_SOURCE=2  -Wall -Wshadow -Wwrite-strings -Wundef -Wstrict-prototypes -Wold-style-definition -Wmissing-prototypes -Wmissing-declarations -Os -fno-builtin-strlen -finline-limit=0 -fomit-frame-pointer -ffunction-sections -fdata-sections -fno-guess-branch-probability -funsigned-char -static-libgcc -falign-functions=1 -falign-jumps=1 -falign-labels=1 -falign-loops=1 -Wdeclaration-after-statement -Wno-pointer-sign    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(run_shell)"  -D"KBUILD_MODNAME=KBUILD_STR(run_shell)" -c -o libbb/run_shell.o libbb/run_shell.c

deps_libbb/run_shell.o := \
  libbb/run_shell.c \
    $(wildcard include/config/selinux.h) \
  include/libbb.h \
    $(wildcard include/config/locale/support.h) \
    $(wildcard include/config/feature/shadowpasswds.h) \
    $(wildcard include/config/lfs.h) \
    $(wildcard include/config/feature/buffers/go/on/stack.h) \
    $(wildcard include/config/buffer.h) \
    $(wildcard include/config/ubuffer.h) \
    $(wildcard include/config/feature/buffers/go/in/bss.h) \
    $(wildcard include/config/feature/ipv6.h) \
    $(wildcard include/config/ture/ipv6.h) \
    $(wildcard include/config/feature/prefer/applets.h) \
    $(wildcard include/config/busybox/exec/path.h) \
    $(wildcard include/config/getopt/long.h) \
    $(wildcard include/config/feature/pidfile.h) \
    $(wildcard include/config/feature/syslog.h) \
    $(wildcard include/config/route.h) \
    $(wildcard include/config/gunzip.h) \
    $(wildcard include/config/ktop.h) \
    $(wildcard include/config/ioctl/hex2str/error.h) \
    $(wildcard include/config/feature/editing.h) \
    $(wildcard include/config/feature/editing/history.h) \
    $(wildcard include/config/ture/editing/savehistory.h) \
    $(wildcard include/config/feature/editing/savehistory.h) \
    $(wildcard include/config/feature/tab/completion.h) \
    $(wildcard include/config/feature/username/completion.h) \
    $(wildcard include/config/feature/editing/vi.h) \
    $(wildcard include/config/inux.h) \
    $(wildcard include/config/feature/devfs.h) \
  include/platform.h \
    $(wildcard include/config/werror.h) \
    $(wildcard include/config///.h) \
    $(wildcard include/config//nommu.h) \
    $(wildcard include/config//mmu.h) \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/byteswap.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/byteswap.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/endian.h \
    $(wildcard include/config/.h) \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/features.h \
    $(wildcard include/config/c99.h) \
    $(wildcard include/config/c95.h) \
    $(wildcard include/config/ix.h) \
    $(wildcard include/config/ix2.h) \
    $(wildcard include/config/ix199309.h) \
    $(wildcard include/config/ix199506.h) \
    $(wildcard include/config/en.h) \
    $(wildcard include/config/en/extended.h) \
    $(wildcard include/config/x98.h) \
    $(wildcard include/config/en2k.h) \
    $(wildcard include/config/gefile.h) \
    $(wildcard include/config/gefile64.h) \
    $(wildcard include/config/e/offset64.h) \
    $(wildcard include/config/d.h) \
    $(wildcard include/config/c.h) \
    $(wildcard include/config/ile.h) \
    $(wildcard include/config/ntrant.h) \
    $(wildcard include/config/tify/level.h) \
    $(wildcard include/config/i.h) \
    $(wildcard include/config/ern/inlines.h) \
    $(wildcard include/config/ern/inlines/in/libc.h) \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/predefs.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/sys/cdefs.h \
    $(wildcard include/config/espaces.h) \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/wordsize.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/gnu/stubs.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/endian.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/arpa/inet.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/netinet/in.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/stdint.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/wchar.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/sys/socket.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/sys/uio.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/sys/types.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/types.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/typesizes.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/time.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../lib/gcc/arm-none-linux-gnueabi/4.3.3/include/stddef.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/sys/select.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/select.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/sigset.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/time.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/sys/sysmacros.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/pthreadtypes.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/uio.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/socket.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/sockaddr.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/asm/socket.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/asm/sockios.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/socket2.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/in.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../lib/gcc/arm-none-linux-gnueabi/4.3.3/include/stdbool.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/sys/mount.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/sys/ioctl.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/ioctls.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/asm/ioctls.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/asm/ioctl.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/asm-generic/ioctl.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/ioctl-types.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/sys/ttydefaults.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/ctype.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/xlocale.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/dirent.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/dirent.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/posix1_lim.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/local_lim.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/linux/limits.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/errno.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/errno.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/linux/errno.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/asm/errno.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/asm-generic/errno.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/asm-generic/errno-base.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/fcntl.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/fcntl.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/sys/stat.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/stat.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/fcntl2.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/inttypes.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/mntent.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/stdio.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/paths.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/netdb.h \
    $(wildcard include/config/3/ascii/rules.h) \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/rpc/netdb.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/siginfo.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/netdb.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/setjmp.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/setjmp.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/signal.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/signum.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/sigaction.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/sigcontext.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/asm/sigcontext.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/sigstack.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/sys/ucontext.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/sys/procfs.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/sys/time.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/sys/user.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/sigthread.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/libio.h \
    $(wildcard include/config/a.h) \
    $(wildcard include/config/ar/t.h) \
    $(wildcard include/config//io/file.h) \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/_G_config.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/wchar.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../lib/gcc/arm-none-linux-gnueabi/4.3.3/include/stdarg.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/stdio_lim.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/sys_errlist.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/stdio2.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/stdlib.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/waitflags.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/waitstatus.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/alloca.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/stdlib.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/string.h \
    $(wildcard include/config/ing/inlines.h) \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/string3.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/sys/mman.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/mman.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/sys/statfs.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/statfs.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/sys/wait.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/sys/resource.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/resource.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/termios.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/termios.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/unistd.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/posix_opt.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/environments.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/confname.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/getopt.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/unistd.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/utime.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/locale.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/locale.h \
  include/pwd_.h \
    $(wildcard include/config/use/bb/pwd/grp.h) \
  include/grp_.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../lib/gcc/arm-none-linux-gnueabi/4.3.3/include-fixed/limits.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../lib/gcc/arm-none-linux-gnueabi/4.3.3/include-fixed/syslimits.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/limits.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/posix2_lim.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/bits/xopen_lim.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/sys/param.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/linux/param.h \
  /home/lei-zhang/Desktop/at91sam9260/arm-2009q1/bin/../arm-none-linux-gnueabi/libc/usr/include/asm/param.h \
  include/xatonum.h \

libbb/run_shell.o: $(deps_libbb/run_shell.o)

$(deps_libbb/run_shell.o):
