#include "GAAlgorithms.h"
#include <iostream>
#include "RandomNumberGenerator.h"


std::vector<std::pair<int, int>> selectBest(int bestParents,int parentNumber, const std::vector<std::pair<int, double>>& modelFitness)
{
	RandomIntGenerator intGenerator(0, bestParents - 1);
	std::vector<std::pair<int, int>> parents;


	while (parents.size() * 2 < parentNumber)
	{
		int firstParent = intGenerator();
		int secondParent = intGenerator();
		while (firstParent == secondParent)
		{
			secondParent = intGenerator();
		}
		parents.push_back(std::pair<int, int>{modelFitness[firstParent].first, modelFitness[secondParent].first});
	}
	return parents;
}



int GetWheelParent(const std::vector<double>& CumulativeDistribution)
{
	static RandomDoubleGenerator generator{ 0.0,1.0 };
	// Get Random size and check his position in c.d.f
	double randomValue{ generator() };
	if (randomValue <= CumulativeDistribution[0])
	{
		return 0;
	}
	for (int i = 0; i < CumulativeDistribution.size() - 1; ++i)
	{
		if (randomValue > CumulativeDistribution[i] && randomValue <= CumulativeDistribution[i + 1])
		{
			return i + 1;
		}
	}
	return 0;
}




std::vector<std::pair<int, int>> selectWheel(int parentNumber, const std::vector<std::pair<int, double>>& modelFitness)
{
	std::vector<std::pair<int, int>> parents;
	std::vector<double> Probabilites;
	std::vector<double> CumulativeDistribution;
	parents.reserve(parentNumber);
	Probabilites.reserve(modelFitness.size());
	CumulativeDistribution.reserve(modelFitness.size());
	double fitnessSum{ 0.0 };

	// Calculating probabilities
	for (auto& fitness : modelFitness)
	{
		fitnessSum += fitness.second;
	}
	for (auto& fitness : modelFitness)
	{
		Probabilites.push_back(fitness.second / fitnessSum);
	}
	// Cumulative distribution
	CumulativeDistribution.push_back(Probabilites[0]);
	for (int i = 1; i < modelFitness.size(); ++i)
	{
		CumulativeDistribution.push_back(CumulativeDistribution[i - 1] + Probabilites[i]);
	}

	while (parents.size() * 2 < parentNumber)
	{
		int FirstParent = GetWheelParent(CumulativeDistribution);
		int SecondParent = GetWheelParent(CumulativeDistribution);
		while (FirstParent == SecondParent)
		{
			SecondParent = GetWheelParent(CumulativeDistribution);
		}
		parents.push_back(std::pair<int, int>{modelFitness[FirstParent].first, modelFitness[SecondParent].first});

	}
	return parents;
}

void crossoverSPC(double crossoverProbability, std::vector<std::vector<double>>& weights)
{
	RandomDoubleGenerator doubleGenerator(0.0, 1.0);
	RandomIntGenerator intGenerator(0, static_cast<int>(weights.size() - 1));

	for (int i = 0; i < weights.size() / 2; ++i)
	{
		if (doubleGenerator() < crossoverProbability)
		{
			int dividePoint = intGenerator();

			for (; dividePoint < weights[i * 2].size();++dividePoint)
			{
				std::swap(weights[i * 2][dividePoint], weights[i * 2 + 1][dividePoint]);
			}
		}
	}
}

void crossoverMPC(double crossoverProbability, std::vector<std::vector<double>>& weights)
{
	RandomDoubleGenerator doubleGenerator(0.0, 1.0);
	RandomIntGenerator intGenerator(0, static_cast<int>(weights.size() - 1));

	for (int i = 0; i < weights.size() / 2; ++i)
	{
		if (doubleGenerator() < crossoverProbability)
		{
			int dividePoint1 = intGenerator();
			int dividePoint2 = intGenerator();

			while (dividePoint1 == dividePoint2)
			{
				dividePoint2 = intGenerator();
			}
			if (dividePoint1 > dividePoint2)
			{
				std::swap(dividePoint1, dividePoint2);
			}
			for (; dividePoint1 <= dividePoint2; ++dividePoint1)
			{
				std::swap(weights[i * 2][dividePoint1], weights[i * 2 + 1][dividePoint2]);
			}
		}
	}
}

void mutationADD(double mutationProbability, double min, double max, std::vector<std::vector<double>>& weights)
{
	RandomDoubleGenerator probaGenerator(0.0, 1.0);
	RandomDoubleGenerator doubleGenerator(min, max);

	for (auto& weight : weights)
	{
		if (probaGenerator() < mutationProbability)
		{
			for (auto& connection : weight)
			{
				connection += doubleGenerator();
			}
		}
	}
}

void mutationNEW(double mutationProbability, std::vector<std::vector<double>>& weights)
{
	RandomDoubleGenerator probaGenerator(0.0, 1.0);
	RandomDoubleGenerator doubleGenerator(-1.0, 1.0);

	for (auto& weight : weights)
	{
		if (probaGenerator() < mutationProbability)
		{
			for (auto& connection : weight)
			{
				connection = doubleGenerator();
			}
		}
	}
}



