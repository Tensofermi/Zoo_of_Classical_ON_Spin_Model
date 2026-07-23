"""Parameter profiles for the 2D O(2) XY/BKT example."""

import os


PROFILE = os.environ.get("EXAMPLE_PROFILE", "quick")

N = [2]
D = [2]
h = [0]
N_Measure = [1]
NperBin = [1]

if PROFILE == "quick":
    beta = [1.00, 1.06, 1.10, 1.12, 1.14, 1.18]
    L = [8, 16, 24, 32]
    Seed = [161803]
    N_Each = [80]
    N_Therm = [4]
    N_Total = [12]
    NBlock = [16]
    MaxNBin = [100000]
elif PROFILE == "standard":
    beta = [1.04, 1.08, 1.10, 1.11, 1.12, 1.13, 1.14, 1.16, 1.20]
    L = [8, 12, 16, 24, 32, 48, 64]
    Seed = [161803, 141421]
    N_Each = [200]
    N_Therm = [8]
    N_Total = [32]
    NBlock = [32]
    MaxNBin = [1000000]
else:
    raise ValueError("EXAMPLE_PROFILE must be 'quick' or 'standard'")


if __name__ == "__main__":
    print("model=2D XY/BKT (N=2, D=2, h=0)")
    print("reference_beta=1.12")
    print("beta={}".format(beta))
    print("L={}".format(L))
    print("Seed={}".format(Seed))
    print("N_Each={}, N_Therm={}, N_Total={}, NBlock={}".format(N_Each, N_Therm, N_Total, NBlock))
