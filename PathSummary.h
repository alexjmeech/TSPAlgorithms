#ifndef PATHSUMMARY_H_
#define PATHSUMMARY_H_

#include <string>
#include <list>
using namespace std;

class PathSummary
{
private:
	string algorithmName;
	list<unsigned long> path;
	double distance;
public:
	PathSummary();

	PathSummary(string);

	const string& getAlgorithm();

	const list<unsigned long>& getPath();

	double getPathDistance();

	void addStep(unsigned long, double = 0);
};

#endif /* PATHSUMMARY_H_ */
