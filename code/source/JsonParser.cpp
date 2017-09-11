#include <string>

#include "../headers/JsonParser.h"



const map<AugNode*, CourseMatrix> JsonParser::parse_input(const char* input, map<int, CourseNode*>* _crs_details)
{
	
	json j_input = json::parse(input);
	for (auto crs_itr = j_input[1].begin(); crs_itr != j_input[1].end(); crs_itr++)
	{ //extract all course details and store in global map _crs_detauils
		int crs_id = atoi(crs_itr.key().c_str());

		vector<CourseSchedule> schedules;
		for (json& j_sch : crs_itr.value())
		{
			//avoid duplicate quarters
			QUARTER qtr = map_qtr_for_input( QUARTER(j_sch[3].get<uint32_t>()));
			int existing_qtr_index = -1;
			for (int i = 0; i < schedules.size(); i++)
			{
				if (schedules[i].qtr == qtr)
				{
					existing_qtr_index = i;
					break;
				}
				//adding section
				schedules[i].section = j_sch[4].get<uint32_t>();
			}

			//schedule time markers
			int fro = j_sch[0].get<uint32_t>();
			int to = j_sch[1].get<uint32_t>();
			DAY_OF_WEEK day = DAY_OF_WEEK(j_sch[2].get<uint32_t>());

			schedule sch;
			sch.day = day;
			sch.time = pair<int, int>(fro, to);

			if (existing_qtr_index != -1)
			{
				//quarter had been previously added
				schedules[existing_qtr_index].schedules.push_back(sch);
			}
			else {
				CourseSchedule crs_sch;
				crs_sch.qtr = qtr;
				schedules.push_back(crs_sch);
				schedules.back().schedules.push_back(sch);
			}
			sort(schedules.begin(), schedules.end(), [](CourseSchedule const& a, CourseSchedule const& b)->bool {
				if (a.qtr == QUARTER::FALL)
					return true;
				if (b.qtr == QUARTER::FALL)
					return false;
				return a.qtr < b.qtr;
			});

		}



		CourseNode* course = new CourseNode(crs_id, schedules); //memory will be reclaimed by caller
		_crs_details->insert(pair<int, CourseNode*>(crs_id, course));

		//add course to requisite matrix
		json pre_reqs = j_input[3][to_string(crs_id)];
		vector<int> pre_crs_ids;
		for (auto pre_crs = pre_reqs.begin(); pre_crs != pre_reqs.end(); pre_crs++)
		{
			int pre_crs_id = pre_crs->get<uint32_t>();
			pre_crs_ids.push_back(pre_crs_id);
		}
		auto* crs_pre_req = &_crs_details->at(crs_id)->post_reqs;
		crs_pre_req->insert(crs_pre_req->end(), pre_crs_ids.begin(), pre_crs_ids.end());
	}

	

	//find paths
	map<AugNode*, CourseMatrix> paths;
	for (auto req_itr = j_input[0].begin(); req_itr != j_input[0].end(); req_itr++)
	{
		AugNode* target = new AugNode();
		target->course_ids.push_back(atoi(req_itr.key().c_str()));
		json pre_reqs = req_itr.value();
		CourseMatrix multiPaths;
		for (auto& prq : pre_reqs)
		{
			json crs_chain = prq["TargetList"];
			vector<AugNode*> path;
			
			for (auto& crses : crs_chain)
			{
				AugNode* pathNode = new AugNode();
		
				for (int i=0; i < crses.size(); i++)
				{
					pathNode->course_ids.push_back(crses[i].get<uint32_t>());
				}
				

				
				//building a single path
				path.push_back(pathNode);
			}
			//adding a built path to the list of target course's path
			multiPaths.push_back(path);
			
		}
		//adding all paths for a target course to phase 2's prospective input
		paths.insert(pair<AugNode*, CourseMatrix>(target, multiPaths));

	}
	return paths;
}

QUARTER JsonParser::map_qtr_for_input(QUARTER input)
{
	int q = static_cast<int>(input) - 1;
	if (q < 1)
	{
		q = 4;
	}
	return QUARTER(q);
}


int JsonParser::remap_qtr_for_output(QUARTER input)
{
	int q = (static_cast<int>(input) + 1 )% 4;
	if (q == 0)
		return 4;
	return q;
}

string const JsonParser::generate_output_str(map<rank_index,DegreePlan>&  output)
{
	json j_out;
	
	for (auto& plan : output)
	{

		json j_plan;
		j_plan["PlanId"] = plan.first.index;
		j_plan["PlanScore"] = plan.first.score;
		for (auto& qtr : plan.second)
		{
			json j_qtr;
			j_qtr["Quarter"] = remap_qtr_for_output(qtr.first.quarter); 
			j_qtr["Year"] = qtr.first.year;
			j_qtr["Courses"] = {};
			for (auto& crs : qtr.second)
			{
				j_qtr["Courses"].push_back(crs->course_code);
			}
			j_plan["Quarters"].push_back(j_qtr);
		}
		j_out.push_back(j_plan);
	}
	return j_out.dump(4);
}