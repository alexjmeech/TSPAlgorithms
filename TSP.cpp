#include "TSPAlgorithmDispatcher.h"
#include "FileIOUtil.hpp"
#include <iostream>
#include <chrono>
#include "TSP.h"

void TSP::Load(string directory)
{
	if (graph != nullptr)
	{
		delete[] graph;
	}

	execution = 0;

	FileIOUtil<Point3D>::readData(directory, string("graph.txt"), graph, size, &(Point3D::fromInput));
}

void TSP::Execute()
{
	if (algorithm == nullptr || graph == nullptr)
	{
		return;
	}

	auto start = std::chrono::high_resolution_clock::now();
	summary = (*algorithm)(graph, size);
	auto end = std::chrono::high_resolution_clock::now();
	execution = (end - start).count();
}

void TSP::Display()
{
	if (summary.getAlgorithm().empty())
	{
		return;
	}

	cout << "Path:" << '\n';
	cout << 1;

	for (unsigned long node : summary.getPath())
	{
		cout << " -> " << (node + 1);
	}

	cout << '\n' << '\n';

	cout.flush();
}

void TSP::Stats()
{
	if (summary.getAlgorithm().empty())
	{
		return;
	}

	OutputSummary(cout, summary, false);
	cout.flush();
}

void TSP::Select(int algorithmType)
{
	if (algorithmType < 0 || algorithmType >= TSPAlgorithmType::END_TSP)
	{
		return;
	}

	execution = 0;
	summary = PathSummary();
	algorithm = TSPAlgorithmDispatcher::getAlgorithm((TSPAlgorithmType)algorithmType);
}

void TSP::Save()
{
	if (summary.getAlgorithm().empty())
	{
		return;
	}

	ostringstream buffer;

	OutputSummary(buffer, summary, true);

	FileIOUtil<Point3D>::writeBuffer("output", summary.getAlgorithm() + "_Path.txt", buffer);
}

void TSP::Configure() {}

void TSP::OutputSummary(ostream& output, PathSummary& summary, bool path)
{
	output << "Algorithm:" << '\n';
	output << summary.getAlgorithm() << '\n' << '\n';

	if (path)
	{
		output << "Path:" << '\n';
		output << 1;

		for (unsigned long node : summary.getPath())
		{
			output << " -> " << (node + 1);
		}

		output << '\n' << '\n';
	}

	output << "Nodes in Path:" << '\n';
	output << (summary.getPath().size() + 1) << '\n' << '\n';

	output << "Distance of Path:" << '\n';
	output << summary.getPathDistance() << '\n' << '\n';

	output << "Execution Time:" << '\n';
	output << execution << " nanoseconds" << '\n' << '\n' << '\n';
}

TSP::~TSP()
{
	if (graph != nullptr)
	{
		delete[] graph;
	}

	if (algorithm != nullptr)
	{
		algorithm = nullptr;
	}

	execution = 0;
	summary = PathSummary();
}
