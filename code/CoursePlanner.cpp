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

map<AugNode*, CourseMatrix> CoursePlanner::planPhase1(vector<AugNode*>& startNodes, vector<AugNode*>& endNodes)
{
	return _planner->phase1(startNodes, endNodes);
}
