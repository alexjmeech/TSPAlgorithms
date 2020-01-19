#ifndef TSP_H_
#define TSP_H_

#include "Algorithm.h"
#include "Point3D.h"
#include "PathSummary.h"
#include <ostream>

class TSP : public Algorithm
{
private:
	Point3D* graph;
	unsigned long size;

	PathSummary summary;
	long execution;

	PathSummary(*algorithm)(Point3D*, unsigned long);

	virtual void OutputSummary(ostream&, PathSummary&, bool);
public:
	virtual void Load(string);
	virtual void Execute();
	virtual void Display();
	virtual void Stats();
	virtual void Select(int);
	virtual void Save();
	virtual void Configure();

	virtual ~TSP();
};

#endif /* TSP_H_ */
