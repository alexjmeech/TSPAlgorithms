#include "PathSummary.h"

PathSummary::PathSummary()
{
	distance = 0;
}

PathSummary::PathSummary(string algorithm)
{
	algorithmName = algorithm;
	distance = 0;
}

const string& PathSummary::getAlgorithm()
{
	return algorithmName;
}

const list<unsigned long>& PathSummary::getPath()
{
	return path;
}

double PathSummary::getPathDistance()
{
	return distance;
}

void PathSummary::addStep(unsigned long node, double stepDistance)
{
	path.push_back(node);
	distance += stepDistance;
}
