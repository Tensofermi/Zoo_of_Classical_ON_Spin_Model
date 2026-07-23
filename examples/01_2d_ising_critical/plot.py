#!/usr/bin/env python3

import argparse
from pathlib import Path
import sys


sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "common"))
from plot_results import create_plot


parser = argparse.ArgumentParser(description="Plot the 2D Ising critical-region demo")
parser.add_argument("--input", required=True)
parser.add_argument("--output-prefix", required=True)
args = parser.parse_args()

create_plot(
    args.input,
    args.output_prefix,
    "2D Ising model near the critical point",
    0.4406867935,
    [("Qm", "Binder ratio $Q_m$"), ("Corr/L", r"correlation-length ratio $\xi/L$")],
)
