#include "Neuron.h"
#include <iostream>

Neuron::Neuron(size_t connectionNumber, double value) : connections(connectionNumber),value(value)
{}

void Neuron::actiavteConnection(size_t index, double value)
{
	if (at(index) == 0.0)
		at(index) = value;
}

void Neuron::deactivateConnection(size_t index)
{
	at(index) = 0.0;
}

void Neuron::deleteConnections()
{
	connections.clear();
}

void Neuron::popConnection()
{
	connections.pop_back();
}

void Neuron::pushConnection(double value)
{
	connections.push_back(value);
}

double& Neuron::operator[](size_t index)
{
	return connections[index];
}

double& Neuron::at(size_t index)
{
	return connections.at(index);
}

