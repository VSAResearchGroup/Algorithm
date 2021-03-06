#ifndef SERIAL_PLANNER_H
#define SERIAL_PLANNER_H
#include "IPlanner.h"
#include <ctime>

//a serial implementation of the course planner
class SerialPlanner : public IPlanner
{
private:

	map<int, CourseNode*>* _crs_details;
	map<rank_index, DegreePlan> final_output;


	int glb_plan_id = 0;
	
	

	//creates an empty degree plan with stock quarters
	//qtr_yr: intended starting quarter
	DegreePlan emit_new_plan(QuarterNode qtr_yr);

	//merges a list of courses into current degree plan considering only quarters and course requisite relationships
	//qtr_chain: the current degree plan in which courses are being merged into
	//path: a sequence of courses in the requisite network
	//year: intended start year
	void merge_paths_into_qtr_chain(DegreePlan& qtr_chain, vector<AugNode*> path, ushort year,int max_credit_pref);

	//recursively looks for an appropriate quarter to place a course even if it has to pre-empt a post requisite
	//qtr_chain: degree plan in which course is being placed
	//crs: course to be placed
	//qtr: originally intended qtr to fix course
	void place_crs_in_chain(DegreePlan& qtr_chain, CourseNode* crs, QuarterNode qtr, int max_credit_pref);


	//resets the boolean flags that determines if a course has been included in a tentative degree plan
	void reset_queued_flags();

	//use the prunner class to sort the plans in order of suitability
	//tod_pref: preferred time of day(day, evening, or either)
	//max_credit_pref : maximum number of credits to be taken per quarter
	//max_budget_pref : approximate budget limit per quarter
	void rank_plans(TIME_OF_DAY tod_pref, int max_credit_pref, float max_budget_pref);

	//used to determine the path to select based on certain heuirstics
	//paths: the list of paths to the target course
	//RETURNS: the index of the selected path
	int heuristic_path_section(CourseMatrix paths);
public:
	
	//_crs_details: a pointer to the peripheral details of every course needed for the recommendation engine
	SerialPlanner(map<int, CourseNode*>* _crs_details);
	//The second phase of the selected planning algorithm
	//input: a reference to the output from phase 1
	//qtr: the intended starting quarter for student
    //tod_pref: bitflag for preferred time of day(morning, afternoon, evening, or either)
	//max_credit_pref : maximum number of credits to be taken per quarter
	//max_budget_pref : approximate budget limit per quarter
	//RETURNS: a ranked list of possible degree plans scheduled in a chronological sequence of quarters
	map<rank_index,DegreePlan> phase2(map<AugNode*, CourseMatrix>& input, QuarterNode start_qtr, TIME_OF_DAY tod_pref, int max_credit_pref, float max_budget_pref);
	

	

};
#endif
