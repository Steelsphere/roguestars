#pragma once
#include <random>

class Random
{
public:
	static std::random_device device;
	
	static std::mt19937_64 generator;
	
	static std::uniform_int_distribution<int> one_to_four;
	
	static std::uniform_int_distribution<int> big_number;
	
	static std::uniform_int_distribution<int> one_to_sixteen;
	
	static std::uniform_int_distribution<int> coinflip;
};

