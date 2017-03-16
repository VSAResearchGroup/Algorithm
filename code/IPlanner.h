#ifndef IPLANNER_H
#define IPLANNER_H


#include <vector>
#include <map>
#include "CourseNode.h"


using namespace std;


//an interface that defines a course planner algorithm
class IPlanner
{

public:

	//The second phase of the selected planning algorithm
	//input: a reference to the output from phase 1
	//qtr: the intended starting quarter for student
	//RETURNS: a list of possible degree plans scheduled in a chronological sequence of quarters
	virtual map<int, DegreePlan> phase2(map<AugNode*, CourseMatrix>& input, QuarterNode start_qtr, map<int, CourseNode*>* _crs_details) = 0;


};

#endif