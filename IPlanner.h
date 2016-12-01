#ifndef IPLANNER_H
#define IPLANNER_H

#define MAX_PATHS 5

#include <vector>
#include <map>
#include "CourseNode.h"


using namespace std;

typedef vector<vector<AugNode*>> CourseMatrix;

class IPlanner
{

public: 
	virtual map<AugNode*,CourseMatrix> phase1(vector<AugNode*>& startNodes, vector<AugNode*>& endNodes) = 0;
};

#endif