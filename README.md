# Dropbear All-in-One

Dropbear is a relatively small SSH server and client. It runs on a variety of POSIX-based platforms.
Dropbear is open source software, distributed under a MIT-style license.
Dropbear is particularly useful for "embedded"-type Linux (or other Unix) systems, such as wireless routers.

## Differences between this version and the official version

* The `scp` utility is integrated into the main module `dropbearmulti`.
  An example of running integrated `scp` utility: `./dropbearmulti scp -pf '/tmp/file.txt'`

* No external libraries required.

## Build toolchain

Download `musl` version of toolchain: [linux musl toolchain](https://toolchains.bootlin.com/)

## Build

```
git clone https://github.com/openwrt-xiaomi/dropbear.git

cd dropbear

TARGET_HOST=mipsel-buildroot-linux-musl
CROSS_TOOL=/home/<toolchain_dir>/bin/mipsel-buildroot-linux-musl

#TARGET_HOST=arm-buildroot-linux-musleabihf
#CROSS_TOOL=/home/<toolchain_dir>/bin/arm-buildroot-linux-musleabihf

#TARGET_HOST=aarch64-buildroot-linux-musl
#CROSS_TOOL=/home/<toolchain_dir>/bin/aarch64-buildroot-linux-musl

./configure --host=$TARGET_HOST --enable-static --disable-zlib --disable-harden \
  --disable-pam --enable-bundled-libtom --enable-openpty --enable-syslog \
  --disable-lastlog --disable-utmpx --disable-utmp --disable-wtmp --disable-wtmpx \
  --disable-loginfunc --disable-pututline --disable-pututxline \
  CC="$CROSS_TOOL-gcc" \
  CFLAGS="-Os -Wl,-static -ffunction-sections -fdata-sections" \
  LDFLAGS="-static -Wl,--gc-sections -Wl,--strip-all"

make PROGRAMS="dropbear scp dropbearkey" MULTI=1 STATIC=1
```


