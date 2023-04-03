#pragma once
#include "Layer.h"

inline double TahnFunction(double x) { return tanh(x); }
inline double SigmoidFunction(double x) { return 1.0f / (1.0f + exp(-x)); }
inline double ReluFunction(double x) { return x <= 0 ? 0 : x; }

class NeuralNetwork
{
public:
	NeuralNetwork(size_t inputSize, size_t outputSize, double(*activationFunc)(double));
	NeuralNetwork(std::initializer_list<std::tuple<std::string, size_t, double(*)(double)>> layerInfo);
	// Iterators
	std::vector<Layer>::iterator begin() { return layers.begin(); }
	std::vector<Layer>::iterator end() { return layers.end(); }
	std::vector<Layer>::const_iterator cbegin() const { return layers.cbegin(); }
	std::vector<Layer>::const_iterator cend() const { return layers.cend(); }
	//Modificators
	void addLayer(std::string name, size_t layerSize, double(*activationFunc)(double));
	void deleteLayer();
	void addNode(size_t layerIndex);
	void deleteNode(size_t layerIndex);
	void deleteConnection(size_t layerIndex, size_t nodeIndex, size_t connectionIndex);
	void addConnection(size_t layerIndex, size_t nodeIndex, size_t connectionIndex);
	//Prediction
	std::vector<double> predict(std::vector<double>& input);
	std::vector<double> getWeights();
	void setWeights(std::vector<double>& weights);
	size_t weightSize();
private:
	std::vector<Layer> layers;
};