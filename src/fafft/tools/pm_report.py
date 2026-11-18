#!/usr/bin/env python3
"""Program and data memory of every tile, read from the linker maps of a hardware build.

usage: pm_report.py --work Work_hw_<name> [--out results/<name>/pm_report.txt]

Every tile has a map file <work>/aie/<column>_<row>/Release/<column>_<row>.map. "Used" of the memory
PM is the program size in bytes (limit 16,384 per tile), "Used" of the memory DMb the data size in
bytes including the 4,096-byte stack (limit 32,768 per tile)."""
import argparse
import re
import sys
from pathlib import Path

LIMIT = {"PM": 16384, "DMb": 32768}


def used(text, memory):
    start = text.find(f"Memory map for memory '{memory}'")
    match = re.search(r"Used\s*=\s*(\d+)", text[start:start + 400]) if start >= 0 else None
    return int(match[1]) if match else None


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--work", type=Path, required=True)
    parser.add_argument("--out", type=Path)
    args = parser.parse_args()
    rows = []
    for path in sorted(args.work.glob("aie/*_*/Release/*.map")):
        text = path.read_text(errors="replace")
        # the kernel entry point is the C++ symbol _Z<length>k_<name>...; several tiles can run the same function
        kernel = next((m[2][:int(m[1])] for m in re.finditer(r"_Z(\d+)(k_[A-Za-z0-9_]+)", text)), "?")
        rows.append((kernel, path.parts[-3], used(text, "PM"), used(text, "DMb")))
    if not rows or any(pm is None or dm is None for _, _, pm, dm in rows):
        sys.exit(f"{args.work}: no map files, or a map without the PM / DMb summary")
    lines = ["kernel | tile | program bytes | data bytes"]
    lines += [f"{kernel} | {tile} | {pm} | {dm}" for kernel, tile, pm, dm in rows]
    largest_pm = max(rows, key=lambda r: r[2]); largest_dm = max(rows, key=lambda r: r[3])
    lines += [f"tiles: {len(rows)}",
              f"largest program: {largest_pm[2]} bytes ({largest_pm[0]}, tile {largest_pm[1]}), limit {LIMIT['PM']}",
              f"largest data: {largest_dm[3]} bytes ({largest_dm[0]}, tile {largest_dm[1]}), limit {LIMIT['DMb']}"]
    print("\n".join(lines[-3:]))
    if args.out:
        args.out.parent.mkdir(parents=True, exist_ok=True)
        args.out.write_text("\n".join(lines) + "\n")
    return 0 if largest_pm[2] <= LIMIT["PM"] and largest_dm[3] <= LIMIT["DMb"] else 1


if __name__ == "__main__":
    sys.exit(main())
