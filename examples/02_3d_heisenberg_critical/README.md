# 3D O(3) Heisenberg critical point

This example sets $N=3$, $D=3$, and $h=0$ and brackets the continuous
Heisenberg transition near $\beta_c=0.693003(2)$:

```bash
./examples/02_3d_heisenberg_critical/run.sh quick
```

Unlike the Ising model, every spin has three continuous components, but the
same dimensionless finite-size diagnostics remain useful. The `Qm` and
`Corr/L` curves should evolve toward a common critical region as $L$ grows.
The quick profile prioritizes runtime over equilibration and error precision;
use the standard profile and extend independent seeds before fitting crossing
drift or critical exponents.
