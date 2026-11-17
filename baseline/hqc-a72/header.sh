#!/bin/sh
# vck190-hqc-profile: profile of the FAFFT-based HQC C code on the VCK190's Arm Cortex-A72 (aarch64 Linux).
# Usage, on the board:   sh vck190-hqc-profile.run
# It unpacks itself into a temporary folder under /tmp, runs the correctness checks and the profiles,
# writes ONE result file into the current folder (or /tmp if that is not writable), and removes the
# temporary folder. No root, no network, no system changes.
set -u
PAYLOAD_SHA256=@PAYLOAD_SHA256@
arch=$(uname -m 2>/dev/null) || arch=unknown
if [ "$arch" != "aarch64" ] && [ -z "${VCK190_SKIP_ARCH_CHECK:-}" ]; then
    echo "ERROR: this file must run on the VCK190 board itself (64-bit Arm, aarch64)."
    echo "       This computer is: $(uname -a 2>/dev/null)"
    echo "       Copy the file to the board first (e.g. scp it to /tmp on the board), log in to the board, run it there."
    exit 1
fi
ORIG=$(pwd)
SELF=$0
case "$SELF" in /*) ;; *) SELF="$ORIG/$SELF" ;; esac
TMP=$(mktemp -d /tmp/vck190-hqc-profile.XXXXXX 2>/dev/null) || {
    TMP=/tmp/vck190-hqc-profile.$$
    mkdir -p "$TMP" || { echo "ERROR: cannot create a folder in /tmp"; exit 1; }
}
LINE=$(awk '/^__VCK190_PAYLOAD__$/ { print NR + 1; exit }' "$SELF")
if [ -z "$LINE" ]; then echo "ERROR: payload marker not found"; rm -rf "$TMP"; exit 1; fi
if command -v sha256sum >/dev/null 2>&1; then
    got=$(tail -n +"$LINE" "$SELF" | sha256sum | cut -d' ' -f1)
    if [ "$got" != "$PAYLOAD_SHA256" ]; then
        echo "ERROR: this file was damaged in transfer (payload sha256 $got, expected $PAYLOAD_SHA256)"
        rm -rf "$TMP"; exit 1
    fi
    echo "payload sha256 ok"
else
    echo "sha256sum not found; skipping the integrity check"
fi
tail -n +"$LINE" "$SELF" | tar xzf - -C "$TMP" || { echo "ERROR: unpack failed"; rm -rf "$TMP"; exit 1; }
OUTDIR="$ORIG" sh "$TMP/vck190-hqc-profile/run_profile.sh"
rc=$?
rm -rf "$TMP"
exit $rc
__VCK190_PAYLOAD__
