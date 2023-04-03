#include "Layer.h"
#include <stdexcept>


Layer::Layer(std::string layerName, size_t neuronNumber, size_t connectionNumber, double(*func)(double), bool bias) : layerName(layerName),bias(bias)
{
	setActivationFunction(func);

	// Add additional bias neuron
	if (bias)
	{
		neuronNumber++;
	}
	// Add neurons and generate connections weights
	for (size_t i = 0; i < neuronNumber; ++i)
	{
		neurons.emplace_back(connectionNumber);
		for (size_t j = 0; j < connectionNumber; ++j)
		{
			neurons.back()[j] = generator();
		}
	}
	// Bias neuron should have value equal to 1.0
	if (bias)
	{
		neurons.back().setValue(1.0);
	}
}

void Layer::deleteNeuron(Layer* prev)
{
	// Bias layer require atleast 2 neurons
	if (bias && neurons.size() == 2)
	{
		return;
	}
	// Output layer require atleast 1 neuron
	else if(!bias && neurons.size() == 1)
	{
		return;
	}
	// Delete neuron before bias from vector;
	neurons.erase(neurons.end() - 2);
	if (prev != nullptr)
	{
		// Delete connection to neuron
		for (auto& neuron : *prev)
		{
			neuron.popConnection();
		}
	}
}

void Layer::addNeuron(Layer* prev, Layer* next)
{
	// Output layer doesnt need connections
	if (next == nullptr)
	{
		neurons.emplace_back(0, 0.0);
	}
	else
	{
		auto pos = neurons.emplace(neurons.end() - 1, next->getSize() - next->isBias());
		for (auto& connection : *pos)
		{
			connection = generator();
		}
	}
	if (prev != nullptr)
	{
		// Create new connection from prev layer
		for (auto& neuron : *prev)
		{
			neuron.pushConnection(generator());
		}
	}
}

void Layer::restartConnection(size_t connectionSize)
{
	for (auto& neuron : neurons)
	{
		neuron.deleteConnections();
		for (size_t i = 0; i < connectionSize; ++i)
		{
			neuron.pushConnection(generator());
		}
	}
}

void Layer::addConnection(size_t nodeIndex, size_t connectionIndex)
{
	if (nodeIndex >= neurons.size())
		throw std::out_of_range("This neuron doesnt exist");
	neurons[nodeIndex].deactivateConnection(connectionIndex);
}

void Layer::deleteConnection(size_t nodeIndex, size_t connectionIndex)
{
	if (nodeIndex >= neurons.size())
		throw std::out_of_range("This neuron doesnt exist");
	neurons[nodeIndex].actiavteConnection(connectionIndex,generator());
}

Neuron& Layer::operator[](size_t index)
{
	return neurons[index];
}

Neuron& Layer::at(size_t index)
{
	return neurons.at(index);
}

void Layer::feedForward(Layer* nextLayer)
{
	size_t nextLayerSize = nextLayer->getSize() - nextLayer->isBias();
	for (size_t i = 0; i < nextLayerSize; ++i)
	{
		double sum{ 0.0 };
		for (auto& neuron : neurons)
		{
			sum += neuron.getValue() * neuron[i];
		}
		(*nextLayer)[i].setValue(activationFunction(sum));
	}
}

RandomDoubleGenerator Layer::generator(-1.0, 1.0f);
