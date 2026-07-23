# 2D XY BKT transition

This example sets $N=2$, $D=2$, and $h=0$ and scans the topological transition
region near $\beta_{\mathrm{BKT}}\approx1.12$:

```bash
./examples/03_2d_xy_bkt/run.sh quick
```

The transition is driven by vortex-antivortex unbinding rather than ordinary
symmetry-breaking order. The primary panel therefore follows `Corr/L` as both
$\beta$ and $L$ change; `Qm` provides a complementary magnetic diagnostic.
BKT scaling contains strong logarithmic corrections, so the dashed reference
line is only a guide and the curves need not form the clean size-independent
crossing familiar from an ordinary second-order transition. Precision work
requires larger $L$, more seeds, and a BKT-specific finite-size analysis.
