#include <string>

#include "JsonParser.h"



const map<AugNode*, CourseMatrix> JsonParser::parse_input(const char* input, map<int, CourseNode*>* _crs_details)
{

	json j_input = json::parse(input);
	for (auto crs_itr = j_input[1].begin(); crs_itr != j_input[1].end(); crs_itr++)
	{ //extract all course details and store in global map _crs_detauils
		int crs_id = atoi(crs_itr.key().c_str());

		vector<QUARTER> quarters;
		vector<schedule> schedules;
		for (json& j_sch : crs_itr.value())
		{
			//avoid duplicate quarters
			QUARTER qtr = QUARTER(j_sch[3].get<uint32_t>());
			if (find(quarters.begin(), quarters.end(), qtr) == quarters.end())
			{
				quarters.push_back(qtr);
			}

			//schedule time markers
			int fro = j_sch[0].get<uint32_t>();
			int to = j_sch[1].get<uint32_t>();
			DAY_OF_WEEK day = DAY_OF_WEEK(j_sch[2].get<uint32_t>());

			schedule sch;
			sch.day = day;
			sch.time = pair<int, int>(fro, to);
			schedules.push_back(sch);

		}

		CourseNode* course = new CourseNode(crs_id, quarters, schedules); //memory will be reclaimed by caller
		_crs_details->insert(pair<int, CourseNode*>(crs_id, course));
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
			vector<int> temp; //used for building requisite matrix
			for (auto& crses : crs_chain)
			{
				AugNode* pathNode = new AugNode();

				for (int i = 0; i < crses.size(); i++)
				{
					pathNode->course_ids.push_back(crses[i].get<uint32_t>());
				}
				for (int i = 0; i < temp.size(); i++)
				{

					vector<int>* post_reqs = &(*_crs_details)[temp[i]]->post_reqs;
					post_reqs->insert(post_reqs->end(), pathNode->course_ids.begin(), pathNode->course_ids.end());

				}

				//add newly visited courses to the pre-req tree
				for (int c : pathNode->course_ids)
				{
					temp.push_back(c);
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

int JsonParser::remap_qtr_for_output(QUARTER input)
{
	int q = (static_cast<int>(input) + 1) % 4;
	if (q == 0)
		return 4;
	return q;
}

string const JsonParser::generate_output_str(map<int, DegreePlan>&  output)
{
	json j_out;

	for (auto& plan : output)
	{
		json j_plan;
		j_plan["PlanId"] = plan.first;
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