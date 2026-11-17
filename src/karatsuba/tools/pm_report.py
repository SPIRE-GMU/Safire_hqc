#!/usr/bin/env python3
"""Report Synopsys bridge map sections, symbols and coreverify diagnostics.

The supported format was established from excerpts of the map files, not from a complete map.
Section Used and coreverify program size are independent measurements. Never
sum alias memories, infer sizes from addresses, or convert Width != 8 to bytes.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import shutil

ROOT = Path.cwd()   # the project directory: make runs the tools from <config>/aie
SECTION = re.compile(r"^\s*Memory map for memory '([^']+)':\s*$")
FIELD = re.compile(r"^\s*(Size|Width|Offset|Used)\s*=\s*(0[xX][\da-fA-F]+|\d+)(?:\s+bits)?\s*$")
ENTRY = re.compile(r"^\s*(0[xX][\da-fA-F]+)\.\.(0[xX][\da-fA-F]+)"
                   r"(?:\s+\(\s*(\d+)\s+items\))?\s*:\s*(.*?)\s*$")
SYMBOL = re.compile(r"^(.*?)\s+\((Function|Data),\s*([^,]+),\s*([^)]*)\)"
                    r"(?:\s+\(stack frame size\s*=\s*(\d+)\))?\s*$")
COREVERIFY = re.compile(r"Program size (\d+) bytes exceeds the available program memory size (\d+)")


def number(value):
    return int(value, 16 if value.lower().startswith("0x") else 10)


def plain_name(symbol):
    """Read length-prefixed Itanium identifiers, not a full demangler.

    Crucially k_btfy is not mistaken for a prefix of k_btfy_si10.
    """
    if not symbol.startswith("_Z"):
        return symbol
    rest = symbol[2:]
    nested = rest.startswith("N")
    if nested:
        rest = rest[1:]
    names = []
    while rest:
        match = re.match(r"(\d+)", rest)
        if not match:
            break
        length = int(match[1])
        rest = rest[len(match[1]):]
        if length <= 0 or len(rest) < length:
            return symbol
        names.append(rest[:length])
        rest = rest[length:]
        if not nested:
            break
    return "::".join(names) if names else symbol


def parse_map(text):
    sections = []
    section = None
    warnings = []
    for lineno, line in enumerate(text.splitlines(), 1):
        match = SECTION.match(line)
        if match:
            section = {"name": match[1], "line": lineno, "fields": {},
                       "field_records": [], "duplicate_fields": [], "entries": []}
            sections.append(section)
            continue
        if section is None:
            continue
        match = FIELD.match(line)
        if match:
            key = match[1].lower()
            if key in section["fields"]:
                warnings.append(f"line {lineno}: duplicate {key} field in {section['name']}")
                section["duplicate_fields"].append(key)
            section["field_records"].append({"line": lineno, "field": key,
                                             "value": number(match[2]), "raw": line})
            section["fields"][key] = number(match[2])
            continue
        match = ENTRY.match(line)
        if not match:
            if re.match(r"^\s*0[xX]", line):
                warnings.append(f"line {lineno}: unparsed address entry: {line}")
            continue
        start, end, items, body = match.groups()
        entry = {"line": lineno, "raw": line, "start": start, "end": end,
                 "size_items": int(items) if items is not None else None}
        if body == "Reserved" or body == "Stack":
            entry.update(kind=body, description=body)
        elif body.startswith("Occupied in alias"):
            entry.update(kind="Alias", description=body)
        else:
            symbol_match = SYMBOL.match(body)
            if symbol_match:
                qualified, kind, visibility, segment, stack = symbol_match.groups()
                if "::" in qualified:
                    source, symbol = qualified.rsplit("::", 1)
                else:
                    source, symbol = None, qualified
                entry.update(kind=kind, object=source, symbol=symbol,
                             name=plain_name(symbol), visibility=visibility,
                             segment=segment, stack_frame_raw=int(stack) if stack else None)
            else:
                entry.update(kind="Unknown", description=body)
                warnings.append(f"line {lineno}: unsupported entry kind: {body}")
        section["entries"].append(entry)
    names = [section["name"] for section in sections]
    for name in sorted(set(names)):
        if names.count(name) > 1:
            warnings.append(f"{name}: duplicate memory sections; usage is UNRESOLVED")
    for section in sections:
        fields = section["fields"]
        width = fields.get("width") if "width" not in section["duplicate_fields"] else None
        used = fields.get("used") if "used" not in section["duplicate_fields"] else None
        section.update(width_bits=width, units="bytes" if width == 8 else "units unknown",
                       used_items=used, used_bytes=used if width == 8 else None)
        for entry in section["entries"]:
            entry.update(memory=section["name"], width_bits=width,
                         units=section["units"],
                         size_bytes=entry["size_items"] if width == 8 else None)
            if "stack_frame_raw" in entry:
                entry["stack_frame_bytes"] = entry["stack_frame_raw"] if width == 8 else None
        for key in ("size", "width", "offset", "used"):
            if key not in fields:
                warnings.append(f"{section['name']}: missing {key} field")
    if not sections:
        warnings.append("No supported memory section found; inspect saved original map")
    return sections, warnings


def load_mapping(root=ROOT):
    """Only function identities are prescribed; physical placement is remote."""
    names = json.loads((root / "tools/project.json").read_text())["kernel_names"]
    if len(set(names)) != len(names):
        raise ValueError("duplicate kernel function in tools/project.json")
    return {name: {"function": name, "source": "tools/project.json"} for name in names}


def identify(functions, core, mapping):
    names = sorted({entry["name"] for entry in functions if entry["name"] in mapping})
    result = {"kernel": "UNRESOLVED", "kernel_candidates": names}
    if len(names) != 1:
        result["association_evidence"] = "expected one kernel function symbol; found " + str(names)
        return result
    name = names[0]
    result.update(kernel=name, mapping=mapping[name],
                  association_evidence="Unique configured function symbol in this map; "
                                       "core is read from its directory, not prescribed by the graph")
    return result


def one_section(sections, name):
    matches = [section for section in sections if section["name"] == name]
    return matches[0] if len(matches) == 1 else None


def usage(sections, name):
    section = one_section(sections, name)
    if section is None:
        return {"used_items": None, "used_bytes": None, "width_bits": None, "units": "UNRESOLVED"}
    return {key: section[key] for key in ("used_items", "used_bytes", "width_bits", "units")}


def sorted_entries(sections, kind):
    entries = [entry for section in sections for entry in section["entries"] if entry["kind"] == kind]
    # Compare byte sizes only when known; otherwise sort raw items within each width.
    return sorted(entries, key=lambda entry: (entry["size_bytes"] is None,
                  entry["width_bits"] or 0, -(entry["size_items"] or 0), entry["symbol"]))


def analyze(path, work, result_dir, mapping):
    content = path.read_bytes()
    sections, warnings = parse_map(content.decode("utf-8", errors="replace"))
    relative = path.relative_to(work)
    parts = relative.parts
    core = parts[1] if len(parts) == 4 and parts[0] == "aie" and parts[2] == "Release" else "UNRESOLVED"
    functions = sorted_entries(sections, "Function")
    data = sorted_entries(sections, "Data")
    association = identify(functions, core, mapping)
    saved = result_dir / "maps" / relative
    saved.parent.mkdir(parents=True, exist_ok=True)
    shutil.copy2(path, saved)
    diagnostics = []
    log = work / "aie" / core / (core + ".log")
    saved_log = None
    if log.is_file():
        saved_log = result_dir / "maps" / log.relative_to(work)
        saved_log.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(log, saved_log)
        for lineno, line in enumerate(log.read_text(encoding="utf-8", errors="replace").splitlines(), 1):
            match = COREVERIFY.search(line)
            if match:
                diagnostics.append({"line": lineno, "raw": line,
                                    "program_bytes": int(match[1]), "available_bytes": int(match[2])})
    runtime = []
    unknown_objects = []
    for entry in functions:
        source = entry["object"]
        if source is None or core == "UNRESOLVED" or association["kernel"] == "UNRESOLVED":
            entry["ownership"] = "UNRESOLVED"
            unknown_objects.append(entry["symbol"])
        elif Path(source.split("(", 1)[0]).name == core + ".o" and "(" not in source:
            entry["ownership"] = "core object"
        else:
            entry["ownership"] = "runtime / non-core object"
            runtime.append(entry)
    frames = [entry["stack_frame_bytes"] for entry in functions if entry["stack_frame_bytes"] is not None]
    frame_rows = [{key: entry[key] for key in ("symbol", "memory", "stack_frame_raw", "stack_frame_bytes", "units")}
                  for entry in functions if entry["stack_frame_raw"] is not None]
    frame_known = all(entry["stack_frame_bytes"] is not None for entry in functions
                      if entry["stack_frame_raw"] is not None)
    runtime_known = (not unknown_objects and
                     len({section["name"] for section in sections}) == len(sections) and
                     all(entry["size_bytes"] is not None for entry in runtime))
    runtime_groups = {}
    for entry in runtime:
        key = str(entry["width_bits"])
        previous = runtime_groups.get(key, 0)
        runtime_groups[key] = (previous + entry["size_items"]
                               if previous is not None and entry["size_items"] is not None else None)
    if unknown_objects:
        warnings.append("Function ownership unresolved: " + ", ".join(unknown_objects))
    return dict(association, core=core, map=str(path), saved_map=str(saved),
                sha256=hashlib.sha256(content).hexdigest(), log=str(log) if log.is_file() else None,
                saved_log=str(saved_log) if saved_log else None, sections=sections,
                functions=functions, data=data, coreverify=diagnostics,
                pm=usage(sections, "PM"), dmb=usage(sections, "DMb"), stack=usage(sections, "DM_stack"),
                max_stack_frame_bytes=max(frames) if frames and frame_known else None, stack_frames=frame_rows,
                runtime={"definition": "Function entries from objects other than <core>.o; no padding inferred",
                         "function_count": len(runtime), "items_by_width_bits": runtime_groups,
                         "size_bytes": sum(entry["size_bytes"] for entry in runtime) if runtime_known and functions else None,
                         "unknown_ownership": unknown_objects}, warnings=warnings,
                fallback_excerpt=content.decode("utf-8", errors="replace").splitlines()[:100]
                if not sections else [])


def show_usage(value):
    if value["used_bytes"] is not None:
        return f"{value['used_bytes']} B"
    if value["used_items"] is not None:
        return f"{value['used_items']} items (Width={value['width_bits']}; units unknown)"
    return "UNRESOLVED"


def report_text(records, work, result_dir, mapping=None):
    lines = ["Karatsuba AIE Synopsys bridge map report", f"Work: {work}",
             "Supported format: established from map excerpts; complete maps still require verification.",
             "PM Used and coreverify program size remain separate; no reconciliation or alias summation.",
             "Stack = DM_stack Used; maximum frame is a single function frame, not call-chain stack usage.",
             "Only Width = 8 is converted from items to bytes. Width != 8: units unknown.", "",
             "kernel | core | PM Used | coreverify program bytes | DMb Used | stack | max frame B"]
    if not records:
        lines.append("NO MAP FILES: pending remote build")
    for record in records:
        cv = ", ".join(str(item["program_bytes"]) for item in record["coreverify"]) or "absent"
        frame = record["max_stack_frame_bytes"]
        lines.append(" | ".join((record["kernel"], record["core"], show_usage(record["pm"]), cv,
                     show_usage(record["dmb"]), show_usage(record["stack"]), str(frame) if frame is not None else "UNRESOLVED")))
    for record in records:
        lines.extend(["", f"[{record['kernel']}] core {record['core']}",
                      "Association: " + record["association_evidence"], f"Original map: {record['saved_map']}",
                      "Memory sections (separate raw records; aliases are not summed):"])
        for section in record["sections"]:
            fields = section["fields"]
            lines.append(f"  {section['name']}: Size={fields.get('size')} Width={fields.get('width')} bits "
                         f"Offset={fields.get('offset')} Used={fields.get('used')} items; {show_usage(section)}")
        lines.append("Coreverify diagnostics (verbatim):")
        lines.extend("  " + entry["raw"] for entry in record["coreverify"])
        if not record["coreverify"]:
            lines.append("  No matching diagnostic; this is not evidence of a successful compile.")
        for kind, key in (("Functions", "functions"), ("Data", "data")):
            lines.extend([kind + " (descending size; unknown-width items grouped by Width):",
                          "  size | memory | name | stack frame | object"])
            for entry in record[key]:
                size = f"{entry['size_bytes']} B" if entry["size_bytes"] is not None else f"{entry['size_items']} items (units unknown)"
                stack = entry.get("stack_frame_bytes")
                if stack is None and entry.get("stack_frame_raw") is not None:
                    stack = f"{entry['stack_frame_raw']} (units unknown)"
                lines.append(f"  {size} | {entry['memory']} | {entry['name']} | {stack if stack is not None else '-'} | {entry['object']}")
            if not record[key]:
                lines.append("  No supported entries.")
        runtime = record["runtime"]
        subtotal = f"{runtime['size_bytes']} B" if runtime['size_bytes'] is not None else "UNRESOLVED"
        lines.append(f"Runtime / non-core-object function subtotal: {subtotal}; "
                     f"{runtime['function_count']} functions; raw items by Width={runtime['items_by_width_bits']}")
        lines.append("  " + runtime["definition"])
        lines.extend("WARNING: " + item for item in record["warnings"])
        if record["fallback_excerpt"]:
            lines.extend(["Unsupported format: first 100 original lines:"] + record["fallback_excerpt"])
    if mapping is not None:
        identified = {row["kernel"] for row in records if row["kernel"] != "UNRESOLVED"}
        lines.extend(["", f"Configured kernel tiles: {len(mapping)}; identified kernel tiles in maps: {len(identified)}",
                      "Missing kernel maps: " + (", ".join(sorted(set(mapping) - identified)) or "none"),
                      "Total physical tile count: UNVERIFIED. Map directories do not establish whether additional",
                      "tiles hold only data; inspect compiler placement/memory allocation reports remotely."])
    lines.extend(["", f"Reports: {result_dir / 'pm_report.txt'} and {result_dir / 'pm_report.json'}"])
    return "\n".join(lines) + "\n"


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--work", type=Path, default=ROOT / "Work")
    parser.add_argument("--results", type=Path, default=ROOT / "results")
    args = parser.parse_args(argv)
    work = args.work
    result_dir = args.results / work.resolve().name
    result_dir.mkdir(parents=True, exist_ok=True)
    mapping = load_mapping()
    paths = sorted((work / "aie").glob("*/Release/*.map"))
    if not paths and work.is_dir():
        paths = sorted(work.rglob("*.map"))
    records = [analyze(path, work, result_dir, mapping) for path in paths]
    summary = {"format": "Synopsys bridge memory map", "format_evidence": "map excerpts",
               "full_remote_format_verified": False, "work": str(work), "maps_found": len(records),
               "maps": records, "unresolved_maps": [row["map"] for row in records if row["kernel"] == "UNRESOLVED"]}
    identified = {row["kernel"] for row in records if row["kernel"] != "UNRESOLVED"}
    summary["tile_inventory"] = {
        "configured_kernel_tiles": len(mapping), "identified_kernel_tiles": len(identified),
        "missing_kernel_maps": sorted(set(mapping) - identified),
        "total_physical_tiles": None, "additional_data_only_tiles": None,
        "note": "Requires compiler placement/memory allocation reports; map file counts alone are insufficient."}
    report = report_text(records, work, result_dir, mapping)
    (result_dir / "pm_report.json").write_text(json.dumps(summary, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
    (result_dir / "pm_report.txt").write_text(report, encoding="utf-8")
    print(report, end="")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
