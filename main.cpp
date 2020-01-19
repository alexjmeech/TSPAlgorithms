#include "TSPAlgorithmDispatcher.h"
#include <cstring>
#include "TSP.h"

int main(int argv, char** argc)
{
	Algorithm* a = new TSP();
	a->Load("input/");

	if (argv > 1 && strcasecmp(argc[1], "DP") == 0)
	{
		a->Select(TSPAlgorithmType::DYNAMIC_PROGRAMMING);
		a->Execute();
		a->Display();
		a->Stats();
		a->Save();

		return 0;
	}

	for (int i = TSPAlgorithmType::BRUTE_FORCE; i < TSPAlgorithmType::END_TSP; i++)
	{
		a->Select(i);
		a->Execute();
		a->Display();
		a->Stats();
		a->Save();
	}

	return 0;
}
