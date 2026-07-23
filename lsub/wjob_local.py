#!/usr/bin/env python3
"""Generate local O(N) simulation jobs from a Python parameter file.

Running this file without arguments preserves the historical workflow.  The
command-line options make the same generator reusable by the examples without
editing ``lsub/parameter_lsub.py`` or mixing independent campaigns.
"""

from __future__ import print_function

import argparse
import datetime
import importlib.util
import itertools
import os
from pathlib import Path
import shlex
import subprocess
import sys


PARAMETER_NAMES = (
    "N",
    "D",
    "beta",
    "L",
    "h",
    "Seed",
    "N_Measure",
    "N_Each",
    "N_Therm",
    "N_Total",
    "NBlock",
    "MaxNBin",
    "NperBin",
)


def parse_args():
    script_dir = Path(__file__).resolve().parent
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--parameter-file",
        type=Path,
        default=script_dir / "parameter_lsub.py",
        help="Python file containing the parameter arrays",
    )
    parser.add_argument(
        "--output-dir",
        type=Path,
        help="exact batch directory; the default is lsub/jobs/DATE_JOB_NAME_N",
    )
    parser.add_argument("--job-name", default="ON", help="short campaign name")
    parser.add_argument(
        "--max-parallel",
        type=int,
        default=max(1, min(4, os.cpu_count() or 1)),
        help="maximum jobs launched concurrently by lsub.sh",
    )
    parser.add_argument(
        "--skip-build",
        action="store_true",
        help="use the existing bin/a.out instead of invoking make",
    )
    parser.add_argument(
        "--make-jobs",
        type=int,
        default=max(1, os.cpu_count() or 1),
        help="parallelism used for make",
    )
    return parser.parse_args()


def load_parameters(path):
    path = path.expanduser().resolve()
    if not path.is_file():
        raise FileNotFoundError("parameter file does not exist: {}".format(path))
    spec = importlib.util.spec_from_file_location("on_job_parameters", str(path))
    if spec is None or spec.loader is None:
        raise RuntimeError("cannot load parameter file: {}".format(path))
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)

    values = []
    for name in PARAMETER_NAMES:
        if not hasattr(module, name):
            raise ValueError("parameter file is missing array '{}'".format(name))
        value = list(getattr(module, name))
        if not value:
            raise ValueError("parameter array '{}' is empty".format(name))
        values.append(value)
    return values


def unique_default_directory(root, job_name):
    date = datetime.datetime.now().strftime("%Y%m%d")
    base = root / "jobs" / "{}_{}".format(date, job_name)
    suffix = 1
    candidate = Path("{}_{}".format(base, suffix))
    while candidate.exists():
        suffix += 1
        candidate = Path("{}_{}".format(base, suffix))
    return candidate


def shell_quote(value):
    return shlex.quote(str(value))


def write_input(path, combo):
    labels = PARAMETER_NAMES
    with path.open("w") as stream:
        stream.write("//----- Model_Parameters\n")
        for label, value in zip(labels[:5], combo[:5]):
            stream.write("{:<20}\t{}\n".format(label, value))
        stream.write("\n//----- Simulation_Parameters\n")
        for label, value in zip(labels[5:], combo[5:]):
            stream.write("{:<20}\t{}\n".format(label, value))
        stream.write("\n")


def job_directory_name(combo):
    return "N_{}_D_{}_beta_{}_L_{}_h_{}_seed_{}".format(
        combo[0], combo[1], combo[2], combo[3], combo[4], combo[5]
    )


def write_job_script(path, work_dir, executable, display_name):
    with path.open("w") as stream:
        stream.write("#!/usr/bin/env bash\n")
        stream.write("set -eu\n")
        stream.write("cd {}\n".format(shell_quote(work_dir)))
        stream.write("{}\n".format(shell_quote(executable)))
        stream.write(
            "printf '%s finished at %s\\n' {} \"$(date)\"\n".format(
                shell_quote(display_name)
            )
        )
    path.chmod(0o755)


def write_launcher(path, job_count, max_parallel):
    with path.open("w") as stream:
        stream.write("#!/usr/bin/env bash\n")
        stream.write("set -eu\n")
        stream.write("cd \"$(CDPATH= cd -- \"$(dirname -- \"$0\")\" && pwd)\"\n")
        stream.write("max_parallel={}\n".format(max_parallel))
        stream.write("running=0\n")
        stream.write("pids=''\n")
        stream.write("wait_batch() {\n")
        stream.write("  status=0\n")
        stream.write("  for pid in $pids; do\n")
        stream.write("    wait \"$pid\" || status=1\n")
        stream.write("  done\n")
        stream.write("  pids=''\n")
        stream.write("  running=0\n")
        stream.write("  return \"$status\"\n")
        stream.write("}\n")
        stream.write("status=0\n")
        stream.write("i=1\n")
        stream.write("while [ \"$i\" -le {} ]; do\n".format(job_count))
        stream.write("  ./job_$i >\"job_$i.log\" 2>&1 &\n")
        stream.write("  pids=\"$pids $!\"\n")
        stream.write("  running=$((running + 1))\n")
        stream.write("  if [ \"$running\" -ge \"$max_parallel\" ]; then\n")
        stream.write("    wait_batch || status=1\n")
        stream.write("  fi\n")
        stream.write("  i=$((i + 1))\n")
        stream.write("done\n")
        stream.write("if [ \"$running\" -gt 0 ]; then wait_batch || status=1; fi\n")
        stream.write("exit \"$status\"\n")
    path.chmod(0o755)


def main():
    args = parse_args()
    if args.max_parallel < 1 or args.make_jobs < 1:
        raise ValueError("--max-parallel and --make-jobs must be positive")

    script_dir = Path(__file__).resolve().parent
    repository = script_dir.parent
    arrays = load_parameters(args.parameter_file)
    combinations = list(itertools.product(*arrays))

    if not args.skip_build:
        subprocess.run(
            ["make", "-C", str(repository), "-j{}".format(args.make_jobs)],
            check=True,
        )

    executable = (repository / "bin" / "a.out").resolve()
    if not executable.is_file():
        raise FileNotFoundError(
            "simulation executable is missing; run make or omit --skip-build: {}".format(
                executable
            )
        )

    if args.output_dir:
        output_dir = args.output_dir.expanduser().resolve()
        if output_dir.exists():
            raise FileExistsError("output directory already exists: {}".format(output_dir))
    else:
        output_dir = unique_default_directory(script_dir, args.job_name).resolve()
    output_dir.mkdir(parents=True)

    for index, combo in enumerate(combinations, start=1):
        display_name = job_directory_name(combo)
        work_dir = output_dir / display_name
        work_dir.mkdir()
        write_input(work_dir / "input.txt", combo)
        write_job_script(output_dir / "job_{}".format(index), work_dir, executable, display_name)

    launcher = output_dir / "lsub.sh"
    write_launcher(launcher, len(combinations), args.max_parallel)

    if args.output_dir is None:
        fast_script = script_dir / "fast.sh"
        with fast_script.open("w") as stream:
            stream.write("#!/usr/bin/env bash\nset -eu\n")
            stream.write("{}\n".format(shell_quote(launcher)))
        fast_script.chmod(0o755)

    print("jobs are in {}".format(output_dir))
    print("generated {} jobs; run {}".format(len(combinations), launcher))


if __name__ == "__main__":
    try:
        main()
    except (FileNotFoundError, FileExistsError, RuntimeError, ValueError) as error:
        print("error: {}".format(error), file=sys.stderr)
        sys.exit(2)
