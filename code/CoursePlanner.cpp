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

vector<DegreePlan> CoursePlanner::planPhase2(map<AugNode*, CourseMatrix> input, QuarterNode start_qtr)
{
	return _planner->phase2(input, start_qtr);
}