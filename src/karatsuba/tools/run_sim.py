#!/usr/bin/env python3
"""Keep simulator outputs separate using only the existing simulator arguments."""
import argparse
from pathlib import Path
import subprocess
import uuid

ROOT = Path.cwd()   # the project directory: make runs the tools from <config>/aie


def run(kind, work, output, root=ROOT):
    root = root.resolve()
    work = (root / work).resolve()
    output = (root / output).resolve()
    for path in (work, output):
        if root not in path.parents:
            raise ValueError("work and output must be strictly inside the project directory")
    if output == work or output in work.parents or work in output.parents:
        raise ValueError("output must be separate from the work directory")
    if not work.is_dir():
        raise ValueError(f"missing package directory: {work}")
    if output.exists():
        raise ValueError(f"preserve existing output by renaming it inside the project directory: {output}")
    default = root / ("x86simulator_output" if kind == "x86" else "aiesimulator_output")
    if work == default or default in work.parents or work in default.parents:
        raise ValueError("work must be separate from the simulator's default output")
    if output != default and (default in output.parents or output in default.parents):
        raise ValueError("custom output must not contain or be inside the default output")
    # The simulators write to fixed relative paths. Temporarily retain an older
    # default output, then restore it after moving the new output to its target.
    lock = root / (".t1-" + kind + "-simulation.lock")
    lock.mkdir()  # Reject concurrent runs that would share the default path.
    saved = None
    try:
        if default.exists():
            saved = root / (".t1-saved-" + kind + "-" + uuid.uuid4().hex)
            default.rename(saved)
            print(f"Temporarily preserved {default.name} at {saved.name}", flush=True)
        command = (["x86simulator", "--pkg-dir=" + str(work)] if kind == "x86" else
                   ["aiesimulator", "--pkg-dir=" + str(work), "--profile",
                    "--hang-detect-time=20000000"])  # 20 ms: the default 1 ms window closed a 7 ms single-tile run (2026-11-17)
        print("Running: " + " ".join(command), flush=True)
        rc = subprocess.run(command, cwd=root).returncode
        if rc == 0 and not default.is_dir():
            raise ValueError("simulator returned success without an output directory")
        return rc
    finally:
        # Preserve partial output on failure as well. Never merge it into an old
        # run, and do not delete the saved output if moving fails.
        try:
            if output != default and default.exists():
                output.parent.mkdir(parents=True, exist_ok=True)
                default.rename(output)
                print(f"Saved simulator output: {output}", flush=True)
            if saved is not None:
                saved.rename(default)
                print(f"Restored previous {default.name}", flush=True)
        finally:
            lock.rmdir()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--kind", choices=("x86", "hw"), required=True)
    parser.add_argument("--work", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    try:
        return run(args.kind, args.work, args.output)
    except (OSError, ValueError) as exc:
        print(f"ERROR: {exc}", flush=True)
        return 2


if __name__ == "__main__":
    raise SystemExit(main())
