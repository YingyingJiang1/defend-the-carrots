#include <random>

int myRand(int range)
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0,range-1); // distribution in range [0, range-1]

    return dist(rng);
}

int rand0_3()
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0,3); // distribution in range [0, 3]

    return dist(rng);
}
