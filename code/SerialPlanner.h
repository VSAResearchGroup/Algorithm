#ifndef SERIAL_PLANNER_H
#define SERIAL_PLANNER_H
#include "IPlanner.h"

class SerialPlanner : public IPlanner
{
private:
	map<AugNode*, vector<AugNode*>> _req_matrix;
	void augment_graph( vector<AugNode*>& endNodes);
	void probe_graph(vector<AugNode*>& startNodes, vector<AugNode*>& endNodes, map<AugNode*, CourseMatrix>& output);
	void build_req_matrix(vector<AugNode*>& startNodes);
	void scan_and_add(AugNode* crs, vector<AugNode*>& parents);
	DegreePlan chain_end_courses(map<AugNode*, CourseMatrix>& paths_map, QuarterNode qtr_yr);
	void merge_paths_into_qtr_chain(DegreePlan& qtr_chain, vector<AugNode*> path, ushort year);
	void place_crs_in_chain(DegreePlan& qtr_chain, AugNode* crs, QuarterNode qtr);
	AugNode* get_target_with_shortest_path(map<AugNode*, CourseMatrix>& paths_map);
public:
	map<AugNode*, CourseMatrix> phase1(vector<AugNode*>& startNodes, vector<AugNode*>& endNodes);
	vector<DegreePlan> phase2(map<AugNode*, CourseMatrix>& input, QuarterNode start_qtr);

	

};
#endif
