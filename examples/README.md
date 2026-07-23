# One-command physics examples

These examples demonstrate the complete local workflow already used by this
repository:

$$\text{parameter grid}\rightarrow\texttt{lsub}\rightarrow\text{Monte Carlo jobs}\rightarrow\text{data formatting}\rightarrow\text{plot}.$$

The three cases were chosen because they show three distinct kinds of critical
behavior with the same O(N) code:

| Example | Model | Reference region | Main diagnostic |
|---|---|---|---|
| `01_2d_ising_critical` | $N=1$, $D=2$ | $\beta_c=\frac{1}{2}\ln(1+\sqrt{2})\approx0.4406868$ | Binder ratio `Qm` and `Corr/L` crossings |
| `02_3d_heisenberg_critical` | $N=3$, $D=3$ | $\beta_c\approx0.693003$ | `Qm` and `Corr/L` crossings |
| `03_2d_xy_bkt` | $N=2$, $D=2$ | $\beta_{\mathrm{BKT}}\approx1.12$ | finite-size evolution of `Corr/L` |

## Run an example

Python 3, a working C++ compiler, GNU Make, and Matplotlib are required. From
the repository root, run for example:

```bash
./examples/01_2d_ising_critical/run.sh quick
```

`quick` is the default and is intentionally small enough for a laptop demo. It
shows the expected qualitative trend but is not publication-quality data.
`standard` uses more sizes, beta values, samples, and independent seeds:

```bash
./examples/01_2d_ising_critical/run.sh standard
```

Every invocation creates a new directory below
`examples/runs/EXAMPLE/TIMESTAMP_PROFILE_PID/`; existing runs are never
overwritten. The directory contains:

- `jobs/`: generated inputs, local job scripts, logs, and raw `ouput.txt` files;
- `data/raw.tsv`: one formatted row per completed seed;
- `data/processed.tsv`: repeated seeds combined by inverse-variance weighting;
- `figure.png` and `figure.pdf`: error-bar plots grouped by lattice size;
- `manifest.txt`: profile, source commit, compiler, parameter file, and command.

The runner rejects incomplete jobs unless the last reported status is 100%.
Observable columns are looked up by their printed names, so the examples do not
depend on remembered numeric column indices.

## Scientific limits

Crossing plots are a way to see finite-size scaling, not an automatic critical
point estimator. Corrections to scaling shift finite-size crossings. This is
especially important for the 2D XY example: a BKT transition has logarithmic
finite-size corrections, so a naive common crossing or ordinary power-law fit
should not be interpreted as a precision estimate of $\beta_{\mathrm{BKT}}$.

Edit the `quick` and `standard` blocks in each example's `parameters.py` when
planning a new range. First expand statistics near the transition, then add
larger sizes after measuring runtime and autocorrelation. Keep at least two
independent seeds for quantitative comparisons.
