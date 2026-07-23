#!/usr/bin/env bash
set -eu

script_dir=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
exec "$script_dir/../common/run_example.sh" "$script_dir" "01_2d_ising_critical" "${1:-quick}"
