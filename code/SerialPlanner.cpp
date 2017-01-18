#include "SerialPlanner.h"
#include <queue>


void SerialPlanner::augment_graph(vector<AugNode*>& endNodes)
{
	std::queue<AugNode*> q;
	//augment graph
	for (auto course = endNodes.begin(); course != endNodes.end(); course++)
	{
		AugNode* crs = *course;
		NDPair nd;
		nd.courseCode = crs->course->course_code;
		nd.distance = 0;
		crs->distances.push_back(nd);
		crs->isQueued1 = true;
		q.push(crs);
	}

	while (!q.empty())
	{
		AugNode*  currNode = q.front();
		q.pop();
		for (auto node = currNode->course->postreq_nodes.begin(); node != currNode->course->postreq_nodes.end(); node++)
		{
			AugNode* nd = node->second;

			for (auto distCell = nd->distances.begin(); distCell != nd->distances.end(); distCell++)
			{
				NDPair d = *distCell;
				d.distance++;
				currNode->distances.push_back(d);
			}


		}

		for (auto node = currNode->course->prereq_nodes.begin(); node != currNode->course->prereq_nodes.end(); node++)
		{
			AugNode* nd = node->second;
			if (!nd->isQueued1)
			{
				q.push(nd);
				nd->isQueued1 = true;
			}
		}

	}


}

void SerialPlanner::probe_graph(vector<AugNode*>& startNodes, vector<AugNode*>& endNodes, map<AugNode*, CourseMatrix>& output)
{
	queue<AugNode*> q;

	for (auto endNode = endNodes.begin(); endNode != endNodes.end(); endNode++)
	{
		AugNode* en = *endNode;
		//send probe for each end node's pre-requisite

		for (auto node = en->course->prereq_nodes.begin(); node != en->course->prereq_nodes.end(); node++)
		{
			Probe probe;
			probe.originCrs = en->course->course_code;
			probe.path.push_back(en);
			AugNode* pre_req = node->second;
			pre_req->currProbes.push_back(probe);
			if (!pre_req->isQueued2)
			{
				q.push(pre_req);
				pre_req->isQueued2 = true;
			}
		}

	}

	while (!q.empty())
	{
		AugNode* node = q.front();

		q.pop();
		if (node->course->prereq_nodes.empty())
		{
			//start node AKA end of path for each probe going through this node
			for (auto probe = node->currProbes.begin(); probe != node->currProbes.end(); probe++)
			{
				probe->path.push_back(node);
				if (output.find(probe->path[0]) == output.end())
				{
					CourseMatrix paths;
					paths.push_back(probe->path);
					output.insert(pair<AugNode*, CourseMatrix>(probe->path[0], paths));
				}
				else {
					//insertion sort
					CourseMatrix& paths = output[probe->path[0]];
					bool inserted = false;
					for (auto path = paths.begin(); path != paths.end(); path++)
					{
						vector<AugNode*> pt = *path;
						if (probe->path.size() <= pt.size())
						{
							inserted = true;
							paths.insert(path, probe->path);
							break;
						}
					}
					if (!inserted)
						paths.push_back(probe->path);
				}

			}


		}
		else
		{
			//intermediate node
			for (auto probe = node->currProbes.begin(); probe != node->currProbes.end(); probe++)
			{
				probe->path.push_back(node);
				Probe prb = *probe;
				for (auto pre_req = node->course->prereq_nodes.begin(); pre_req != node->course->prereq_nodes.end(); pre_req++)
				{
					AugNode* prq = pre_req->second;
					for (auto ndp = prq->distances.begin(); ndp != prq->distances.end(); ndp++)
					{
						NDPair& dp = *ndp;
						if (dp.courseCode == prb.originCrs && dp.distance != -1)
						{
							//cut through first cell for probe's origin
							dp.distance = -1;
							prq->currProbes.push_back(prb);
							//queue pre-req node if not queued
							if (!prq->isQueued2)
							{
								q.push(prq);
								prq->isQueued2 = true;
							}
							break;
						}
					}

				}
			}
		}
		node->currProbes.clear();
	}
}



