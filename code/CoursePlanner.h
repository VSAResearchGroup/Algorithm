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

	
	
	//runs the second phase of the selected planning algorithm
	//input: the output from phase 1
	//qtr: the intended starting quarter for student
	//tod_pref: preferred time of day(day, evening, or either)
	//max_credit_pref : maximum number of credits to be taken per quarter
	//max_budget_pref : approximate budget limit per quarter
	//RETURNS: a list of possible degree plans scheduled in a chronological sequence of quarters
	map<rank_index,DegreePlan> planPhase2(map<AugNode*, CourseMatrix>& input, QuarterNode start_qtr,TIME_OF_DAY tod_pref, int max_credit_pref, float max_budget_pref);


};
#endif