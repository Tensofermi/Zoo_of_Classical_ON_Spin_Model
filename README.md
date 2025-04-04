# Zoo of Classical O(N) Spin Model

This Monte Carlo simulation program computes the partition function:
$$\mathcal{Z} = \sum_{\{S_i\}} e^{-\beta H}$$
for a **d-dimensional O(N) spin model**, where $d = 1, 2, 3, \ldots$ and $N = 1, 2, 3, \ldots$. The Hamiltonian is given by:
$$ \beta H = -\beta J \sum_{\langle ij \rangle} \mathbf{S}_i \cdot \mathbf{S}_j - \beta \mathbf{h} \cdot \sum_i \mathbf{S}_i, $$
with $J = 1$ (ferromagnetic coupling), leaving $\beta$ (inverse temperature) and $\mathbf{h}$ (external field) as tunable parameters.

The **Swendsen-Wang cluster algorithm** is employed for efficient configuration updates, particularly effective near critical points.

## How to use
<!-- 1. Clone the repository:
   ```bash
   git clone https://github.com/username/O(N)-spin-model.git
   cd O(N)-spin-model
   ```
2. 1 -->


## Features
- Simulate O(N) spin models in arbitrary dimensions $d$ and spin components $N$.
- Swendsen-Wang algorithm for reduced critical slowing-down.
- Parallel computing support (OpenMP or MPI).
- Measure observables: energy, magnetization, susceptibility, correlation length.
- Output thermodynamic quantities and critical exponents.
- Phase diagram plotting tools.

## Phase Transitions and Universality Classes
Below is a summary of notable properties for specific $d$ and $N$:

| $d$ | $N$ | Phase Transition? | Type          | Universality Class      | Lower Critical Dimension | Notable Model      |
|---------|---------|--------------------|---------------|-------------------------|--------------------------|--------------------|
| 1       | Any     | No (except $T=0$) | —             | —                       | 1                        | —                  |
| 2       | 1       | Yes                | Continuous    | 2D Ising                | 1                        | Ising              |
| 2       | 2       | Yes                | BKT Transition| Kosterlitz-Thouless      | 2                        | XY                 |
| 2       | ≥3      | No                 | —             | —                       | 2                        | —                  |
| 3       | 1       | Yes                | Continuous    | 3D Ising                | 1                        | Ising              |
| 3       | 2       | Yes                | Continuous    | 3D XY                   | 2                        | XY                 |
| 3       | 3       | Yes                | Continuous    | 3D Heisenberg           | 2                        | Heisenberg         |
| ≥4      | Any     | Yes                | Continuous    | Mean-field              | $d_c = 4$            | —                  |

**Notes**:
- **Lower critical dimension**: Dimension below which no finite-$T$ phase transition occurs.
- **BKT transition**: Topological phase transition in 2D XY models ($N=2$).
- **Mermin-Wagner theorem**: Prohibits spontaneous symmetry breaking for $N \geq 2$ in $d=2$.

