#include "NeuralNetwork.h"
#include <stdexcept>

NeuralNetwork::NeuralNetwork(size_t inputSize, size_t outputSize, double(*activationFunc)(double))
{
	if (inputSize == 0 || outputSize == 0)
	{
		throw std::length_error("Length of layer cant be less than 1");
	}
	layers.emplace_back("Input layer", inputSize, outputSize, activationFunc, true);
	layers.emplace_back("Output layer", outputSize, 0, nullptr, false);
}

NeuralNetwork::NeuralNetwork(std::initializer_list<std::tuple<std::string, size_t, double(*)(double)>> layerInfo)
{
	if (layerInfo.size() < 2)
	{
		throw std::length_error("Length of Neural network cant be less than 2 ( input layer, output layer");
	}
	for (auto it = layerInfo.begin(); it != layerInfo.end(); ++it)
	{
		if (std::get<1>(*it) == 0)
		{
			throw std::length_error("Layers must have atleast 1 neuron");
		}
		if (it != layerInfo.end() - 1)
		{
			layers.emplace_back(std::get<0>(*it), std::get<1>(*it), std::get<1>(*std::next(it)), std::get<2>(*it), true);
		}
		else
		{
			layers.emplace_back(std::get<0>(*it), std::get<1>(*it), 0, std::get<2>(*it), false);
		}
	}
}

void NeuralNetwork::addLayer(std::string name, size_t layerSize, double(*activationFunc)(double))
{
	auto pos = layers.emplace(layers.end() - 1, name, layerSize, layers.back().getSize(), activationFunc, true);
	std::prev(pos)->restartConnection(pos->getSize() - pos->isBias());
}

void NeuralNetwork::deleteLayer()
{
	// Network require atleast 2 layers
	if (layers.size() < 2)
	{
		return;
	}
	// Delete layer one before last
	auto it = layers.erase(layers.end() - 2);
	std::prev(it)->restartConnection(it->getSize());
}

void NeuralNetwork::addNode(size_t layerIndex)
{
	if (layerIndex >= layers.size())
		throw std::out_of_range("Cant delete node from layer that doesnt exist");
	Layer* prev = nullptr;
	Layer * next = nullptr;

	if (layerIndex > 0)
	{
		prev = &layers[layerIndex - 1];
	}
	if (layerIndex < layers.size() - 1)
	{
		next = &layers[layerIndex + 1];
	}
	layers[layerIndex].addNeuron(prev, next);
}

void NeuralNetwork::deleteNode(size_t layerIndex)
{
	if (layerIndex >= layers.size())
		throw std::out_of_range("Cant delete node from layer that doesnt exist");
	Layer* prev = layerIndex > 0 ? &layers[layerIndex - 1] : nullptr;
	layers[layerIndex].deleteNeuron(prev);
}

void NeuralNetwork::deleteConnection(size_t layerIndex, size_t nodeIndex, size_t connectionIndex)
{
	if (layerIndex >= layers.size())
		throw std::out_of_range("This layer doesnt exist");
	layers[layerIndex].deleteConnection(nodeIndex, connectionIndex);
}

void NeuralNetwork::addConnection(size_t layerIndex, size_t nodeIndex, size_t connectionIndex)
{
	if (layerIndex >= layers.size())
		throw std::out_of_range("This layer doesnt exist");
	layers[layerIndex].addConnection(nodeIndex, connectionIndex);
}

std::vector<double> NeuralNetwork::predict(std::vector<double>& input)
{
	if (input.size() != layers.front().getSize() - 1)
	{
		throw std::length_error("Input must be equal to input layer");
	}
	// Set values for input layer
	for (size_t i = 0; i < layers[0].getSize() - 1; ++i)
	{
		layers[0][i].setValue(input[i]);
	}
	for (auto it = layers.begin(); it != layers.end() - 1; ++it)
	{
		it->feedForward(&(*std::next(it)));
	}
	std::vector<double>output;
	for (auto& neuron : layers.back())
	{
		output.push_back(neuron.getValue());
	}
	return output;
}

std::vector<double> NeuralNetwork::getWeights()
{
	std::vector<double> weights;
	for (auto& layer : layers)
	{
		for (auto& neuron : layer)
		{
			for (auto& connection : neuron)
			{
				weights.push_back(connection);
			}
		}
	}
	return weights;
}

void NeuralNetwork::setWeights(std::vector<double>& weights)
{
	auto it = weights.begin();
	for (auto& layer : layers)
	{
		for (auto& neuron : layer)
		{
			for (auto& connection : neuron)
			{
				connection = *it++;
			}
		}
	}
}

size_t NeuralNetwork::weightSize()
{
	size_t weightSize{ 0 };
	for (auto& layer : layers)
	{
		for (auto& neuron : layer)
		{
			weightSize += neuron.getSize();
		}
	}
	return weightSize;
}

