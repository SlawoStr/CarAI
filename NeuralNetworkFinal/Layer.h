#pragma once
#include <functional>
#include "Neuron.h"
#include "RandomNumberGenerator.h"

class Layer
{
public:
	Layer(std::string layerName, size_t neuronNumber,size_t connectionNumber, double(*func)(double), bool bias);
	// Neuron settings
	void deleteNeuron(Layer* prev);
	void addNeuron(Layer* prev, Layer* next);
	void restartConnection(size_t connectionSize);
	void addConnection(size_t nodeIndex, size_t connectionIndex);
	void deleteConnection(size_t nodeIndex, size_t connectionIndex);
	// Iterators
	std::vector<Neuron>::iterator begin() { return neurons.begin(); }
	std::vector<Neuron>::iterator end() { return neurons.end(); }
	std::vector<Neuron>::const_iterator cbegin() const { return neurons.cbegin(); }
	std::vector<Neuron>::const_iterator cend() const { return neurons.cend(); }
	// Getters
	size_t getSize()const { return neurons.size(); }
	std::string getName()const { return layerName; }
	// Setters
	void setName(std::string layerName) { this->layerName = layerName; }
	void setActivationFunction(double(*func)(double)) { activationFunction = [=](double value) {return func(value); }; }
	bool isBias() { return bias; }
	// Elements Access
	Neuron& operator[] (size_t index);
	Neuron& at(size_t index);
	// Predictions
	void feedForward(Layer* nextLayer);

private:
	static RandomDoubleGenerator generator;
	std::function<double(double)> activationFunction;
	std::string layerName;
	std::vector<Neuron> neurons;
	bool bias;
};