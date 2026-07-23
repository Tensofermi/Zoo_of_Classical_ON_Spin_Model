# 2D Ising critical point

This example sets $N=1$, $D=2$, and $h=0$, then scans around the exact square-
lattice critical coupling

$$\beta_c=\frac{1}{2}\ln(1+\sqrt{2})\approx0.4406867935.$$

Run it from the repository root:

```bash
./examples/01_2d_ising_critical/run.sh quick
```

The plot shows the Binder ratio `Qm` and correlation-length ratio `Corr/L` for
several $L$. These dimensionless quantities approach size-independent scaling
functions near criticality, so neighboring curves cross close to $\beta_c$.
Finite-size corrections and the deliberately short quick run prevent the
crossing from being a precision estimate. Use `standard`, then increase seeds
and statistics in `parameters.py`, for quantitative work.
