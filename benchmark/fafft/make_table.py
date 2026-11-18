#!/usr/bin/env python3
"""Comparison table of the FAFFT design points against the ARM A72 baseline.

Run `make latency CFG=<name>` in src/fafft for the configurations of interest, then this script. It
reads src/fafft/results/<name>/latency.json, counts the kernels of <name>/graph.h (one kernel per
tile; the mapping report of the hardware build lists the same number of cores) and prints tiles,
latency of the first multiplication, throughput = n / latency and the speedup over the A72. With an
argument it reads the results from that directory instead of src/fafft/results."""
import csv
import json
import pathlib
import re
import sys

HERE = pathlib.Path(__file__).resolve().parent
FAFFT = HERE.parents[1] / "src" / "fafft"
RESULTS = pathlib.Path(sys.argv[1]) if len(sys.argv) > 1 else FAFFT / "results"
CONFIGS = ["n16", "h3n16", "h5n16"]
ARM_US = {17669: 506.32, 35851: 1133.31, 57637: 1143.33}   # VCK190 A72 1.4 GHz, one vect_mul, board-measured
CASE = {17669: "HQC-1", 35851: "HQC-3", 57637: "HQC-5"}

rows = []
for name in CONFIGS:
    path = RESULTS / name / "latency.json"
    if not path.exists():
        continue
    report = json.loads(path.read_text())
    n, latency_us = report["n"], report["latency_ns"] / 1000
    tiles = len(re.findall(r"kernel::create\(", (FAFFT / name / "graph.h").read_text()))
    rows.append(dict(config=name, case=CASE[n], n=n, tiles=tiles, latency_us=round(latency_us, 2),
                     mbps=round(n / latency_us, 2), arm_us=ARM_US[n], speedup=round(ARM_US[n] / latency_us, 2),
                     interval_us=round(report["last_output_intervals_ns"][0] / 1000, 2)))
if not rows:
    sys.exit(f"no latency.json under {RESULTS}")
lines = ["| configuration | case | tiles | latency (us) | throughput (Mbps) | A72 (us) | speedup | interval (us) |",
         "|---|---|---:|---:|---:|---:|---:|---:|"]
lines += [f"| {r['config']} | {r['case']} | {r['tiles']} | {r['latency_us']:.2f} | {r['mbps']:.2f} | {r['arm_us']:.2f} | {r['speedup']:.2f} | {r['interval_us']:.2f} |" for r in rows]
print("\n".join(lines))
(HERE / "table.md").write_text("\n".join(lines) + "\n")
with open(HERE / "table.csv", "w", newline="") as f:
    writer = csv.DictWriter(f, fieldnames=list(rows[0]))
    writer.writeheader(); writer.writerows(rows)
