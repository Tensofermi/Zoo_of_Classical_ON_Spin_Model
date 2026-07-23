# Repository guide for agents

This file applies to the whole repository. Treat this repository as both a simulation program and the reference implementation of the local/PBS/Slurm-to-analysis workflow reused by the companion repositories.

## Scientific purpose

The code simulates the ferromagnetic, nearest-neighbor classical O(N) model on a periodic $D$-dimensional hypercubic lattice. Each site carries an $N$-component unit spin. With coupling fixed to one, the simulated Hamiltonian is

$$H=-\sum_{\langle i,j\rangle}\mathbf S_i\cdot\mathbf S_j-h\sum_i S_i^{(1)}.$$

The input varies the spin dimension `N`, spatial dimension `D`, inverse temperature `beta`, external field `h`, and linear size `L`. The repository is valuable as a common baseline for Ising (`N=1`), XY (`N=2`), Heisenberg (`N=3`), and higher-O(N) universality classes, and as a controlled reference against which the long-range, bilayer, loop, and random-walk repositories can be compared.

Important regimes include 2D Ising criticality, 2D XY BKT behavior, ordinary 3D O(N) criticality, and upper-critical-dimension/mean-field crossover. Do not assume that one finite-size ansatz fits all of them: BKT transitions need logarithmic corrections, while $D\geq4$ can require upper-critical-dimension or dangerous-irrelevant-variable corrections.

## Code map and invariants

- `src/config/Inputor_config.hpp` defines and validates model inputs.
- `src/config/Configuration/UpdataScheme.cpp` selects the update algorithm. Inspect it before every production campaign; the available implementations include Swendsen-Wang, Wolff, and Metropolis variants.
- `src/config/Measurement_config.hpp` fills raw observables.
- `src/config/Observable_config.hpp` defines normalization, combined observables, and the textual output contract.
- `input.txt` is the single-run input. `run.sh` builds/runs it.
- `lsub/` generates local jobs. `qsub/` generates PBS or Slurm jobs.
- `data/` formats, collects, splits, and combines results. `fit/` and `plot/` are analysis templates.
- `examples/` contains three isolated, one-command critical-region demonstrations. Their generated jobs, processed tables, manifests, and figures live under the ignored `examples/runs/` tree.

Preserve these invariants:

- The lattice volume is $V=L^D$ and must be checked for integer and allocation overflow when changing lattice code.
- `input.txt`, `Inputor_config.hpp`, job generators, output metadata, and data-column settings form one schema. Change them together.
- Never silently change observable normalization or RNG consumption in a refactor. Such changes can alter published estimators or seeded trajectories.
- Keep formulas in Markdown as `$...$` or `$$...$$`; do not use escaped-parenthesis or escaped-bracket delimiters.

## Build and single-run workflow

```bash
make clean
make -j
make test
./run.sh
```

The program reads `input.txt` from its working directory and writes `ouput.txt` (the historical misspelling is part of the pipeline contract). For a smoke test, use small `L`, small `N_Therm`, and small `N_Total`, then confirm that the run finishes, all reported values are finite, the status reaches 100%, and the requested Seed appears in the header.

The simulation fields are:

- Model: `N`, `D`, `beta`, `L`, `h` in the input file. The printed metadata order is `N,D,beta,h,L`.
- Sampling: `Seed`, `N_Measure`, `N_Each`, `N_Therm`, `N_Total`, `NBlock`, `MaxNBin`, `NperBin`.
- `N_Each` is work between progress/output blocks; `N_Total` controls production blocks; `NBlock`, `MaxNBin`, and `NperBin` control binning. Estimate autocorrelation before treating nominal samples as independent.

Main observables include energy and heat capacity; `M`, `M2`, and `M4`; Binder-like `Qm`; Fourier mode `Mk2`; correlation-length ratio `Corr/L`; and FK/cluster quantities `NCluster`, `S2`, `S4`, `C1`, `C2`, `Qm_fk`, and `C12`. Read the observable header rather than relying on remembered numeric column indices.

## Planning a parameter campaign

Start from the scientific question, not from a large Cartesian product.

1. Fix `N`, `D`, and `h` for a universality-class campaign. Do not pool different models in one fit.
2. Run a pilot with 3--4 modest, geometrically spaced sizes and a coarse `beta` grid. Use dimensionless ratios such as `Qm` and `Corr/L`, together with energy/magnetization diagnostics, to bracket the transition.
3. Refine `beta` near crossings or BKT drift. Add larger sizes only after timing and memory scale have been measured.
4. Use several independent Seeds at every parameter point. Allocate more statistics where autocorrelation is longest, not uniformly by habit.
5. Fit multiple lower-size cutoffs `L_min`, include the correction form appropriate to the universality class, and report fit stability and goodness of fit.

