"""Strict decimal PLIO reader, adapted from the existing FAFFT t1_io.py."""
from decimal import Decimal
from pathlib import Path
import json
import re

ROOT = Path.cwd()   # the project directory: make runs the tools from <config>/aie
TIMESTAMP = re.compile(r"T\s+([+]?(?:\d+(?:\.\d*)?|\.\d+)(?:[eE][+-]?\d+)?)\s+(ps|ns|us)\s*$")
SCALE_NS = {"ps": Decimal("0.001"), "ns": Decimal(1), "us": Decimal(1000)}


def output_path(path):
    path = Path(path)
    if path.is_file():
        return path
    return (path / "data" if (path / "data").is_dir() else path) / "out.txt"


def read_plio(path, timestamps=False):
    words, times = [], []
    current, previous = None, None
    count = 0
    for lineno, raw in enumerate(Path(path).read_text().splitlines(), 1):
        line = raw.strip()
        if not line or line.startswith("#"):
            continue
        if line.startswith("T"):
            if timestamps:
                match = TIMESTAMP.fullmatch(line)
                if not match:
                    raise ValueError(f"{path}:{lineno}: unsupported timestamp: {raw}")
                current = Decimal(match[1]) * SCALE_NS[match[2]]
                if previous is not None and current < previous:
                    raise ValueError(f"{path}:{lineno}: timestamps decreased")
                previous = current
                count += 1
            continue
        if not re.fullmatch(r"[+-]?\d+", line):
            raise ValueError(f"{path}:{lineno}: expected one decimal 32-bit word")
        value = int(line)
        if not -(1 << 31) <= value <= (1 << 32) - 1:
            raise ValueError(f"{path}:{lineno}: word outside 32-bit range")
        words.append(value & 0xffffffff)
        times.append(current)
    return words, times, count


def write_json(report, path):
    text = json.dumps(report, ensure_ascii=False, indent=2) + "\n"
    if path == "-":
        print(text, end="")
    elif path:
        destination = Path(path)
        destination.parent.mkdir(parents=True, exist_ok=True)
        destination.write_text(text)
