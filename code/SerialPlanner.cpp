#include "SerialPlanner.h"
#include "PlanPrunner.h"
#include <set>
#include <fstream>
#include <algorithm>
#include <tuple>
#include <iostream>
//fstream logger;

int calls = 0;
bool call_flag = true;

SerialPlanner::SerialPlanner(map<int, CourseNode*>* _crs_details)
{
	this->_crs_details = _crs_details;
	srand(time(NULL));
}

void SerialPlanner::reset_queued_flags()
{
	for (auto& crs : (*_crs_details))
	{
		crs.second->isQueued = false;
	}
}




DegreePlan SerialPlanner::emit_new_plan(QuarterNode start_qtr)
{
	DegreePlan qtr_map;
	unsigned short start_yr = start_qtr.year;
	QuarterNode temp = start_qtr;
	for (int i = 0; i < 4; i++)
	{
		qtr_map.insert(pair<QuarterNode, vector<CourseNode*>>(start_qtr, vector<CourseNode*>()));
		start_qtr = start_qtr.next_qtr();
	}

	return qtr_map;
}


void SerialPlanner::merge_paths_into_qtr_chain(DegreePlan& qtr_chain, vector<AugNode*> path, ushort year, int max_credit_pref)
{

	for (auto crs = path.begin(); crs != path.end(); crs++)
	{

		for (auto crs_id : (*crs)->course_ids)
		{
			CourseNode* course = (*_crs_details)[crs_id];
			if (!course->isQueued)
			{
				QuarterNode qtr;
				qtr.quarter = (*_crs_details)[crs_id]->get_current_schedule().qtr;
				qtr.year = year;
						//logger << "Fresh course about to be placed: " << course->course_code << "qtr: " << qtr.quarter <<" yr: "<< year<<endl;
				place_crs_in_chain(qtr_chain, course, qtr,max_credit_pref);
				if (!call_flag)
				{
					break;
				}
			}
		}

		if (!call_flag)
		{
			break;
		}
	}
}

