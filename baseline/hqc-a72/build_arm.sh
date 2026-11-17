#!/bin/bash
# Cross-compile the HQC profiling harness for the VCK190's Cortex-A72 (aarch64 Linux, static) and
# package it as ONE self-extracting file, out/vck190-hqc-profile.run. Run from this directory after
# ./setup.sh, which puts the upstream sources in src/. Only harness/ is this project's code.
set -euo pipefail
cd "$(dirname "$0")"
W=$PWD
TC=${TC:-/tools/Xilinx/Vitis/2023.2/gnu/aarch64/lin/aarch64-linux/bin}
CC=${CC:-$TC/aarch64-linux-gnu-gcc}
AR=${AR:-$TC/aarch64-linux-gnu-ar}
OPT_NAME=${OPT_NAME:-A72}
OPT=${OPT:--O3 -mcpu=cortex-a72}
PKG=vck190-hqc-profile
B=$W/build
P=$W/out/$PKG

for v in 128 192 256; do
    [ -f "$W/src/hqc-$v/parameters.h" ] || { echo "src/hqc-$v is missing: run ./setup.sh first"; exit 1; }
done
[ -f "$W/src/common/fips202.c" ] || { echo "src/common/fips202.c is missing: run ./setup.sh first"; exit 1; }
rm -rf "$B" "$W/out"
mkdir -p "$B" "$P/bin"
DIAG=$W/out/compile-diagnostics.txt
: > "$DIAG"
trap 'echo "BUILD FAILED; compiler diagnostics:"; cat "$DIAG" 2>/dev/null' ERR

for v in 128 192 256; do
    D=$B/hqc-$v
    mkdir -p "$D"
    FLAGS="$OPT -std=c99 -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-but-set-variable -Wno-sign-compare -I$W/src/common -I$W/src/hqc-$v"
    for f in "$W"/src/hqc-$v/*.c "$W"/src/common/fips202.c "$W"/src/common/randombytes.c; do
        $CC $FLAGS -c "$f" -o "$D/$(basename "$f" .c).o" 2>> "$DIAG"
    done
    rm -f "$D/libhqc.a"
    $AR rcs "$D/libhqc.a" "$D"/*.o
    HF="$OPT -std=c99 -Wall -Wno-unused-function -Wno-unused-variable -I$W/src/common -I$W/src/hqc-$v -DHQCNS=PQCLEAN_HQC${v}_CLEAN_ -DSET_NAME=\"hqc-$v\" -DOPT_NAME=\"$OPT_NAME\""
    for p in kem_check vectmul_check prof_kem prof_vectmul; do
        EXTRA=""
        [ $p = kem_check ] && EXTRA="-Wl,--wrap=PQCLEAN_HQC${v}_CLEAN_vect_mul"
        $CC $HF -static -s -o "$P/bin/${p}_hqc-$v" "$W/harness/$p.c" "$D/libhqc.a" $EXTRA 2>> "$DIAG"
    done
done
echo "compile diagnostics: $(wc -l < "$DIAG") lines (full text in out/compile-diagnostics.txt)"

bad=$(file "$P"/bin/* | grep -v 'ARM aarch64.*statically linked' || true)
if [ -n "$bad" ]; then echo "ERROR: not static aarch64:"; echo "$bad"; exit 1; fi
file "$P"/bin/* | sed 's/, BuildID[^,]*//'

cp "$W/run_profile.sh" "$P/run_profile.sh"
chmod +x "$P/run_profile.sh" "$P"/bin/*
{
    echo "package=$PKG-v1"
    echo "build_utc=$(date -u '+%Y-%m-%dT%H:%M:%SZ')"
    echo "build_host=$(uname -n) $(uname -r)"
    echo "cc=$CC"
    echo "cc_version=$($CC --version | sed -n 1p)"
    echo "opt_name=$OPT_NAME"
    echo "opt=$OPT"
    echo "lib_flags=-std=c99 -Wall -Wextra"
    echo "link=-static -s"
    echo "source=myhoon/FAFFT_HQC 16d6773f crypto_kem/hqc-{128,192,256}/m4f, pure C (_M4_ASM_ removed), fetched by setup.sh"
    echo "harness_sha256:"
    (cd "$W/harness" && sha256sum common.h kem_check.c vectmul_check.c prof_kem.c prof_vectmul.c)
    echo "src_tree_sha256=$(cd "$W/src" && find . -type f | LC_ALL=C sort | xargs sha256sum | sha256sum | cut -d' ' -f1)"
} > "$P/BUILDINFO.txt"
(cd "$P" && sha256sum bin/* run_profile.sh BUILDINFO.txt > SHA256SUMS)

tar -C "$W/out" -czf "$W/out/payload.tar.gz" "$PKG"
sum=$(sha256sum "$W/out/payload.tar.gz" | cut -d' ' -f1)
sed "s/@PAYLOAD_SHA256@/$sum/" "$W/header.sh" > "$W/out/$PKG.run"
cat "$W/out/payload.tar.gz" >> "$W/out/$PKG.run"
chmod +x "$W/out/$PKG.run"
(cd "$W/out" && sha256sum "$PKG.run" > "$PKG.run.sha256")
ls -l "$W/out/$PKG.run"
cat "$W/out/$PKG.run.sha256"
