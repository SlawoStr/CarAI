#include "GeneticAlgorithm.h"
#include <iostream>

void printNetwork(NeuralNetwork& network)
{
	int layerCounter{ 0 };
	for (auto& layer : network)
	{
		std::cout << layer.getName() << " :" << ++layerCounter << std::endl;
		int neuronCounter{ 0 };
		for (auto& neuron : layer)
		{
			std::cout << "\tNeuron number: " << ++neuronCounter << "  Value: " << neuron.getValue() << std::endl;
			int connectionCounter{ 0 };
			for (auto& connection : neuron)
			{
				std::cout << "\t\tConnection number: " << ++connectionCounter << " Weight: " << connection << std::endl;
			}
		}
	}
}


GeneticAlgorithm::GeneticAlgorithm(std::initializer_list<std::tuple<std::string, size_t, double(*)(double)>> layerInfo,size_t modelsNumber) //: network(modelsNumber)
{
	for (size_t i = 0; i < modelsNumber; ++i)
	{
		network.emplace_back(layerInfo);
	}
}

std::vector<double> GeneticAlgorithm::predictOne(size_t modelIndex, std::vector<double>& input)
{
	return network[modelIndex].predict(input);
}

std::vector<std::vector<double>> GeneticAlgorithm::predictMany(std::vector<size_t> modelIndex, std::vector<std::vector<double>>& inputs)
{
	std::vector<std::vector<double>> predictions(inputs.size());

	// Open mp for faster prediction
	for (size_t i = 0; i < modelIndex.size(); ++i)
	{
		predictions[i] = network[i].predict(inputs[i]);
	}
	return predictions;
}

std::vector<std::vector<double>> GeneticAlgorithm::predictAll(std::vector<std::vector<double>>& inputs)
{
	std::vector<std::vector<double>> predictions(inputs.size());

	for (int i = 0; i < network.size(); ++i)
	{
		predictions[i] = network[i].predict(inputs[i]);
	}
	return predictions;
}

void GeneticAlgorithm::createNewGeneration(std::vector<std::pair<int, double>> modelsFitness)
{
	if (modelsFitness.size() != network.size())
	{
		throw std::length_error("Model fitness must be equal to netowrk size");
	}

	std::sort(modelsFitness.begin(), modelsFitness.end(), [](const auto& lhs, const auto& rhs) {return lhs.second > rhs.second; });

	std::vector<std::pair<int,int>> parents = selectionFunc(modelsFitness);
	std::vector<std::vector<double>>weights;
	for (auto it = parents.begin(); it != parents.end(); ++it)
	{
		weights.push_back(network[it->first].getWeights());
		weights.push_back(network[it->second].getWeights());
	}
	crossoverFunc(weights);
	mutationFunc(weights);

	size_t survivors = network.size() - weights.size();

	std::vector<int> modelsToKeep;
	for (int i = 0; i < survivors; ++i)
	{
		modelsToKeep.push_back(modelsFitness[i].first);
	}
	std::sort(modelsToKeep.begin(), modelsToKeep.end());

	auto it = modelsToKeep.begin();

	for (size_t i = 0, j = 0; i < network.size(); ++i)
	{
		if (it != modelsToKeep.end() && i == *it)
		{
			++it;
			continue;
		}
		network[i].setWeights(weights[j++]);
	}
}
