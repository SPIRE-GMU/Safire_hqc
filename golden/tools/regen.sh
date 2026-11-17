#!/bin/sh
# Rebuild the golden dumper for HQC-1/3/5 against the HQC reference implementation
# (gitlab.com/pqc-hqc/hqc, tag v5.1.0) in $HQC_DIR, default golden/tools/upstream/hqc,
# and regenerate golden/hqc-{1,3,5}/. Needs only a C compiler. Nothing inside the
# clone is modified; the build goes to golden/tools/build/.
set -eu
HERE=$(cd "$(dirname "$0")" && pwd)
ROOT=$(cd "$HERE/../.." && pwd)
HQC=${HQC_DIR:-$HERE/upstream/hqc}
[ -d "$HQC/src/ref" ] || { echo "HQC reference not found at $HQC: git clone --branch v5.1.0 https://gitlab.com/pqc-hqc/hqc.git \"$HQC\" (or set HQC_DIR)"; exit 1; }
OUT="$ROOT/golden"
BUILD="$HERE/build"
CC=${CC:-cc}
mkdir -p "$BUILD"

for v in 1 3 5; do
    bin="$BUILD/dump_keygen_hqc$v"
    extra=""
    for f in "$HQC"/src/ref/hqc-$v/*.c "$HQC"/src/common/hqc-$v/*.c; do
        [ -e "$f" ] && extra="$extra $f"
    done
    # shellcheck disable=SC2086
    "$CC" -O2 -std=c11 -Wall -Wno-unused-function \
        -I"$HERE/shim" \
        -I"$HQC/src/common" -I"$HQC/src/common/hqc-$v" \
        -I"$HQC/src/ref" -I"$HQC/src/ref/hqc-$v" \
        -I"$HQC/lib/fips202" \
        "$HERE/dump_keygen.c" "$HQC"/src/common/*.c "$HQC"/src/ref/*.c $extra \
        -o "$bin"
    mkdir -p "$OUT/hqc-$v"
    echo "== HQC-$v -> $OUT/hqc-$v"
    "$bin" --out "$OUT/hqc-$v" \
        --intermediates "$HQC/kats/ref/hqc-$v/intermediates_values" \
        --kat "$HQC/kats/ref/hqc-$v/PQCkemKAT_32.rsp"
done
