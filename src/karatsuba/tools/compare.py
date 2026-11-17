#!/usr/bin/env python3
"""Compare packed hex output frames, ignoring blank, T and # lines.

Examples:
  python3 -B ../../tools/compare.py output.txt --expected data/expected.txt --frame-words 554

One expected frame automatically repeats when the output has multiple complete
frames. Multiple expected files/vectors define the exact sequence. --repeat N
repeats that sequence explicitly. Extra words within --frame-words must be zero.
"""
from __future__ import annotations

import argparse
import json
from pathlib import Path
import re
import sys

sys.dont_write_bytecode = True
HERE = Path(__file__).resolve().parent
HEXWORD = re.compile(r"(?:0[xX])?([0-9a-fA-F]{1,8})\Z")


def read_words(path: Path) -> list[int]:
    words = []
    for line_number, line in enumerate(path.read_text().splitlines(), 1):
        line = line.strip()
        if not line or line.startswith(("T", "#")):
            continue
        for token in line.split():
            match = HEXWORD.fullmatch(token)
            if not match:
                raise ValueError(f"{path}:{line_number}: invalid uint32 hex token {token!r}")
            words.append(int(match.group(1), 16))
    return words


def compare(output: list[int], expected: list[list[int]], frame_words: int,
            repeat: int | None = None) -> dict:
    if not expected or any(not frame for frame in expected):
        raise ValueError("expected frames must be nonempty")
    if frame_words <= 0 or any(len(frame) > frame_words for frame in expected):
        raise ValueError("frame-words must be positive and at least every expected frame length")
    if not output:
        raise ValueError("output has no data words")
    if len(output) % frame_words:
        raise ValueError(f"incomplete frame: output_words={len(output)} frame_words={frame_words} remainder={len(output) % frame_words}")
    actual_frames = len(output)//frame_words
    if repeat is None:
        repeat = actual_frames if len(expected) == 1 else 1
    if repeat <= 0:
        raise ValueError("repeat must be positive")
    wanted_frames = repeat * len(expected)
    if actual_frames != wanted_frames:
        raise ValueError(f"frame count mismatch: expected={wanted_frames} actual={actual_frames}")
    for frame_index in range(wanted_frames):
        wanted = expected[frame_index % len(expected)]
        for word_index in range(frame_words):
            expected_word = wanted[word_index] if word_index < len(wanted) else 0
            actual_word = output[frame_index*frame_words + word_index]
            difference = actual_word ^ expected_word
            if difference:
                bit = (difference & -difference).bit_length() - 1
                padding = " (extra padding word must be zero)" if word_index >= len(wanted) else ""
                raise ValueError(
                    f"mismatch: frame={frame_index} word={word_index} bit_in_word={bit} "
                    f"bit_in_frame={32*word_index+bit} stream_bit={32*(frame_index*frame_words+word_index)+bit} "
                    f"expected={expected_word:08x} actual={actual_word:08x}{padding}; all indices are zero-based")
    return {"status": "PASS", "frames": wanted_frames, "frame_words": frame_words,
            "compared_words": len(output), "extra_padding_words_per_sequence": sum(frame_words-len(frame) for frame in expected)}


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("output", type=Path)
    parser.add_argument("--expected", action="append", type=Path, help="expected file; may be repeated for distinct frames")
    parser.add_argument("--manifest", type=Path, default=HERE / "manifest.json")
    parser.add_argument("--case", choices=("hqc-1", "hqc-3", "hqc-5", "mini"))
    parser.add_argument("--vectors", nargs="+", help="expected vector sequence within --case")
    parser.add_argument("--kind", choices=("red", "lin", "a", "b"), default="red")
    parser.add_argument("--frame-words", type=int, help="physical output words per frame, including zero padding")
    parser.add_argument("--repeat", type=int, help="repeat expected sequence N times (one expected frame auto-repeats by default)")
    args = parser.parse_args()
    try:
        if args.expected:
            if args.case or args.vectors:
                raise ValueError("--expected cannot be combined with --case/--vectors")
            paths = args.expected
        else:
            if not args.case or not args.vectors:
                raise ValueError("provide --expected or both --case and --vectors")
            manifest = json.loads(args.manifest.read_text())
            cases = {case["name"]: case for case in manifest["cases"]}
            vectors = {vector["name"]: vector for vector in cases[args.case]["vectors"]}
            paths = [args.manifest.parent / vectors[name]["files"][args.kind]["path"] for name in args.vectors]
        expected = [read_words(path) for path in paths]
        frame_words = args.frame_words if args.frame_words is not None else len(expected[0])
        result = compare(read_words(args.output), expected, frame_words, args.repeat)
        print("PASS " + json.dumps(result, sort_keys=True))
    except (ValueError, KeyError, OSError) as exc:
        print(f"FAIL {exc}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
