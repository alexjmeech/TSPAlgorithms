#ifndef TSPALGORITHMDISPATCHER_H_
#define TSPALGORITHMDISPATCHER_H_

#include "PathSummary.h"
#include "Point3D.h"
#include <unordered_set>
#include <list>
using namespace std;

enum TSPAlgorithmType
{
	BRUTE_FORCE,
	DYNAMIC_PROGRAMMING,
	END_TSP
};

typedef PathSummary(*AlgorithmFunction)(Point3D*, unsigned long);

class TSPAlgorithmDispatcher
{
private:
	static list<PathSummary> BF_Helper(Point3D*, unsigned long, unsigned long, list<unsigned long>, PathSummary);
	static PathSummary BruteForce(Point3D*, unsigned long);

	/*
	 * Dynamic programming sections adapted from William Fiset's explanation of the Dynamic Programming Solution to the TSP
	 * https://www.youtube.com/watch?v=cY4HiiFHO1o
	 */

	static void DP_FillCombinations(unsigned long, unsigned long, unsigned long, unsigned long, list<unsigned long>&);
	static list<unsigned long> DP_GenerateCombinations(unsigned long, unsigned long);
	static bool DP_VerifyNotIn(unsigned long, unsigned long);

	static PathSummary DynamicProgramming(Point3D*, unsigned long);

	TSPAlgorithmDispatcher() {}
public:
	static AlgorithmFunction getAlgorithm(TSPAlgorithmType);
};

#endif /* TSPALGORITHMDISPATCHER_H_ */