map<AugNode*, CourseMatrix> SerialPlanner::phase1(vector<AugNode*>& startNodes, vector<AugNode*>& endNodes)
{
	map<AugNode*, CourseMatrix> pathsForAllEnds;
	augment_graph(endNodes);
	probe_graph(startNodes, endNodes, pathsForAllEnds);
	build_req_matrix(startNodes); //prep for phase 2
	return pathsForAllEnds;
}

void SerialPlanner::build_req_matrix(vector<AugNode*>& startNodes)
{
	for (auto crs = startNodes.begin(); crs != startNodes.end(); crs++)
	{
		scan_and_add(*crs, vector<AugNode*>());
	}
}

void SerialPlanner::scan_and_add(AugNode* crs, vector<AugNode*>& parents)
{
	if (_req_matrix.find(crs) == _req_matrix.end())
	{
		crs->isQueued1 = false;
		crs->isQueued2 = false;
		_req_matrix.insert(pair<AugNode*, vector<pair<bool,AugNode*>>>(crs, vector<pair<bool,AugNode*>>()));

		for (auto scan_crs = crs->course->postreq_nodes.begin(); scan_crs != crs->course->postreq_nodes.end(); scan_crs++)
		{
			_req_matrix[crs].push_back(*scan_crs);
			for (auto prt = parents.begin(); prt != parents.end(); prt++)
			{
				_req_matrix[*prt].push_back(*scan_crs);
			}

		}
		if (!crs->course->postreq_nodes.empty())
		{
			parents.push_back(crs);
		}
		for (auto scan_crs = crs->course->postreq_nodes.begin(); scan_crs != crs->course->postreq_nodes.end(); scan_crs++)
		{
			scan_and_add(scan_crs->second, parents);
	
		}
	}
}

AugNode* SerialPlanner::get_target_with_shortest_path(map<AugNode*, CourseMatrix>& paths_map)
{
	AugNode* target_crs = paths_map.begin()->first;
	for (auto target = paths_map.begin(); target != paths_map.end(); target++)
	{
		if (target->second[0].size() < paths_map[target_crs][0].size())
		{
			target_crs = target->first;
		}
	}
	return target_crs;
}

DegreePlan SerialPlanner::chain_end_courses(map<AugNode*, CourseMatrix>& paths_map, QuarterNode start_qtr)
{
	DegreePlan qtr_map;
	unsigned short start_yr = start_qtr.year;
	for (int i = 0; i < 4; i++)
	{
		qtr_map.insert(pair<QuarterNode, vector<AugNode*>>(start_qtr, vector<AugNode*>()));
		start_qtr = start_qtr.next_qtr();
	}
	for (auto target = paths_map.begin(); target != paths_map.end(); target++)
	{
		QuarterNode qtr;
		qtr.quarter = target->first->course->quarters[0];
		qtr.year = start_yr;
		if (qtr_map.find(qtr) == qtr_map.end())
			qtr.year++;
		qtr_map[qtr].push_back(target->first);
	}
	return qtr_map;
}


void SerialPlanner::merge_paths_into_qtr_chain(DegreePlan& qtr_chain, vector<AugNode*> path, ushort year)
{

	for (auto crs = path.begin(); crs != path.end(); crs++)
	{
		if (!(*crs)->isQueued1)
		{
			QuarterNode qtr;
			qtr.quarter = (*crs)->course->quarters[0];
			qtr.year = year;
			place_crs_in_chain(qtr_chain, *crs, qtr);
		}
	}
}

