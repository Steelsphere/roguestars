#include "Random.h"

std::random_device Random::device;
std::mt19937_64 Random::generator(device());

std::uniform_int_distribution<int> Random::one_to_four(1, 4);
std::uniform_int_distribution<int> Random::big_number(0, 500000);
std::uniform_int_distribution<int> Random::one_to_sixteen(1, 16);
std::uniform_int_distribution<int> Random::coinflip(1, 2);