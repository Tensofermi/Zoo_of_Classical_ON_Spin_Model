#!/usr/bin/env python3
"""Validate, format, and combine the output of one generated local batch."""

import argparse
import csv
import math
from pathlib import Path
import re
import statistics
import subprocess
import sys


STATUS_PATTERN = re.compile(r"Status\s*=\s*([0-9]+(?:\.[0-9]+)?)%")


def parse_args():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--jobs-dir", type=Path, required=True)
    parser.add_argument("--output-dir", type=Path, required=True)
    parser.add_argument("--formatter", type=Path, required=True)
    return parser.parse_args()


def validate_complete(path):
    text = path.read_text(errors="replace")
    statuses = [float(value) for value in STATUS_PATTERN.findall(text)]
    return statuses and abs(statuses[-1] - 100.0) < 1.0e-9


def parse_collect(path):
    lines = path.read_text().splitlines()
    if len(lines) < 3:
        raise ValueError("formatted file has fewer than three lines: {}".format(path))
    names = [normalize_name(name) for name in lines[1].lstrip("#").split()]
    values = lines[2].split()
    if len(names) != len(values):
        raise ValueError(
            "header/data width mismatch in {}: {} names, {} values".format(
                path, len(names), len(values)
            )
        )
    return names, [float(value) for value in values]


def normalize_name(name):
    """Turn printed names such as <Qm>_err into stable table names."""
    if name.startswith("<") and ">" in name:
        closing = name.index(">")
        return name[1:closing] + name[closing + 1 :]
    return name


def write_table(path, names, rows):
    with path.open("w", newline="") as stream:
        writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
        writer.writerow(names)
        for row in rows:
            writer.writerow(["{:.16g}".format(value) for value in row])


def weighted_average(values, errors):
    weighted = [
        (value, error)
        for value, error in zip(values, errors)
        if math.isfinite(value) and math.isfinite(error) and error > 1.0e-13
    ]
    if weighted:
        weights = [1.0 / (error * error) for _, error in weighted]
        total_weight = sum(weights)
        mean = sum(value * weight for (value, _), weight in zip(weighted, weights)) / total_weight
        return mean, math.sqrt(1.0 / total_weight)

    finite_values = [value for value in values if math.isfinite(value)]
    if not finite_values:
        return float("nan"), float("nan")
    mean = statistics.fmean(finite_values)
    if len(finite_values) > 1:
        return mean, statistics.stdev(finite_values) / math.sqrt(len(finite_values))
    fallback_error = errors[0] if errors and math.isfinite(errors[0]) else 0.0
    return mean, fallback_error


def aggregate(names, rows):
    error_indices = [index for index, name in enumerate(names) if name.endswith("_err")]
    if not error_indices:
        raise ValueError("no observable error columns were found")
    observable_start = error_indices[0] - 1
    if observable_start < 0:
        raise ValueError("invalid observable column layout")

    metadata_indices = [
        index
        for index in range(observable_start)
        if names[index] not in ("Seed", "ZZ")
    ]
    groups = {}
    for row in rows:
        key = tuple(row[index] for index in metadata_indices)
        groups.setdefault(key, []).append(row)

    output_names = [names[index] for index in metadata_indices] + ["replicas"]
    output_names.extend(names[observable_start:])
    output_rows = []

    for key, members in groups.items():
        combined = list(key) + [float(len(members))]
        index = observable_start
        while index < len(names):
            if index + 1 >= len(names) or names[index + 1] != names[index] + "_err":
                raise ValueError(
                    "expected value/error pair at column '{}'".format(names[index])
                )
            mean, error = weighted_average(
                [row[index] for row in members],
                [row[index + 1] for row in members],
            )
            combined.extend((mean, error))
            index += 2
        output_rows.append(combined)

    sort_names = [name for name in ("N", "D", "L", "beta", "h") if name in output_names]
    sort_indices = [output_names.index(name) for name in sort_names]
    output_rows.sort(key=lambda row: tuple(row[index] for index in sort_indices))
    return output_names, output_rows


def main():
    args = parse_args()
    jobs_dir = args.jobs_dir.resolve()
    output_dir = args.output_dir.resolve()
    formatter = args.formatter.resolve()
    output_dir.mkdir(parents=True, exist_ok=True)

    outputs = sorted(jobs_dir.glob("*/ouput.txt"))
    if not outputs:
        raise RuntimeError("no ouput.txt files found below {}".format(jobs_dir))

    incomplete = [path for path in outputs if not validate_complete(path)]
    if incomplete:
        preview = ", ".join(str(path.parent.name) for path in incomplete[:5])
        raise RuntimeError(
            "{} incomplete jobs (last status is not 100%): {}".format(
                len(incomplete), preview
            )
        )

    common_names = None
    raw_rows = []
    for output in outputs:
        subprocess.run(
            [sys.executable, str(formatter), str(output)],
            cwd=str(formatter.parent),
            check=True,
        )
        collect = output.with_name("collect.txt")
        if not collect.is_file():
            raise RuntimeError("formatter did not create {}".format(collect))
        names, row = parse_collect(collect)
        if common_names is None:
            common_names = names
        elif names != common_names:
            raise RuntimeError("observable headers differ between generated jobs")
        raw_rows.append(row)

    write_table(output_dir / "raw.tsv", common_names, raw_rows)
    processed_names, processed_rows = aggregate(common_names, raw_rows)
    write_table(output_dir / "processed.tsv", processed_names, processed_rows)
    print(
        "processed {} completed jobs into {} parameter points".format(
            len(raw_rows), len(processed_rows)
        )
    )


if __name__ == "__main__":
    try:
        main()
    except (OSError, RuntimeError, ValueError, subprocess.CalledProcessError) as error:
        print("error: {}".format(error), file=sys.stderr)
        sys.exit(2)
