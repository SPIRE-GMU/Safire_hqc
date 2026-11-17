#!/usr/bin/env python3
"""Read the decimal simulator output of the project in the current directory and compare it with data/expected.txt."""
import argparse
import importlib.util
import json
from pathlib import Path
import sys
from plio import ROOT, output_path, read_plio, write_json

sys.dont_write_bytecode = True


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("output", type=Path)
    parser.add_argument("--json")
    args = parser.parse_args()
    try:
        config = json.loads((ROOT / "tools/project.json").read_text())
        checker = (ROOT / config["checker_relative"]).resolve()
        spec = importlib.util.spec_from_file_location("task03_checker", checker)
        module = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(module)
        expected, _, _ = read_plio(ROOT / "data/expected.txt")
        frame = config["frame_words"]
        if len(expected) != config["iterations"] * frame:
            raise ValueError("expected.txt does not match the configured frame count")
        frames = [expected[i:i + frame] for i in range(0, len(expected), frame)]
        words, _, _ = read_plio(output_path(args.output))
        report = module.compare(words, frames, frame, repeat=1)
        print("PASS " + json.dumps(report, sort_keys=True))
        write_json(report, args.json)
    except (OSError, ValueError, KeyError) as exc:
        print(f"FAIL {exc}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
