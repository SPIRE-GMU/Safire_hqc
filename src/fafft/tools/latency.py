#!/usr/bin/env python3
"""Latency of the first multiplication from the aiesimulator output time stamps.

usage: latency.py <aiesimulator output directory> --config <name> [--json results/<name>/latency.json]

A line "T <time> <ps|ns|us>" in the output file stamps the data words that follow it. The latency is
the time stamp of the last output word of the first multiplication, counted from the start of the
simulation; throughput is n / latency. The intervals between the last words of successive
multiplications are reported as well."""
import argparse
import json
import re
import sys
from pathlib import Path

from check import FRAMES, case_of

STAMP = re.compile(r"T\s+([0-9.]+(?:[eE][+-]?[0-9]+)?)\s+(ps|ns|us)\s*$")
TO_NS = {"ps": 1e-3, "ns": 1.0, "us": 1e3}


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("output", type=Path)
    parser.add_argument("--config", required=True)
    parser.add_argument("--json", type=Path)
    args = parser.parse_args()
    sub, frame, n = case_of(args.config)
    path = args.output / "data" / sub / f"{args.config}_out.txt"
    times, now = [], None
    for raw in path.read_text().splitlines():
        line = raw.strip()
        match = STAMP.match(line)
        if match:
            now = float(match[1]) * TO_NS[match[2]]
        elif line and not line.startswith(("T", "#")):
            times.append(now)
    if len(times) != FRAMES * frame or any(t is None for t in times):
        sys.exit(f"{path}: {len(times)} stamped words, expected {FRAMES * frame} (x86simulator output has no time stamps)")
    last = [times[(i + 1) * frame - 1] for i in range(FRAMES)]
    report = {
        "config": args.config, "n": n, "frame_words": frame,
        "first_output_ns": [times[i * frame] for i in range(FRAMES)],
        "last_output_ns": last,
        "latency_ns": last[0],
        "last_output_intervals_ns": [round(last[i + 1] - last[i], 3) for i in range(FRAMES - 1)],
        "throughput_Mbps": n * 1000 / last[0],
    }
    print(f"{args.config}: latency of the first multiplication {last[0]:.3f} ns ({last[0] / 1000:.2f} us)")
    print(f"{args.config}: throughput = {n} / latency = {report['throughput_Mbps']:.2f} Mbps")
    print(f"{args.config}: intervals between successive results {report['last_output_intervals_ns']} ns")
    if args.json:
        args.json.parent.mkdir(parents=True, exist_ok=True)
        args.json.write_text(json.dumps(report, indent=2) + "\n")
    return 0


if __name__ == "__main__":
    sys.exit(main())
