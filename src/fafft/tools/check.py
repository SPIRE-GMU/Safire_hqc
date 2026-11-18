#!/usr/bin/env python3
"""Compare the simulator output of one configuration with the expected product words.

usage: check.py <simulator output directory> --config <name>

Run from src/fafft. The output file is <directory>/data/<name>_out.txt (data/hqc3/ or data/hqc5/ for
the h3* and h5* configurations) and the expected words are in data/expect.txt (data/hqc3/expect.txt,
data/hqc5/expect.txt): three multiplications, one decimal 32-bit word per line. Lines that start with
T are aiesimulator time stamps and are skipped."""
import argparse
import sys
from pathlib import Path

FRAMES = 3


def case_of(config):
    """(data subdirectory, words per frame, n) of a configuration name."""
    if config.startswith("h3"):
        return "hqc3", 1122, 35851
    if config.startswith("h5"):
        return "hqc5", 1802, 57637
    return "", 554, 17669


def read_words(path):
    words = []
    for number, raw in enumerate(Path(path).read_text().splitlines(), 1):
        line = raw.strip()
        if not line or line.startswith(("T", "#")):
            continue
        try:
            value = int(line)
        except ValueError:
            raise SystemExit(f"{path}:{number}: expected one decimal word, found {raw!r}")
        words.append(value & 0xFFFFFFFF)
    return words


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("output", type=Path)
    parser.add_argument("--config", required=True)
    args = parser.parse_args()
    sub, frame, _ = case_of(args.config)
    actual = read_words(args.output / "data" / sub / f"{args.config}_out.txt")
    expected = read_words(Path("data") / sub / "expect.txt")
    ok = len(expected) == FRAMES * frame and len(actual) == len(expected)
    if not ok:
        print(f"length: output {len(actual)} words, expected {len(expected)} words, {FRAMES} frames of {frame}")
    for i in range(FRAMES):
        a, e = actual[i * frame:(i + 1) * frame], expected[i * frame:(i + 1) * frame]
        bad = next((k for k in range(frame) if k >= len(a) or k >= len(e) or a[k] != e[k]), None)
        if bad is None:
            print(f"multiplication {i + 1}: PASS ({frame} words)")
        else:
            ok = False
            got = f"0x{a[bad]:08x}" if bad < len(a) else "missing"
            want = f"0x{e[bad]:08x}" if bad < len(e) else "missing"
            print(f"multiplication {i + 1}: FAIL at word {bad}: got {got}, expected {want}")
    print("PASS" if ok else "FAIL", args.config)
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
