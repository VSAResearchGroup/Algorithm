#ifndef  COURSE_PLANNER_H
#define COURSE_PLANNER_H
#include "SerialPlanner.h"

//uses selected planning algorithm to run planning phases
class CoursePlanner
{
private: //selected planning algorithm
	IPlanner* _planner;
public:
	CoursePlanner(IPlanner* planner) :_planner(planner){}

	~CoursePlanner();

	void set_planner(IPlanner* planner);

	IPlanner& get_planner();

	//runs the first phase of the selected planning algorithm
	//startNodes: a list of pointer to the courses with no pre-requisites that could be taken by student
	//endNodes: a list of intended target courses
	/*RETURNS: a key-value pair of target courses and a jagged CourseMatrix with each row representing a 
	sequence in the requisite networks leading to target in order of length*/
	map<AugNode*, CourseMatrix> planPhase1(vector<AugNode*>& startNodes, vector<AugNode*>& endNodes);
	
	//runs the second phase of the selected planning algorithm
	//input: the output from phase 1
	//qtr: the intended starting quarter for student
	//RETURNS: a list of possible degree plans scheduled in a chronological sequence of quarters
	vector<DegreePlan> planPhase2(map<AugNode*, CourseMatrix>& input, QuarterNode start_qtr);


};
#endif