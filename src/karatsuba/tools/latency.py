#!/usr/bin/env python3
"""Report first-frame elapsed latency and subsequent last-output intervals."""
import argparse
import json
from pathlib import Path
import sys
from plio import ROOT, output_path, read_plio, write_json


def summarize(path, config):
    words, times, count = read_plio(path, timestamps=True)
    frame, iterations = config["frame_words"], config["iterations"]
    if len(words) != frame * iterations:
        raise ValueError(f"output length {len(words)} != {frame * iterations}")
    result = {"file": str(path), "words": len(words), "timestamp_lines": count,
              "timestamp_semantics": "T lines describe subsequent words until the next T line",
              "status": "ok" if count else "no_timestamps"}
    if not count:
        result["note"] = "No timestamps (expected for x86sim); latency and Mbps unavailable."
        return result
    if any(t is None for t in times):
        raise ValueError("output words precede the first timestamp")
    lasts = [times[(i + 1) * frame - 1] for i in range(iterations)]
    latency = lasts[0]
    if latency <= 0:
        raise ValueError("first-frame latency must be positive")
    result.update(iterations=[{"iteration": i + 1,
                               "first_output_ns": float(times[i * frame]),
                               "last_output_ns": float(lasts[i])} for i in range(iterations)],
                  latency_ns=float(latency),
                  last_output_intervals_ns=[float(lasts[i] - lasts[i - 1])
                                             for i in range(1, iterations)],
                  throughput_Mbps=float(config["n"] * 1000 / latency),
                  throughput_formula="n * 1000 / first-frame latency_ns; intervals are reported separately")
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("output", type=Path)
    parser.add_argument("--json")
    args = parser.parse_args()
    try:
        report = summarize(output_path(args.output), json.loads((ROOT / "tools/project.json").read_text()))
        if report["status"] == "no_timestamps":
            print(report["note"])
        else:
            print(f"First multiplication latency: {report['latency_ns']:.9g} ns")
            print("Last-output intervals (ns): " + ", ".join(map(str, report["last_output_intervals_ns"])))
            print(f"Mbps = n / first multiplication latency: {report['throughput_Mbps']:.9g}")
        write_json(report, args.json)
    except (OSError, ValueError, KeyError) as exc:
        print(f"FAIL {exc}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