void SerialPlanner::place_crs_in_chain(DegreePlan& qtr_chain, CourseNode* crs, QuarterNode qtr, int max_credit_pref)
{
	++calls;
	if (calls > 15000)
	{
		call_flag = false;
		return;
	}
	vector<CourseNode*>* test_qtr_crses = nullptr;
	if (qtr < qtr_chain.begin()->first)
	{
		qtr.year++; //meant for the next year
	}
	if (qtr_chain.find(qtr) == qtr_chain.end())
	{
		QuarterNode to_add = qtr_chain.rbegin()->first;
		while (to_add != qtr)
		{
			to_add = to_add.next_qtr();
			qtr_chain.insert(pair<QuarterNode, vector<CourseNode*>>(to_add, vector<CourseNode*>()));
		}
	}

	test_qtr_crses = &qtr_chain[qtr];

	bool add_crs = true;
	/*first quarter is where u are being deferred and second quarter is where you are  begin taken*/
	set<tuple<CourseNode*, QuarterNode,QuarterNode>> crs_for_deference; //using set to avoid duplicate entries in the deferrence list
	auto test_qtr_itr = qtr_chain.find(qtr);

	//check all quarters before current quarter under consideration
	for (auto qtr_itr = qtr_chain.begin(); qtr_itr != test_qtr_itr; qtr_itr++)
	{
		for (auto c = qtr_itr->second.begin(); c != qtr_itr->second.end(); c++)
		{
			CourseNode* c_val = *c;
			auto req_ptr = find((*_crs_details)[crs->course_code]->post_reqs.begin(), (*_crs_details)[crs->course_code]->post_reqs.end(), c_val->course_code);
			if (req_ptr != (*_crs_details)[crs->course_code]->post_reqs.end())
			{//crs is a pre-requisite to c 
				//logger << "BEFORE: Crs: " << crs->course_code << " is a pre-requisite to C: " << c_val->course_code << " qtr: "<< qtr_itr->first.quarter << "yr:" << qtr_itr->first.year<< endl; 
				crs_for_deference.insert(tuple<CourseNode*,QuarterNode, QuarterNode>(*c,qtr_itr->first, get_crs_next_feasible_qtr(c_val,qtr))); //add c to a list of courses to be deferred
			}

		}
	}
	if (!test_qtr_crses->empty())
	{//quarter under consideration have courses already assigned to it

		for (auto c = test_qtr_crses->begin(); c != test_qtr_crses->end(); c++)
		{
			CourseNode* c_val = *c;
			auto req_ptr = find((*_crs_details)[c_val->course_code]->post_reqs.begin(), (*_crs_details)[c_val->course_code]->post_reqs.end(), crs->course_code);


			auto rev_req_ptr = find((*_crs_details)[crs->course_code]->post_reqs.begin(), (*_crs_details)[crs->course_code]->post_reqs.end(), c_val->course_code);
			if (req_ptr != (*_crs_details)[c_val ->course_code]->post_reqs.end())
			{ //c is a pre-requisite  to crs
				//logger << "PRESENT: c: " <<   c_val->course_code << " is a pre-requisite to Crs: " << crs->course_code << " qtr: " << qtr.quarter << "yr:" << qtr.year << endl;
				add_crs = false; //do not add crs to this quarter
				break;
			}
			else if (rev_req_ptr != (*_crs_details)[crs->course_code]->post_reqs.end())
			{//crs is a pre-requisite to c 
				//logger << "PRESENT: Crs: " << crs->course_code << " is a pre-requisite to C: " << c_val->course_code << " qtr: " << qtr.quarter << "yr:" << qtr.year << endl;
				crs_for_deference.insert(tuple<CourseNode*,QuarterNode, QuarterNode>(c_val, qtr,get_crs_next_feasible_qtr(c_val, qtr))); //add c to a list of courses to be deferred
			}
		}

	}
	if (add_crs)
	{
		//check quarters after the one under consideration
		for (auto next_qtrs = ++test_qtr_itr; next_qtrs != qtr_chain.end(); next_qtrs++)
		{
			for (auto next_qtr_crs = next_qtrs->second.begin(); next_qtr_crs != next_qtrs->second.end(); next_qtr_crs++)
			{
				CourseNode* c_val = *next_qtr_crs;
				auto req_ptr = find((*_crs_details)[c_val->course_code]->post_reqs.begin(), (*_crs_details)[c_val->course_code]->post_reqs.end(), crs->course_code);
				if (req_ptr != (*_crs_details)[c_val->course_code]->post_reqs.end())
				{
					//c_val is a pre-requisite to crs
					//logger << "FUTURE: c: " << c_val->course_code << " is a pre-requisite to Crs: " << crs->course_code << " qtr: " << next_qtrs->first.quarter << "yr:" << next_qtrs->first.year << endl;
					QuarterNode prev_qtr = get_crs_prev_feasible_qtr(c_val, qtr);
					if (prev_qtr < qtr_chain.begin()->first)
					{
						add_crs = false;
						break;
					}
					
					crs_for_deference.insert(tuple<CourseNode*,QuarterNode, QuarterNode>(c_val,next_qtrs->first,prev_qtr));
					

					
				}
			}
			
		}
	}


	if (!add_crs)
	{
		//if crs is not going to be added, include it in the deference list
		crs_for_deference.insert(tuple<CourseNode*, QuarterNode,QuarterNode>(crs, qtr,get_crs_next_feasible_qtr(crs,qtr)));
	}
	for (auto deferee = crs_for_deference.begin(); deferee != crs_for_deference.end(); deferee++)
	{
		//clean up deferred courses from current quarter chain
		vector<CourseNode*>& qtr_crses = qtr_chain[get<1>(*deferee)];
		auto remove = find(qtr_crses.begin(), qtr_crses.end(), get<0>(*deferee));
		if (remove != qtr_crses.end())
		{ //remove if present
			qtr_crses.erase(remove);
		}
	}

	if (add_crs)
	{

		//check if maximum credits for a quarter have been exceeded
		float total_credits = 0.0;
		for (auto& qtr_crs : *test_qtr_crses)
		{
			total_credits += qtr_crs->compute_no_of_credits();
		}
		if (total_credits + crs->compute_no_of_credits() > (min(MAX_POSSIBLE_CREDITS_QTR,max_credit_pref)))
		{
			//maximum credits will be exceeded of quarter is added
			//crs is not going to be added, include it in the deference list
			crs_for_deference.insert(tuple<CourseNode*,QuarterNode, QuarterNode>(crs,qtr, get_crs_next_feasible_qtr(crs, qtr)));
			add_crs = false;
				//logger << "Maximum credits exceeded: " << crs->course_code << " qtr: " << qtr.quarter << "yr:" << qtr.year << " Credits: " << (total_credits + crs->compute_no_of_credits()) <<endl;

		}
		else
		{

			//check for any clashes
			for (auto qtr_crs : *test_qtr_crses)
			{
				CourseSchedule qtr_crs_sch = qtr_crs->get_current_schedule();
				CourseSchedule crs_sch = crs->get_current_schedule();
				for (auto sch1 : qtr_crs_sch.schedules)
				{
					for (auto sch2 : crs_sch.schedules)
					{
						if (sch1.day == sch2.day)
						{
							if (sch1.time.second > sch2.time.first &&
								sch1.time.first <= sch2.time.second)
							{
								//clash
								add_crs = false;
								crs_for_deference.insert(tuple<CourseNode*,QuarterNode, QuarterNode>(crs,qtr,get_crs_next_feasible_qtr(crs,qtr)));
								break;
							}
						}
					}
					if (!add_crs)
					{
						break;
					}
				}
			}
			
			for (auto deferee = crs_for_deference.begin(); deferee != crs_for_deference.end(); deferee++)
			{
				//clean up deferred courses from current quarter chain
				vector<CourseNode*>& qtr_crses = qtr_chain[get<1>(*deferee)];
				auto remove = find(qtr_crses.begin(), qtr_crses.end(), get<0>(*deferee));
				if (remove != qtr_crses.end())
				{ //remove if present
					qtr_crses.erase(remove);
				}
			}

		}

	}

	if (add_crs)
	{

		//add crs to current quarter
		//logger << "Course Added: " << crs->course_code << " qtr: " << qtr.quarter << "yr:" << qtr.year << endl;
		test_qtr_crses->push_back(crs);
		crs->isQueued = true;
	}
	for (auto deferee = crs_for_deference.begin(); deferee != crs_for_deference.end(); deferee++)
	{
		//logger <<"deferred course about to be placed: " << get<0>(*deferee)->course_code << "qtr: " << get<2>(*deferee).quarter << " yr: " << get<2>(*deferee).year << endl;
		//cycle to next placeable quarter
		place_crs_in_chain(qtr_chain, get<0>(*deferee), get<2>(*deferee),max_credit_pref);
		if (!call_flag)
		{
			break;
		}
	}

}

