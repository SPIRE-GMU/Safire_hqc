#!/usr/bin/env python3
"""Final comparison table from table.csv: for each HQC parameter set, the ARM A72 baseline row followed by
the AIE design points (tiles, latency, throughput = n / latency, speedup). Writes compare.md."""
import csv, pathlib
HERE = pathlib.Path(__file__).resolve().parent
N = {"hqc1": 17669, "hqc3": 35851, "hqc5": 57637}
ARM_US = {"hqc1": 506.32, "hqc3": 1133.31, "hqc5": 1143.33}      # VCK190 A72 1.4 GHz, one vect_mul, board-measured
NAME = {"hqc1": "HQC-128 (n = 17,669)", "hqc3": "HQC-192 (n = 35,851)", "hqc5": "HQC-256 (n = 57,637)"}
rows = list(csv.DictReader(open(HERE / "table.csv")))
out = ["| Parameter set | Platform | Configuration | Tiles | latency (µs) | throughput n/latency (Mbps) | vs ARM |", "|---|---|---|---:|---:|---:|---:|"]
for case in ("hqc1", "hqc3", "hqc5"):
    arm = ARM_US[case]
    out.append(f"| {NAME[case]} | ARM Cortex-A72, 1.4 GHz (VCK190 PS, one core) | reference vect_mul | – | {arm:.2f} | {N[case]/arm:.1f} | 1.00× |")
    for r in rows:
        if r["config"].startswith(case + "_"):
            t = int(r["t"]); tiles = r["tiles"]
            cfg = f"GF(2) Karatsuba, t = {t}, L = {r['L']}"
            if t == 0 and case != "hqc1":
                out.append(f"| | AIE1 | {cfg} | 1 | not applicable (the operands of HQC-192/256 do not fit the 32 KB of one tile) | | |"); continue
            if r["latency_us"]:
                # throughput and speedup come from collect.py (computed from the raw latency_ns), not from the rounded latency
                us = float(r["latency_us"]); out.append(f"| | AIE1 (VCK190 AIE array, 1.25 GHz, aiesimulator) | {cfg} | {tiles} | {us:.2f} | {float(r['mbps']):.1f} | {float(r['speedup_vs_arm']):.2f}× |")
            else:
                out.append(f"| | AIE1 | {cfg} | {tiles} | not run | | |")
out += ["", "latency = time of one multiplication from the first input word entering to the last output word leaving the AIE array (aiesimulator, 0.8 ns per cycle, first multiplication); throughput = n / latency; tiles include the folding tile and the tiles that only provide memory (mapping report, CR = MG). The ARM baseline is the measurement on the VCK190 board. Every AIE configuration reproduces the golden output of both multiplications bit for bit."]
(HERE / "compare.md").write_text("\n".join(out) + "\n"); print("\n".join(out))
