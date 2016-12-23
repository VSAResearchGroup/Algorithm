#ifndef IPLANNER_H
#define IPLANNER_H


#include <vector>
#include <map>
#include "CourseNode.h"


using namespace std;

typedef vector<vector<AugNode*>> CourseMatrix;
typedef unsigned short ushort;
typedef map<QuarterNode, vector<AugNode*>> DegreePlan;
//an interface that defines a course planner algorithm
class IPlanner
{

public: 
	//The first phase of the selected planning algorithm
	//startNodes: a reference to the list of courses with no pre-requisites that could be taken by student
	//endNodes: a reference to the list of intended target courses
	/*RETURNS: a key-value pair of target courses and a jagged CourseMatrix with each row representing a
	sequence in the requisite networks leading to target in order of length*/
	virtual map<AugNode*,CourseMatrix> phase1(vector<AugNode*>& startNodes, vector<AugNode*>& endNodes) = 0;

	//The second phase of the selected planning algorithm
	//input: a reference to the output from phase 1
	//qtr: the intended starting quarter for student
	//RETURNS: a list of possible degree plans scheduled in a chronological sequence of quarters
	virtual vector<DegreePlan> phase2(map<AugNode*, CourseMatrix>& input,QuarterNode start_qtr) = 0;


};

#endif