void SerialPlanner::place_crs_in_chain(DegreePlan& qtr_chain, AugNode* crs, QuarterNode qtr)
{

	vector<AugNode*>* test_qtr_crses = nullptr;
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
			qtr_chain.insert(pair<QuarterNode, vector<AugNode*>>(to_add, vector<AugNode*>()));
		}
	}

	test_qtr_crses = &qtr_chain[qtr];

	bool add_crs = true;
	vector<pair<AugNode*, QuarterNode>> crs_for_deference;
	auto test_qtr_itr = qtr_chain.find(qtr);

	for (auto qtr_itr = qtr_chain.begin(); qtr_itr != test_qtr_itr; qtr_itr++)
	{
		for (auto c = qtr_itr->second.begin(); c != qtr_itr->second.end(); c++)
		{
			AugNode* c_val = *c;
			auto req_ptr = find_if(_req_matrix[crs].begin(), _req_matrix[crs].end(), [&c_val](const pair<bool, AugNode*> entry) {
				return entry.second->course->course_code == c_val->course->course_code;
			});
			if (req_ptr != _req_matrix[crs].end() && !req_ptr->first)
			{//crs is a pre-requisite and not a co-requisite to c 
				crs_for_deference.push_back(pair<AugNode*, QuarterNode>(*c, qtr_itr->first)); //add c to a list of courses to be deferred
			}
			
		}
	}
	if (!test_qtr_crses->empty())
	{//quarter under consideration have courses already assigned to it

		for (auto c = test_qtr_crses->begin(); c != test_qtr_crses->end(); c++)
		{
			auto req_ptr = find_if(_req_matrix[*c].begin(), _req_matrix[*c].end(), [&crs](const pair<bool, AugNode*>& entry) {
				return entry.second->course->course_code == crs->course->course_code;
			});
			AugNode* c_val = *c;
			auto rev_req_ptr = find_if(_req_matrix[crs].begin(), _req_matrix[crs].end(), [&c_val](const pair<bool, AugNode*>& entry){
				return entry.second->course->course_code == c_val->course->course_code;
			});
			if (req_ptr != _req_matrix[*c].end() && !req_ptr->first)
			{ //c is a pre-requisite and not a co-requisite to crs
				add_crs = false; //do not add crs to this quarter
				break;
			}
			else if (rev_req_ptr != _req_matrix[crs].end() && !rev_req_ptr->first)
			{//crs is a pre-requisite and not a co-requisite to c 
				crs_for_deference.push_back(pair<AugNode*, QuarterNode>(*c, qtr)); //add c to a list of courses to be deferred
			}
		}

	}

	if (add_crs)
	{
		//add crs to current quarter
		test_qtr_crses->push_back(crs);
		crs->isQueued1 = true;

	}
	else
	{
		//if crs is not going to be added, include it in the deference list
		crs_for_deference.push_back(pair<AugNode*, QuarterNode>(crs, qtr));
	}
	for (auto deferee = crs_for_deference.begin(); deferee != crs_for_deference.end(); deferee++)
	{
		//remove from quarter chain
		vector<AugNode*>& qtr_crses = qtr_chain[deferee->second];
		qtr_crses.erase(find(qtr_crses.begin(), qtr_crses.end(), deferee->first));
		//cycle to next placeable quarter
		place_crs_in_chain(qtr_chain, deferee->first, get_crs_next_feasible_qtr(deferee->first, deferee->second));
	}
}

void SerialPlanner::resolve_clashes(vector<DegreePlan>& plans)
{
	for (auto plan = plans.begin(); plan != plans.end(); plan++)
	{
		reorder_for_clash(plans, *plan,0,true);
	}
}

