#pragma once
#include <chrono>
#include <string>

#define FTIME(x) \
{std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now(); \
x; \
std::chrono::time_point<std::chrono::high_resolution_clock> end = std::chrono::high_resolution_clock::now(); \
std::chrono::duration<double> elapsed = end - start; \
throw std::runtime_error("\nf: " + std::string(__FILE__) + "\nl: " + std::string(std::to_string(__LINE__)) + "\nrun time: " + std::to_string(elapsed.count()) + "s");}