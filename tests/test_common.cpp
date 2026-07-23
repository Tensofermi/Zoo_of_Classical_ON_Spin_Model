#include "config/Lattice/Hyperrectangle.hpp"
#include "system/RandomNumGen/RandomNumGen.hpp"

#include <cassert>
#include <cstdint>
#include <iostream>

int main()
{
    const std::uint32_t expected[] = {
        3120687069u, 648888036u, 180407624u, 258054113u,
        2602618097u, 573297273u, 555146359u, 1664810582u
    };
    RandomNumGen rng(1234);
    for (std::size_t i = 0; i < sizeof(expected) / sizeof(expected[0]); ++i)
    {
        const std::uint64_t word = static_cast<std::uint64_t>(rng.getRandomDouble() * 4294967296.0);
        assert(word == expected[i]);
    }

    Hyperrectangle lattice;
    lattice.set(3, {2, 3, 4});
    for (int x = 0; x < 2; ++x)
    {
        for (int y = 0; y < 3; ++y)
        {
            for (int z = 0; z < 4; ++z)
            {
                const std::vector<int> coordinate{x, y, z};
                const long site = lattice.getSite(coordinate);
                assert(lattice.getComponent(site, 0) == x);
                assert(lattice.getComponent(site, 1) == y);
                assert(lattice.getComponent(site, 2) == z);
            }
        }
    }
    assert(lattice.getDirSite(0, 0, 5) == lattice.getDirSite(0, 0, 1));

    std::cout << "common regression tests passed\n";
    return 0;
}