void SerialPlanner::reorder_for_clash(vector<DegreePlan>& plans, DegreePlan& single_plan, int start_index, bool is_included)
{
	bool remove_plan = false;
	DegreePlan::iterator qtr_start = single_plan.begin();
	int offset = 0;
	while (offset++ < start_index)
	{
		qtr_start++;
	}
	for (auto qtr = qtr_start; qtr != single_plan.end(); qtr++)
	{
		int size = static_cast<int>(qtr->second.size());
		for (int i = 0; i < size - 1; i++)
		{
			bool found_overlap = false;
			for (int j = i + 1; j < size; j++)
			{
				//check if a course overlaps with another
				AugNode* crs1 = qtr->second[i];
				AugNode* crs2 = qtr->second[j];
				if (crs1->course->days == crs2->course->days)
				{
					if (crs1->course->schedule.second > crs2->course->schedule.first &&
						crs1->course->schedule.first <= crs2->course->schedule.second)
					{
						found_overlap = true;
						remove_plan = is_included;
						//option 1: defer crs 1 with new plan emerging		
						DegreePlan new_plan1 = single_plan;		
						new_plan1[qtr->first].erase(new_plan1[qtr->first].begin() + i);
						merge_paths_into_qtr_chain(new_plan1, { crs1 }, qtr->first.year + 1);
						reorder_for_clash(plans, new_plan1, start_index, false);
						//option 2: defer crs 2 with another new plan emerging
						DegreePlan new_plan2 = single_plan;
						new_plan2[qtr->first].erase(new_plan2[qtr->first].begin() + j);
						merge_paths_into_qtr_chain(new_plan2, { crs2 }, qtr->first.year + 1);
						reorder_for_clash(plans, new_plan2, start_index, false);
						break;
					}
				}
			}
			if (found_overlap)
			{
				break;
			}
			
		}
		start_index ++;
	}
	if (remove_plan)
	{
		plans.erase(find(plans.begin(),plans.end(),single_plan));
	}
	else if (!is_included)
	{
		plans.push_back(single_plan);
	}
}

vector<DegreePlan> SerialPlanner::phase2(map<AugNode*, CourseMatrix>& paths_map_input, QuarterNode start_qtr)
{
	AugNode* target_crs = get_target_with_shortest_path(paths_map_input);
	vector<DegreePlan> output;
	for (auto path = paths_map_input[target_crs].begin(); path != paths_map_input[target_crs].end(); path++)
	{
		map<AugNode*, CourseMatrix> paths_map = paths_map_input; //create a fresh copy to reset isQueued flags
		vector<AugNode*> merged_crses = { target_crs }; //list of target courses merged into this generated path
		DegreePlan qtr_chain = chain_end_courses(paths_map, start_qtr); //fresh output copy
		int query_crs_index = 1;
		for (auto query_crs = path->begin() + 1; query_crs != path->end(); query_crs++) //starting from node n-1
		{
			if (merged_crses.size() < paths_map_input.size())
			{
				//only run if all target paths have not been fully merged
				for (auto target = paths_map.begin(); target != paths_map.end(); target++)
				{
					if (find(merged_crses.begin(), merged_crses.end(), target->first) == merged_crses.end())
					{
						//only do this for courses that have not been fully merged
						int curr_index_offset = 0;
						AugNode* matching_crs = nullptr;
						int matching_crs_index = -1;
						vector<AugNode*> test_path;

						do {
							int i = 0;
							do {
								test_path = target->second[i++];
								if ((query_crs_index + curr_index_offset) < test_path.size())
								{
									matching_crs_index = query_crs_index + curr_index_offset;
									matching_crs = test_path[matching_crs_index];
								}
							} while (matching_crs != *query_crs &&  i < target->second.size());
							curr_index_offset++;
							//while no match found and the longest path (path at the bottom of the list of paths) have not been exhaustively transversed
						} while (matching_crs != *query_crs && ((query_crs_index + curr_index_offset) < target->second.back().size()));

						vector<AugNode*> connector = { *query_crs };
						if (matching_crs != *query_crs)
						{
							//no connection point found
							test_path = target->second[0]; //set path to merge as shortest path 
							connector[0] = test_path[0];
						}
						else
						{
							//found a connection point
							merged_crses.push_back(target->first);
						}
						for (int i = matching_crs_index + 1; i < test_path.size() - 1; i++)
						{
							//add courses between connection point and target
							connector.push_back(test_path[i]);
						}
						merge_paths_into_qtr_chain(qtr_chain, connector, start_qtr.year);
					}

				}
				query_crs_index--;
			}
		}
	
		merge_paths_into_qtr_chain(qtr_chain, *path, start_qtr.year);
		//emit new plan
		output.push_back(qtr_chain);
	}
	//resolve all class periods overlap
	resolve_clashes(output);
	return output;
}

