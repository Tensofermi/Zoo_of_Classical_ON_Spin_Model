"""Parameter profiles for the 2D nearest-neighbor Ising example."""

import os


PROFILE = os.environ.get("EXAMPLE_PROFILE", "quick")

N = [1]
D = [2]
h = [0]
N_Measure = [1]
NperBin = [1]

if PROFILE == "quick":
    beta = [0.425, 0.435, 0.4406868, 0.446, 0.456]
    L = [8, 12, 16, 24]
    Seed = [13579]
    N_Each = [80]
    N_Therm = [4]
    N_Total = [12]
    NBlock = [16]
    MaxNBin = [100000]
elif PROFILE == "standard":
    beta = [0.43, 0.435, 0.438, 0.4406868, 0.443, 0.446, 0.451]
    L = [8, 12, 16, 24, 32]
    Seed = [13579, 24680]
    N_Each = [250]
    N_Therm = [8]
    N_Total = [32]
    NBlock = [32]
    MaxNBin = [1000000]
else:
    raise ValueError("EXAMPLE_PROFILE must be 'quick' or 'standard'")


if __name__ == "__main__":
    print("model=2D Ising (N=1, D=2, h=0)")
    print("reference_beta=0.4406867935")
    print("beta={}".format(beta))
    print("L={}".format(L))
    print("Seed={}".format(Seed))
    print("N_Each={}, N_Therm={}, N_Total={}, NBlock={}".format(N_Each, N_Therm, N_Total, NBlock))
