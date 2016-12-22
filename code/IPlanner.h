#ifndef IPLANNER_H
#define IPLANNER_H


#include <vector>
#include <map>
#include "CourseNode.h"


using namespace std;

typedef vector<vector<AugNode*>> CourseMatrix;
typedef unsigned short ushort;
typedef map<QuarterNode, vector<AugNode*>> DegreePlan;

class IPlanner
{

public: 
	virtual map<AugNode*,CourseMatrix> phase1(vector<AugNode*>& startNodes, vector<AugNode*>& endNodes) = 0;

	virtual vector<DegreePlan> phase2(map<AugNode*, CourseMatrix>& input,QuarterNode start_qtr) = 0;


};

#endif