#!/usr/bin/env python3
"""Collect the design-1 aiesimulator results (latency.json, check logs, profiles) into one table
against the ARM A72 baseline (tiles, latency, throughput = n / latency, speedup).
Usage: python3 collect.py fetch   -> scp the result files of a remote build tree into raw/
                                    (KARA_REMOTE=user@host, KARA_REMOTE_DIR=path of src/karatsuba there)
       python3 collect.py table   -> print Markdown + write table.md and table.csv"""
import json, os, re, subprocess, sys, pathlib, statistics
HERE = pathlib.Path(__file__).resolve().parent
RAW = HERE / "raw"
REMOTE = os.environ.get("KARA_REMOTE")
RBASE = os.environ.get("KARA_REMOTE_DIR", "Jetcas2027/src/karatsuba")
SSH = ["ssh", "-o", "BatchMode=yes", "-o", "ConnectTimeout=20", "-o", "ControlMaster=no", "-o", "ControlPath=none"]
N = {"hqc1": 17669, "hqc3": 35851, "hqc5": 57637}
ARM_US = {"hqc1": 506.32, "hqc3": 1133.31, "hqc5": 1143.33}   # VCK190 A72 1.4 GHz, one vect_mul, board-measured baseline
CONFIGS = [f"{c}_L{L}_t{t}" for c, L, ts in (("hqc1", 288, (0, 2, 3, 4, 5)), ("hqc3", 288, (2, 3, 4, 5)), ("hqc5", 480, (2, 3, 4, 5))) for t in ts]

def fetch():
    if not REMOTE:
        sys.exit("set KARA_REMOTE=user@host (and KARA_REMOTE_DIR) to fetch from a remote build tree")
    RAW.mkdir(exist_ok=True)
    # one ssh: tar the small result files
    pats = " ".join(f"{c}/aie/results*/latency*.json {c}/aie/aiesimulator_output*/profile_funct_*.txt {c}/aie/Work*/reports/*mapping_analysis_report.txt" for c in CONFIGS)
    cmd = SSH + [REMOTE, f"cd {RBASE} && tar czf - {pats} 2>/dev/null"]
    data = subprocess.run(cmd, capture_output=True).stdout
    (RAW / "results.tgz").write_bytes(data)
    subprocess.run(["tar", "xzf", str(RAW / "results.tgz"), "-C", str(RAW)], check=True)
    print("fetched", len(data), "bytes")

def tiles_from_mapping(cfg):
    """count distinct tiles used (core or memory) from the mapping report; None if absent"""
    for rep in (RAW / cfg / "aie").glob("Work*/reports/*mapping_analysis_report.txt"):
        txt = rep.read_text(errors="replace")
        tiles = set(re.findall(r"\b(?:CR|MG)\((\d+),(\d+)\)", txt))
        if tiles: return len(tiles)
    return None

def table():
    rows = []
    for cfg in CONFIGS:
        case, L, t = cfg.split("_"); L = int(L[1:]); t = int(t[1:]); n = N[case]
        lat = None; latf = None
        for f in sorted((RAW / cfg / "aie").glob("results*/latency*.json")):
            j = json.loads(f.read_text())
            if j.get("status") == "ok": lat, latf = j, f
        status = "run" if lat else "not run"
        kernels = (3**t + 1) if t else 1
        tiles = tiles_from_mapping(cfg) or kernels
        if lat:
            us = lat["latency_ns"] / 1000.0; mbps = n / us
            interval = lat["last_output_intervals_ns"][0] / 1000.0 if lat.get("last_output_intervals_ns") else None
            arm = ARM_US.get(case); speed = arm / us if arm else None
            rows.append((cfg, case, L, t, kernels, tiles, us, interval, mbps, speed, str(latf.relative_to(RAW))))
        else:
            rows.append((cfg, case, L, t, kernels, tiles, None, None, None, None, status))
    md = ["| config | n | L | t | kernels | tiles | latency µs | frame interval µs | n/latency Mbps | vs ARM A72 | source |", "|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|"]
    csv = ["config,n,L,t,kernels,tiles,latency_us,interval_us,mbps,speedup_vs_arm,source"]
    for r in rows:
        cfg, case, L, t, k, tiles, us, iv, mbps, sp, src = r
        f = lambda x, d=2: ("%.*f" % (d, x)) if isinstance(x, float) else ("" if x is None else str(x))
        md.append(f"| {cfg} | {N[case]} | {L} | {t} | {k} | {tiles} | {f(us)} | {f(iv)} | {f(mbps,1)} | {f(sp,2)+'×' if sp else ''} | {src} |")
        # the CSV keeps full precision so that make_table.py rounds from the raw values, not from already rounded ones
        csv.append(",".join([cfg, str(N[case]), str(L), str(t), str(k), str(tiles), f(us,6), f(iv,6), f(mbps,6), f(sp,6), src]))
    md.append(""); md.append("ARM A72 1.4 GHz (measured on the VCK190 board, the adopted baseline): one vect_mul takes 506.32 µs for HQC-1 (34.9 Mbps), 1133.31 µs for HQC-3 (31.6 Mbps) and 1143.33 µs for HQC-5 (50.4 Mbps).")
    md.append("latency = time stamp of the last output word of the first multiplication from the start of the simulation (aiesimulator, 0.8 ns per cycle); Mbps = n / latency; tiles = core and memory tiles used according to the mapping report (the kernel count when there is no report).")
    out = "\n".join(md); print(out)
    (HERE / "table.md").write_text(out + "\n"); (HERE / "table.csv").write_text("\n".join(csv) + "\n")

if __name__ == "__main__":
    {"fetch": fetch, "table": table}[sys.argv[1]]()
