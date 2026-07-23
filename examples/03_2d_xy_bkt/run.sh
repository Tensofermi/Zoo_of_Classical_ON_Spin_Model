#!/usr/bin/env bash
set -eu

script_dir=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
exec "$script_dir/../common/run_example.sh" "$script_dir" "03_2d_xy_bkt" "${1:-quick}"
