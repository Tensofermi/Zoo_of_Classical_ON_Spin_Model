"""Parameter profiles for the 3D O(3) Heisenberg example."""

import os


PROFILE = os.environ.get("EXAMPLE_PROFILE", "quick")

N = [3]
D = [3]
h = [0]
N_Measure = [1]
NperBin = [1]

if PROFILE == "quick":
    beta = [0.66, 0.68, 0.693003, 0.706, 0.726]
    L = [4, 6, 8, 10]
    Seed = [314159]
    N_Each = [80]
    N_Therm = [4]
    N_Total = [12]
    NBlock = [16]
    MaxNBin = [100000]
elif PROFILE == "standard":
    beta = [0.675, 0.684, 0.689, 0.693003, 0.697, 0.702, 0.711]
    L = [4, 6, 8, 10, 12, 16]
    Seed = [314159, 271828]
    N_Each = [200]
    N_Therm = [8]
    N_Total = [32]
    NBlock = [32]
    MaxNBin = [1000000]
else:
    raise ValueError("EXAMPLE_PROFILE must be 'quick' or 'standard'")


if __name__ == "__main__":
    print("model=3D O(3) Heisenberg (N=3, D=3, h=0)")
    print("reference_beta=0.693003")
    print("beta={}".format(beta))
    print("L={}".format(L))
    print("Seed={}".format(Seed))
    print("N_Each={}, N_Therm={}, N_Total={}, NBlock={}".format(N_Each, N_Therm, N_Total, NBlock))
