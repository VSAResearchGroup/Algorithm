#ifndef SERIAL_PLANNER_H
#define SERIAL_PLANNER_H
#include "IPlanner.h"

class SerialPlanner : public IPlanner
{
private:
	void augment_graph( vector<AugNode*>& endNodes);
	void probe_graph(vector<AugNode*>& startNodes, vector<AugNode*>& endNodes, map<AugNode*, CourseMatrix>& output);
	
public:
	map<AugNode*, CourseMatrix> phase1(vector<AugNode*>& startNodes, vector<AugNode*>& endNodes);

};
#endif
