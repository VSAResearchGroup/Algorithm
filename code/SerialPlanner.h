#ifndef SERIAL_PLANNER_H
#define SERIAL_PLANNER_H
#include "IPlanner.h"

//a serial implementation of the course planner
class SerialPlanner : public IPlanner
{
private:
	//a requisite matrix in form of a key-value pair where every course maps to its post-requisites
	map<AugNode*, vector<pair<bool,AugNode*>>> _req_matrix;

	//Populates requisite network with additional data needed to aid transversal
	//endNodes: a reference to the list of intended target courses from which the network augmentation begins
	void augment_graph( vector<AugNode*>& endNodes);

	//sends probes to transverse graph in a single pass and derive paths to all target courses
	//startNodes: a reference to the list of courses with no pre-requisites used to terminate the probes
	//endNodes: a reference to the list of intended target courses used to start the probes
	//output: a reference to the final output of phase 1
	void probe_graph(vector<AugNode*>& startNodes, vector<AugNode*>& endNodes, map<AugNode*, CourseMatrix>& output);

	//populates the requisite matrix
	//startNodes: a reference to the list of courses with no pre-requisites used to start the requisite matric
	void build_req_matrix(vector<AugNode*>& startNodes);
	
	//recursively scans for all post-requisites of a course and that of its respective children
	//crs: course whose post-requisite is being scanned for
	//parents: used to keep track of previously visited courses in order to update their post-requisites
	void scan_and_add(AugNode* crs, vector<AugNode*>& parents);

	//starts a new degree plan by fixing only the target courses in their appropriate quarters
	//paths_map: a reference to the output from phase 1
	//qtr_yr: intended starting quarter
	DegreePlan chain_end_courses(map<AugNode*, CourseMatrix>& paths_map, QuarterNode qtr_yr);

	//merges a list of courses into current degree plan considering only quarters and course requisite relationships
	//qtr_chain: the current degree plan in which courses are being merged into
	//path: a sequence of courses in the requisite network
	//year: intended start year
	void merge_paths_into_qtr_chain(DegreePlan& qtr_chain, vector<AugNode*> path, ushort year);

	//recursively looks for an appropriate quarter to place a course even if it has to pre-empt a post requisite
	//qtr_chain: degree plan in which course is being placed
	//crs: course to be placed
	//qtr: originally intended qtr to fix course
	void place_crs_in_chain(DegreePlan& qtr_chain, AugNode* crs, QuarterNode qtr);

	//get the target course with the shortest sequence of courses 
	//paths_map: a reference to the output from phase 1
	AugNode* get_target_with_shortest_path(map<AugNode*, CourseMatrix>& paths_map);

	//resets the boolean flags that determines if a course has been included in a tentative degree plan
	void reset_queued_flags();

	//generate new plans in situations where quarters contain courses with schedule clashes
	//plans: a reference to the list of generated degree plans
	void resolve_clashes(vector<DegreePlan>& plans);

	//recursively generate new permutations of conflict-free degree plans (if any)
	//plans: a reference to the list of generated degree plans
	//single_plan: plan under examination
	//start_index: the index of the quarter to start search from
	//is_included: a flag to check if plan is already included in the list of degree plans
	void reorder_for_clash(vector<DegreePlan>& plans, DegreePlan& single_plan, int start_index,bool is_included);
public:
	//The first phase of the selected planning algorithm
	//startNodes: a reference to the list of courses with no pre-requisites that could be taken by student
	//endNodes: a reference to the list of intended target courses
	/*RETURNS: a key-value pair of target courses and a jagged CourseMatrix with each row representing a
	sequence in the requisite networks leading to target in order of length*/
	map<AugNode*, CourseMatrix> phase1(vector<AugNode*>& startNodes, vector<AugNode*>& endNodes);

	//The second phase of the selected planning algorithm
	//input: a reference to the output from phase 1
	//qtr: the intended starting quarter for student
	//RETURNS: a list of possible degree plans scheduled in a chronological sequence of quarters
	vector<DegreePlan> phase2(map<AugNode*, CourseMatrix>& input, QuarterNode start_qtr);
	

	

};
#endif