For a concrete example, a 3D XY critical-point search can begin with `N=2`, `D=3`, `h=0`, sizes such as 8, 12, 16, 24, 32, and a coarse beta bracket. Refine around the common drift region of `Qm` and `Corr/L`, then add larger sizes and correction terms. A 2D XY campaign must instead be designed around BKT scaling and logarithmic corrections.

## Canonical task pipeline

This repository is the authoritative source for the reusable workflow. Before adapting it elsewhere, first run it here on a tiny grid.

### Generate tasks

- Edit `qsub/parameter_qsub.py` or `lsub/parameter_lsub.py`. The list product covers `N,D,beta,L,h,Seed` and the sampling controls; `mem` and `job_time` are scheduler resources.
- `qsub/wjob_pbs.py` writes PBS jobs and submits with `qsub`.
- `qsub/wjob_slurm.py` writes Slurm jobs and submits with `sbatch`.
- `lsub/wjob_local.py` writes local jobs started with `nohup`; `lsub/kill_all.sh` stops the generated local jobs.
- The generators assume the repository is one directory above, build with `make`, use `bin/a.out`, create `jobs/<parameter-name>/input.txt`, and expect `ouput.txt` in each job directory.

Before submission, inspect at least one generated input and script, verify the executable path, queue/account/partition directives, memory, wall time, job count, and Seed uniqueness. Use a one-point smoke job before the full array.

### Format and combine data

Run the scripts from the directory layout they expect:

```bash
cd data
bash fast_data.sh
```

The stages are:

1. `script/1_format_jobs.py`: find job `ouput.txt` files and convert the final average block to `collect.txt`.
2. `script/2_collect_data.py`: concatenate formatted jobs into `data/raw/raw.dat`.
3. `script/3_extract_data.py`: sort/split data by configured model parameters.
4. `script/4_compress_data.py`: combine repeated Seeds using inverse-variance weighting.

For this repository, `data/script/header_footer.py` must describe 12 leading fields in the formatted row (`header=12`), with Seed as the eighth 1-based field (`seed_index=8`). The zero-based model columns are `N=0`, `D=1`, `beta=2`, `h=3`, `L=4`. Set `mode` to the actual job source. If output metadata changes, regenerate one `collect.txt`, inspect its two header rows, and update all indices before combining data. A wrong Seed index can silently average unlike parameter points.

Use `data/save.sh` only after validating row counts and headers. Treat `data/clear.sh` as destructive: archive provenance first.

### Fit and plot

The files in `fit/` and `plot/plot.ipynb` are templates, not universal analyses. Update data paths, header-derived x/y/error columns, labels, fit ranges, initial/fixed parameters, and physical scaling functions. `fitting.py` contains several finite-size/BKT-oriented examples and `fitting_Tc.py` contains critical-point examples; choose a model because its assumptions match the physics, not because the script runs. Scan `L_min`, inspect residuals, and keep raw plus processed data linked to the producing commit and input grid.

Python analysis commonly requires NumPy, SciPy, Matplotlib, lmfit, and openpyxl. Record the environment used for production analysis.

## Reusing this workflow in companion repositories

Clone this repository beside the target and copy the workflow directories as a starting point:

```bash
git clone --depth 1 https://github.com/Tensofermi/Zoo_of_Classical_ON_Spin_Model.git
cp -R Zoo_of_Classical_ON_Spin_Model/lsub TARGET/
cp -R Zoo_of_Classical_ON_Spin_Model/qsub TARGET/
cp -R Zoo_of_Classical_ON_Spin_Model/data TARGET/
cp -R Zoo_of_Classical_ON_Spin_Model/fit TARGET/
cp -R Zoo_of_Classical_ON_Spin_Model/plot TARGET/
```

Do not run the copied scripts until the target repository's `AGENTS.md` mapping has been applied. At minimum change model parameter arrays and order, generated input fields, job name, build/source variant, executable path, `header`, `seed_index`, grouping columns, observable columns, and fit model. Preserve a note of the Zoo commit from which scripts were copied.

## Change and verification policy

- For physics changes, trace the chain from input to update to measurement to normalized output and analysis.
- Compile with warnings enabled and run `make test`. For algorithm changes, compare small seeded runs against the previous implementation or an exactly enumerable/sanity-check case.
- New observables require coordinated edits to `Parameter_config.hpp`, measurement, observable registration/combination, output-format tests, and data/fit column handling.
- Do not commit generated jobs, binaries, build products, or bulk simulation output unless explicitly requested.
- Keep commits narrow. State the physical or reliability motivation, the implementation, validation performed, and any compatibility effect on old data.
