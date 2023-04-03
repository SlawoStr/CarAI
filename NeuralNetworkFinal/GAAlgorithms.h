#pragma once
#include <vector>

// Selection Algorithms
std::vector<std::pair<int, int>> selectBest(int bestParents,int parentNumber, const std::vector<std::pair<int, double>>& modelFitness);
std::vector<std::pair<int, int>> selectWheel(int parentNumber, const std::vector<std::pair<int, double>>& modelFitness);
// Crossover Algorithms
void crossoverSPC(double crossoverProbability, std::vector<std::vector<double>>& weights);
void crossoverMPC(double crossoverProbability, std::vector<std::vector<double>>& weights);
// Mutation
void mutationADD(double mutationProbability,double min,double max, std::vector<std::vector<double>>& weights);
void mutationNEW(double mutationProbability, std::vector<std::vector<double>>& weights);