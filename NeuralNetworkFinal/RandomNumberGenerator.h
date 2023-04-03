#pragma once
#include <random>


class RandomIntGenerator
{
public:
	RandomIntGenerator(int minValue, int maxValue) : generator(rd()), intDistr(minValue, maxValue) {}
	void resetGenerator(int minValue, int maxValue) { std::uniform_int_distribution<>distr(minValue, maxValue); intDistr = std::move(distr); }

	int operator()() { return intDistr(generator); }
private:
	std::random_device rd;
	std::mt19937 generator;
	std::uniform_int_distribution<>intDistr;
};


class RandomDoubleGenerator
{
public:
	RandomDoubleGenerator(double minValue, double maxValue) : generator(rd()), doubleDistr(minValue, maxValue) {}
	void resetGenerator(double minValue, double maxValue) { std::uniform_real_distribution<>distr(minValue, maxValue); doubleDistr = std::move(distr); }

	double operator()() { return doubleDistr(generator); }
private:
	std::random_device rd;
	std::mt19937 generator;
	std::uniform_real_distribution<> doubleDistr;

};