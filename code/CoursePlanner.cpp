#include "CoursePlanner.h"

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


map<int, DegreePlan> CoursePlanner::planPhase2(map<AugNode*, CourseMatrix>& input, QuarterNode start_qtr, map<int, CourseNode*>* _crs_details)
{
	return _planner->phase2(input, start_qtr, _crs_details);
}
