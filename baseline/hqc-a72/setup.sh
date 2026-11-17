#!/bin/sh
# Fetch the C code the baseline is built from, at pinned upstream commits. Needs curl and tar.
#   src/hqc-{128,192,256}/  FAFFT_HQC crypto_kem/hqc-*/m4f at commit 16d6773f, with the _M4_ASM_
#                           define removed from parameters.h (pure C mode; the only change)
#   src/common/fips202.{c,h}  PQClean common/fips202 (SHAKE/SHA-3) at commit 719b5093
# Nothing else in this directory is touched. Run ./build_arm.sh afterwards.
set -eu
cd "$(dirname "$0")"
HQC_COMMIT=16d6773f8a1a7c25893fe171ead386172fc86260
PQCLEAN_COMMIT=719b50933e52219a72b365a820b993da259f5c6c
mkdir -p upstream src/common
tgz=upstream/FAFFT_HQC-$HQC_COMMIT.tar.gz
[ -f "$tgz" ] || curl -fsSL -o "$tgz" "https://github.com/myhoon/FAFFT_HQC/archive/$HQC_COMMIT.tar.gz"
rm -rf "upstream/FAFFT_HQC-$HQC_COMMIT"
tar -xzf "$tgz" -C upstream
for v in 128 192 256; do
    rm -rf "src/hqc-$v"
    mkdir -p "src/hqc-$v"
    cp "upstream/FAFFT_HQC-$HQC_COMMIT/crypto_kem/hqc-$v/m4f/"* "src/hqc-$v/"
    grep -q '^#define _M4_ASM_' "src/hqc-$v/parameters.h"
    sed '/^#define _M4_ASM_/d' "src/hqc-$v/parameters.h" > "src/hqc-$v/parameters.h.tmp"
    mv "src/hqc-$v/parameters.h.tmp" "src/hqc-$v/parameters.h"
    echo "src/hqc-$v: $(ls "src/hqc-$v" | wc -l | tr -d ' ') files, _M4_ASM_ removed"
done
for f in fips202.c fips202.h; do
    curl -fsSL -o "src/common/$f" "https://raw.githubusercontent.com/PQClean/PQClean/$PQCLEAN_COMMIT/common/$f"
    echo "src/common/$f: $(wc -c < "src/common/$f" | tr -d ' ') bytes"
done
echo "setup done"
