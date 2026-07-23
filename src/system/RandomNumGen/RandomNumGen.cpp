#include "RandomNumGen.hpp"

RandomNumGen::RandomNumGen()
{
    // default seed is 1234567
    initRandomNumGen(1234567);    
}

RandomNumGen::RandomNumGen(int Seed)
{
    initRandomNumGen(Seed);
}

void RandomNumGen::initRandomNumGen(int Seed)
{
    tm32 = 1.0 / 4294967296.0;
    mod2 = 2796203; mul2 = 125;
    len1 = 9689; ifd1 = 471;
    len2 = 127; ifd2 = 30;
    mxrn = 10000;
    
    inxt1.assign(len1 + 1, 0);
    inxt2.assign(len2 + 1, 0);
    ir1.assign(len1 + 1, 0);
    ir2.assign(len2 + 1, 0);
    irn.assign(mxrn + 1, 0);

    int i_r;
    std::uint32_t k_r;
    std::int32_t k1_r;
    nrannr = mxrn;
    const std::uint32_t iseed = static_cast<std::uint32_t>(Seed < 0 ? -static_cast<std::int64_t>(Seed) : Seed) + 1u;
    k_r = 387420489u + 2u * iseed;
    const std::uint32_t initial_k1 = 1313131u * iseed;
    std::memcpy(&k1_r, &initial_k1, sizeof(k1_r));
    k1_r %= mod2;
    for (i_r = 1; i_r <= len1; i_r++)
    {
        k_r *= 32781u;
        k1_r = k1_r * mul2;
        k1_r %= mod2;
        ir1[i_r] = k_r + static_cast<std::uint32_t>(static_cast<std::int64_t>(k1_r) * 8193);
    }
    for (i_r = 1; i_r <= len2; i_r++)
    {
        k_r *= 32781u;
        k1_r = k1_r * mul2;
        k1_r %= mod2;
        ir2[i_r] = k_r + static_cast<std::uint32_t>(static_cast<std::int64_t>(k1_r) * 4099);
    }
    for (i_r = 1; i_r <= len1; i_r++)
    {
        inxt1[i_r] = i_r + 1;
    }
    inxt1[len1] = 1;
    ipnt1 = 1;
    ipnf1 = ifd1 + 1;
    for (i_r = 1; i_r <= len2; i_r++)
    {
        inxt2[i_r] = i_r + 1;
    }
    inxt2[len2] = 1;
    ipnt2 = 1;
    ipnf2 = ifd2 + 1;
}

std::uint32_t RandomNumGen::nextWord()
{
    int i_r;
    std::uint32_t l_r, k_r;
    nrannr = nrannr + 1;
    if (nrannr >= mxrn)
    {
        nrannr = 1;
        for (i_r = 1; i_r <= mxrn; i_r++)
        {
            l_r = ir1[ipnt1] ^ ir1[ipnf1];
            k_r = ir2[ipnt2] ^ ir2[ipnf2];
            irn[i_r] = k_r ^ l_r;
            ir1[ipnt1] = l_r;
            ipnt1 = inxt1[ipnt1];
            ipnf1 = inxt1[ipnf1];
            ir2[ipnt2] = k_r;
            ipnt2 = inxt2[ipnt2];
            ipnf2 = inxt2[ipnf2];
        }
    }
    return irn[nrannr];
}

std::int32_t RandomNumGen::getRandomNum()
{
    const std::uint32_t word = nextWord();
    std::int32_t result;
    std::memcpy(&result, &word, sizeof(result));
    return result;
}

int RandomNumGen::getRandomNum(int MaxNum)
{
    if (MaxNum <= 0) throw std::invalid_argument("MaxNum must be positive");
    return (int)(MaxNum * getRandomDouble());
}

long RandomNumGen::getRandomNum(long MaxNum)
{
    if (MaxNum <= 0) throw std::invalid_argument("MaxNum must be positive");
    return (long)(MaxNum * getRandomDouble());
}

double RandomNumGen::getRandomDouble()
{
    return static_cast<double>(nextWord() ^ 0x80000000u) * tm32;
}

double RandomNumGen::getRandomDouble(double a, double b)
{
    return getRandomDouble() * (b - a) + a;
}

double RandomNumGen::getGaussian()
{
    return sqrt(-2.0 * log(1.0 - getRandomDouble())) * cos(2.0 * std::acos(-1.0) * getRandomDouble());
}
