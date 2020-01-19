#include "TSPAlgorithmDispatcher.h"
#include <limits>

list<PathSummary> TSPAlgorithmDispatcher::BF_Helper(Point3D* graph, unsigned long size, unsigned long prev, list<unsigned long> next, PathSummary working)
{
	list<PathSummary> result;

	unsigned long current = next.front();
	next.pop_front();

	working.addStep(current, graph[prev].distance(graph[current]));

	if (next.empty())
	{
		working.addStep(0, graph[current].distance(graph[0]));
		result.push_back(working);
	}
	else
	{
		for (unsigned long i = 0; i < next.size(); i++)
		{
			list<PathSummary> child = BF_Helper(graph, size, current, next, working);

			for (PathSummary& path : child)
			{
				result.push_back(path);
			}

			next.push_back(next.front());
			next.pop_front();
		}
	}

	return result;
}

PathSummary TSPAlgorithmDispatcher::BruteForce(Point3D* graph, unsigned long size)
{
	PathSummary best;

	list<unsigned long> next;

	for (unsigned long i = 1; i < size; i++)
	{
		next.push_back(i);
	}

	for (unsigned long i = 1; i < size; i++)
	{
		list<PathSummary> child = BF_Helper(graph, size, 0, next, PathSummary("BruteForce"));

		for (PathSummary& path : child)
		{
			if (best.getAlgorithm().empty() || path.getPathDistance() < best.getPathDistance())
			{
				best = path;
			}
		}

		next.push_back(next.front());
		next.pop_front();
	}

	return best;
}

void TSPAlgorithmDispatcher::DP_FillCombinations(unsigned long nodes, unsigned long base, unsigned long included, unsigned long size, list<unsigned long>& subsets)
{
	unsigned long elementsLeftToPick = size - base;

	if (elementsLeftToPick < included)
	{
		return;
	}

	if (included == 0)
	{
		subsets.push_back(nodes);
	}
	else
	{
		for (unsigned long i = base; i < size; i++)
		{
			nodes ^= (1 << i);
			DP_FillCombinations(nodes, i + 1, included - 1, size, subsets);
			nodes ^= (1 << i);
		}
	}
}

list<unsigned long> TSPAlgorithmDispatcher::DP_GenerateCombinations(unsigned long included, unsigned long size)
{
	list<unsigned long> subsets;
	DP_FillCombinations(0, 0, included, size, subsets);

	return subsets;
}

bool TSPAlgorithmDispatcher::DP_VerifyNotIn(unsigned long element, unsigned long subset)
{
	return ((1 << element) & subset) == 0;
}

PathSummary TSPAlgorithmDispatcher::DynamicProgramming(Point3D* graph, unsigned long size)
{
	double** table = new double*[size];

	for (unsigned long i = 0; i < size; i++)
	{
		table[i] = new double[1 << size];
	}

	PathSummary path("DynamicProgramming");

	for (unsigned long end = 1; end < size; end++)
	{
		table[end][1 | (1 << end)] = graph[0].distance(graph[end]);
	}

	for (unsigned long included = 3; included <= size; included++)
	{
		for (unsigned long subset : DP_GenerateCombinations(included, size))
		{
			if (DP_VerifyNotIn(0, subset))
			{
				continue;
			}

			for (unsigned long next = 1; next < size; next++)
			{
				if (DP_VerifyNotIn(next, subset))
				{
					continue;
				}

				unsigned long subsetWithoutNext = subset ^ (1 << next);
				double minDist = numeric_limits<double>::max();

				for (unsigned long end = 1; end < size; end++)
				{
					if (end == next || DP_VerifyNotIn(end, subset))
					{
						continue;
					}

					double newDistance = table[end][subsetWithoutNext] + graph[end].distance(graph[next]);

					if (newDistance < minDist)
					{
						minDist = newDistance;
					}
				}

				table[next][subset] = minDist;
			}
		}
	}

	unsigned long lastIndex = 0;
	unsigned long state = (1 << size) - 1;

	for (unsigned long i = 1; i < size; i++)
	{
		unsigned long index = numeric_limits<unsigned long>::max();

		for (unsigned long j = 1; j < size; j++)
		{
			if (DP_VerifyNotIn(j, state))
			{
				continue;
			}

			if (index == numeric_limits<unsigned long>::max())
			{
				index = j;
			}

			double prevDist = table[index][state] + graph[index].distance(graph[lastIndex]);
			double newDist = table[j][state] + graph[j].distance(graph[lastIndex]);

			if (newDist < prevDist)
			{
				index = j;
			}
		}

		path.addStep(index, graph[lastIndex].distance(graph[index]));
		state = state ^ (1 << index);
		lastIndex = index;
	}

	path.addStep(0, graph[lastIndex].distance(graph[0]));

	for (unsigned long i = 0; i < size; i++)
	{
		delete[] table[i];
	}

	delete[] table;

	return path;
}

AlgorithmFunction TSPAlgorithmDispatcher::getAlgorithm(TSPAlgorithmType type)
{
	switch (type)
	{
	case BRUTE_FORCE:
		return &(TSPAlgorithmDispatcher::BruteForce);
	case DYNAMIC_PROGRAMMING:
		return &(TSPAlgorithmDispatcher::DynamicProgramming);
	default:
		return nullptr;
	}
}
