#!/usr/bin/env bash
set -eu

if [ "$#" -lt 2 ] || [ "$#" -gt 3 ]; then
  echo "usage: $0 EXAMPLE_DIRECTORY EXAMPLE_NAME [quick|standard]" >&2
  exit 2
fi

example_dir=$(CDPATH= cd -- "$1" && pwd)
example_name=$2
profile=${3:-quick}

case "$profile" in
  quick|standard) ;;
  *)
    echo "error: profile must be 'quick' or 'standard'" >&2
    exit 2
    ;;
esac

repository=$(CDPATH= cd -- "$example_dir/../.." && pwd)

for command_name in python3 make; do
  if ! command -v "$command_name" >/dev/null 2>&1; then
    echo "error: required command not found: $command_name" >&2
    exit 2
  fi
done

timestamp=$(date '+%Y%m%d_%H%M%S')
run_dir="$repository/examples/runs/$example_name/${timestamp}_${profile}_$$"
jobs_dir="$run_dir/jobs"
data_dir="$run_dir/data"
cache_dir="$run_dir/.cache"
mkdir -p "$run_dir" "$data_dir" "$cache_dir/matplotlib" "$cache_dir/fontconfig"
export MPLCONFIGDIR="$cache_dir/matplotlib"
export XDG_CACHE_HOME="$cache_dir"

if ! python3 -c 'import matplotlib' >/dev/null 2>&1; then
  echo "error: Matplotlib is required (for example: python3 -m pip install matplotlib)" >&2
  exit 2
fi

echo "[$example_name] building simulator"
make -C "$repository" -j 4

echo "[$example_name] generating local jobs ($profile profile)"
EXAMPLE_PROFILE="$profile" python3 "$repository/lsub/wjob_local.py" \
  --parameter-file "$example_dir/parameters.py" \
  --output-dir "$jobs_dir" \
  --job-name "$example_name" \
  --max-parallel "${EXAMPLE_MAX_PARALLEL:-4}" \
  --skip-build

{
  echo "example=$example_name"
  echo "profile=$profile"
  echo "started=$(date '+%Y-%m-%dT%H:%M:%S%z')"
  echo "repository=$repository"
  echo "source_commit=$(git -C "$repository" rev-parse HEAD 2>/dev/null || echo unknown)"
  echo "parameter_file=$example_dir/parameters.py"
  echo "command=$0 $example_dir $example_name $profile"
  echo
  EXAMPLE_PROFILE="$profile" python3 "$example_dir/parameters.py"
  echo
  make -C "$repository" --no-print-directory compiler-info
} >"$run_dir/manifest.txt"

echo "[$example_name] running generated jobs"
"$jobs_dir/lsub.sh"

echo "[$example_name] formatting and combining data"
python3 "$repository/examples/common/process_results.py" \
  --jobs-dir "$jobs_dir" \
  --output-dir "$data_dir" \
  --formatter "$repository/data/script/format_data.py"

echo "[$example_name] plotting observables"
python3 "$example_dir/plot.py" \
  --input "$data_dir/processed.tsv" \
  --output-prefix "$run_dir/figure"

echo "[$example_name] complete"
echo "results: $run_dir"
