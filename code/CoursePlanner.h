#ifndef  COURSE_PLANNER_H
#define COURSE_PLANNER_H
#include "SerialPlanner.h"

//uses selected planning algorithm to run planning phases
class CoursePlanner
{
private: //selected planning algorithm
	IPlanner* _planner;
public:
	CoursePlanner(IPlanner* planner) :_planner(planner) {}

	~CoursePlanner();

	void set_planner(IPlanner* planner);

	IPlanner& get_planner();



	//runs the second phase of the selected planning algorithm
	//input: the output from phase 1
	//qtr: the intended starting quarter for student
	//RETURNS: a list of possible degree plans scheduled in a chronological sequence of quarters
	map<int, DegreePlan> planPhase2(map<AugNode*, CourseMatrix>& input, QuarterNode start_qtr, map<int, CourseNode*>* _crs_details);


};
#endif