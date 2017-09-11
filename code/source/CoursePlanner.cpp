#include "../headers/CoursePlanner.h"

CoursePlanner::~CoursePlanner()
{
	delete _planner;
}

void CoursePlanner::set_planner(IPlanner* planner)
{
	this->_planner = planner;
}

IPlanner& CoursePlanner::get_planner()
{
	return *_planner;
}


map<rank_index,DegreePlan> CoursePlanner::planPhase2(map<AugNode*, CourseMatrix>& input, QuarterNode start_qtr, TIME_OF_DAY tod_pref, int max_credit_pref, float max_budget_pref)
{
	return _planner->phase2(input, start_qtr,tod_pref,max_credit_pref,max_budget_pref);
}
