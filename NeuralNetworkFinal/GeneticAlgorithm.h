#pragma once
#include "NeuralNetwork.h"

class GeneticAlgorithm
{
public:
	GeneticAlgorithm(std::initializer_list<std::tuple<std::string, size_t, double(*)(double)>> layerInfo,size_t modelsNumber);
	// Predictions
	std::vector<double> predictOne(size_t modelIndex, std::vector<double>& input);
	std::vector<std::vector<double>> predictMany(std::vector<size_t> modelIndex, std::vector<std::vector<double>>& inputs);
	std::vector<std::vector<double>> predictAll(std::vector<std::vector<double>>&inputs);
	
	void createNewGeneration(std::vector<std::pair<int, double>> modelsFitness);

	// Setting functions
	template<typename Func, typename... Args>
	void setMutation(Func F, Args... args)
	{
		mutationFunc = std::bind(F,std::forward<Args>(args)...,std::placeholders::_1);
	}
	template<typename Func, typename... Args>
	void setCrossover(Func F, Args... args)
	{
		crossoverFunc = std::bind(F, std::forward<Args>(args)..., std::placeholders::_1);
	}
	template<typename Func, typename... Args>
	void setSelection(Func F, Args... args)
	{
		selectionFunc = std::bind(F, std::forward<Args>(args)..., std::placeholders::_1);
	}
private:
	std::vector<NeuralNetwork> network;
	std::function<void(std::vector<std::vector<double>>&) > mutationFunc;
	std::function<void(std::vector<std::vector<double>>& weightsPair)> crossoverFunc;
	std::function<std::vector<std::pair<int, int>>(const std::vector<std::pair<int, double>>&)> selectionFunc;
};
