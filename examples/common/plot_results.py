#!/usr/bin/env python3
"""Shared plotting helper for the three one-command examples."""

import csv
import math
from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt


def read_rows(path):
    with Path(path).open(newline="") as stream:
        rows = list(csv.DictReader(stream, delimiter="\t"))
    if not rows:
        raise ValueError("processed table is empty: {}".format(path))
    return rows


def create_plot(input_path, output_prefix, title, beta_reference, panels):
    rows = read_rows(input_path)
    required = {"beta", "L"}
    for observable, _ in panels:
        required.update((observable, observable + "_err"))
    missing = sorted(required.difference(rows[0]))
    if missing:
        raise ValueError("missing columns: {}".format(", ".join(missing)))

    sizes = sorted({int(float(row["L"])) for row in rows})
    figure, axes = plt.subplots(1, len(panels), figsize=(6.1 * len(panels), 4.6))
    if len(panels) == 1:
        axes = [axes]

    for axis, (observable, ylabel) in zip(axes, panels):
        for size in sizes:
            selected = [row for row in rows if int(float(row["L"])) == size]
            selected.sort(key=lambda row: float(row["beta"]))
            x = [float(row["beta"]) for row in selected]
            y = [float(row[observable]) for row in selected]
            yerr = [float(row[observable + "_err"]) for row in selected]
            finite = [
                index
                for index, values in enumerate(zip(x, y, yerr))
                if all(math.isfinite(value) for value in values)
            ]
            axis.errorbar(
                [x[index] for index in finite],
                [y[index] for index in finite],
                yerr=[yerr[index] for index in finite],
                marker="o",
                markersize=4,
                linewidth=1.2,
                capsize=2,
                label="L={}".format(size),
            )
        axis.axvline(beta_reference, color="black", linestyle="--", linewidth=1.0, alpha=0.7)
        axis.set_xlabel(r"inverse temperature $\beta$")
        axis.set_ylabel(ylabel)
        axis.grid(alpha=0.25)
        axis.legend(frameon=False, fontsize=9)

    figure.suptitle(title)
    figure.tight_layout()
    prefix = Path(output_prefix)
    figure.savefig(str(prefix) + ".png", dpi=180, bbox_inches="tight")
    figure.savefig(str(prefix) + ".pdf", bbox_inches="tight")
    plt.close(figure)
