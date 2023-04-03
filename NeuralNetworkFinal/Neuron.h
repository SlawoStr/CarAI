#pragma once
#include <vector>

class Neuron
{
public:
	Neuron(size_t connectionNumber, double value = 0.0);
	// Connection settings
	void actiavteConnection(size_t index, double value);
	void deactivateConnection(size_t index);
	void deleteConnections();
	void popConnection();
	void pushConnection(double value);
	// Iterators
	std::vector<double>::iterator begin() { return connections.begin(); }
	std::vector<double>::iterator end() { return connections.end(); }
	std::vector<double>::const_iterator cbegin() const { return connections.cbegin(); }
	std::vector<double>::const_iterator cend() const { return connections.cend(); }
	// Getters
	double getValue() { return value; }
	size_t getSize() { return connections.size(); }
	// Setters
	void setValue(double value) { this->value = value; }
	// Elements Access
	double& operator[] (size_t index);
	double& at(size_t index);
private:
	std::vector<double> connections;
	double value;
};