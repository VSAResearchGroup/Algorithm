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
	//tod_pref:bitflag for preferred time of day(morning, afternoon, evening, or either)
	//max_credit_pref : maximum number of credits to be taken per quarter
	//max_budget_pref : approximate budget limit per quarter
	//RETURNS: a list of possible degree plans scheduled in a chronological sequence of quarters
	virtual map<int,DegreePlan> phase2(map<AugNode*, CourseMatrix>& input,QuarterNode start_qtr,TIME_OF_DAY tod_pref, int max_credit_pref, float max_budget_pref) = 0;


};

#endif