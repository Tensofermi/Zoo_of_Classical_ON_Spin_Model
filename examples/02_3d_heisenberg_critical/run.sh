#!/usr/bin/env bash
set -eu

script_dir=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
exec "$script_dir/../common/run_example.sh" "$script_dir" "02_3d_heisenberg_critical" "${1:-quick}"
