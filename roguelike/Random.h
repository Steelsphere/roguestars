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

	static std::uniform_int_distribution<int> one_to_thirty_two;

	static std::uniform_int_distribution<int> one_to_sixty_four;

	static std::uniform_int_distribution<int> one_to_one_twenty_eight;

	static std::uniform_int_distribution<int> one_to_eight;

};

