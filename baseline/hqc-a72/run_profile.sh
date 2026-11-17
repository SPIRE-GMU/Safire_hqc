#!/bin/sh
# Runs on the VCK190 (aarch64 Linux). Normally started by vck190-hqc-profile.run, which unpacks
# this folder into /tmp and sets OUTDIR to the folder the user ran it from.
# Order: correctness checks for all three sets, then one-multiplication profiles (the numbers the
# AIE designs are compared with), then the KEM profiles. Each step prints its exit status and time.
set -u
HERE=$(cd "$(dirname "$0")" && pwd)
N=${N:-2000}; WARM=${WARM:-100}; ROUNDS=${ROUNDS:-1000}
OUTDIR=${OUTDIR:-$(pwd)}

host=$(hostname 2>/dev/null) || host=
[ -n "$host" ] || host=$(uname -n 2>/dev/null) || host=
case "$host" in ''|*[!A-Za-z0-9._-]*) host=board ;; esac
stamp=$(date +%Y%m%d-%H%M%S 2>/dev/null) || stamp=
case "$stamp" in ????????-??????) ;; *) stamp=unknown-time ;; esac
OUT="$OUTDIR/vck190-hqc-profile-$host-$stamp.txt"
if ! (set -C; : > "$OUT") 2>/dev/null; then
    OUT="/tmp/vck190-hqc-profile-$host-$stamp.txt"
    (set -C; : > "$OUT") 2>/dev/null || OUT=
fi
CSV="$HERE/all.csv"
echo "kind,set,opt,stage,cat,samples,min_ticks,p25_ticks,median_ticks,p75_ticks,mean_ticks,timer_ghz" > "$CSV"

step() {
    label=$1; shift
    t0=$(date +%s 2>/dev/null) || t0=0
    echo
    echo "===== $label   (start $(date '+%Y-%m-%d %H:%M:%S' 2>/dev/null))"
    "$@" 2>&1
    rc=$?
    t1=$(date +%s 2>/dev/null) || t1=0
    echo "----- $label   exit=$rc elapsed_s=$((t1 - t0))"
}

show() { [ -r "$1" ] && { echo "--- $1"; cat "$1"; }; return 0; }

main() {
    echo "package=vck190-hqc-profile-v1"
    echo "N=$N WARM=$WARM ROUNDS=$ROUNDS"
    echo "result_file=${OUT:-none}"
    echo "===== build"
    cat "$HERE/BUILDINFO.txt"
    echo "===== system"
    date
    uname -a
    show /etc/os-release
    show /proc/version
    show /proc/cpuinfo
    [ -r /proc/device-tree/model ] && { echo "--- device-tree model"; tr -d '\000' < /proc/device-tree/model; echo; }
    for c in /sys/devices/system/cpu/cpu[0-9]*; do
        for f in scaling_driver scaling_governor scaling_cur_freq cpuinfo_min_freq cpuinfo_max_freq; do
            [ -r "$c/cpufreq/$f" ] && echo "$c/cpufreq/$f: $(cat "$c/cpufreq/$f")"
        done
    done
    show /sys/devices/system/clocksource/clocksource0/current_clocksource
    show /proc/sys/kernel/perf_event_paranoid
    show /proc/loadavg
    head -n 5 /proc/meminfo 2>/dev/null

    for v in 128 192 256; do
        step "hqc-$v kem_check rounds=$ROUNDS" "$HERE/bin/kem_check_hqc-$v" "$ROUNDS"
        step "hqc-$v vectmul_check" "$HERE/bin/vectmul_check_hqc-$v"
    done
    for v in 128 192 256; do
        step "hqc-$v prof_vectmul N=$N WARM=$WARM" "$HERE/bin/prof_vectmul_hqc-$v" "$N" "$WARM" "$CSV"
    done
    for v in 128 192 256; do
        step "hqc-$v prof_kem N=$N WARM=$WARM" "$HERE/bin/prof_kem_hqc-$v" "$N" "$WARM" "$CSV"
    done
    echo
    echo "===== all.csv"
    cat "$CSV"
    echo "===== end $(date 2>/dev/null)"
}

if [ -n "$OUT" ]; then
    main 2>&1 | tee -a "$OUT"
    echo
    echo "== done. Bring back this file: $OUT"
else
    main 2>&1
    echo
    echo "== done. No result file could be written; please bring back the screen output."
fi