map<rank_index, DegreePlan> SerialPlanner::phase2(map<AugNode*, CourseMatrix>& paths_map_input, QuarterNode start_qtr, TIME_OF_DAY tod_pref, int max_credit_pref, float max_budget_pref)
{
	auto target_crs = paths_map_input.begin();
	for (auto trgt = ++paths_map_input.begin(); trgt != paths_map_input.end(); trgt++)
	{
		if (trgt->second.size() > target_crs->second.size())
		{
			target_crs = trgt;
		}
	}
	int no_of_plans = min(20, static_cast<int>(target_crs->second.size()));
	//target_crs->second.size(); //get the maximum number of paths to generate the most plans possible without repitition

	//logger.open("logs.txt", fstream::in | fstream::out | fstream::trunc);

	
	for (int count = 0; count < no_of_plans; count++)
	{
		reset_queued_flags();  //reset isQueued flags

		DegreePlan qtr_chain = emit_new_plan(start_qtr); //fresh output copy


		for (auto crses = paths_map_input.begin(); crses != paths_map_input.end(); crses++)
		{
			if (crses->second.size() == 0)
			{
				//target courses without pre-requisites
				merge_paths_into_qtr_chain(qtr_chain, { crses->first }, start_qtr.year,max_credit_pref);
			}
			else
			{
				merge_paths_into_qtr_chain(qtr_chain, crses->second[rand() % crses->second.size()], start_qtr.year,max_credit_pref);
			}
		}

		
		
		rank_index rnk;
	
		if (calls <= 15000)
		{
			//emit new plan
			rnk.index = glb_plan_id++;
			final_output.insert(pair<rank_index, DegreePlan>(rnk, qtr_chain));
			cout << "Plan " << glb_plan_id << " Generated" << endl;
		}
		else {
			count--;
		}

		calls = 0;
		call_flag = true;

		
	}
	//resolve all class periods overlap
	//resolve_clashes(output);
	rank_plans(tod_pref, max_credit_pref, max_budget_pref);

	return final_output;
}


void SerialPlanner::rank_plans(TIME_OF_DAY tod_pref, int max_credit_pref, float max_budget_pref)
{
	vector<pair<float, DegreePlan>> rank_table;
	//insertion sort
	for (auto plan = final_output.begin(); plan != final_output.end(); plan++)
	{
		assessment result = PlanPrunner::assess_plan(plan->second, tod_pref, max_credit_pref, max_budget_pref);


		size_t rank_size = rank_table.size();
		int i = rank_size - 1;
		while (i >= 0 && result.aggregate > rank_table[i].first)
		{
			i--;
		}
		int index = i + 1;
		if (index == rank_size)
		{
			rank_table.push_back(pair<float, DegreePlan>(result.aggregate, plan->second));
		}
		else {
			rank_table.insert(rank_table.begin() + index, pair<float, DegreePlan>(result.aggregate, plan->second));
		}
	}

	//clear and repopulate final output
	final_output.clear();
	glb_plan_id = 0;
	for (auto plan = rank_table.begin(); plan != rank_table.end(); plan++)
	{
		rank_index rnk;
		rnk.index = glb_plan_id++;
		rnk.score = plan->first;
		final_output.insert(pair<rank_index, DegreePlan>(rnk, plan->second));
	}
	//TODO: Remove //logger
	//logger.close();
}
