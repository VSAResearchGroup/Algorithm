#ifndef  COURSE_PLANNER_H
#define COURSE_PLANNER_H
#include "SerialPlanner.h"


class CoursePlanner
{
private:
	IPlanner* _planner;
public:
	CoursePlanner(IPlanner* planner) :_planner(planner){}

	~CoursePlanner();

	void set_planner(IPlanner* planner);

	IPlanner& get_planner();

	map<AugNode*, CourseMatrix> planPhase1(vector<AugNode*>& startNodes, vector<AugNode*>& endNodes);

	vector<DegreePlan> planPhase2(map<AugNode*, CourseMatrix> input, QuarterNode start_qtr);


};
#endif