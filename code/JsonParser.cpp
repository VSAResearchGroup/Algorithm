#include <string>

#include "JsonParser.h"

JsonParser::JsonParser()
{
	
}

AugNode* JsonParser::parse_single_crs(json& j_crs)
{

	vector<QUARTER> quarters;
	vector<schedule> schedules;
	for (json& j_sch : j_crs["CourseSchedule"])
	{
		//avoid duplicate quarters
		QUARTER qtr = QUARTER(j_sch[3].get<uint64_t>());
		if (find(quarters.begin(), quarters.end(), qtr) == quarters.end())
		{
			quarters.push_back(qtr);
		}

		//schedule time markers
		int fro = static_cast<int>(j_sch[0].get<uint64_t>());
		int to = static_cast<int>(j_sch[1].get<uint64_t>());
		DAY_OF_WEEK day = DAY_OF_WEEK(j_sch[2].get<uint64_t>());

		schedule sch;
		sch.day = day;
		sch.time =pair<int, int>(fro, to);
		schedules.push_back(sch);

	}
	int crs_id = static_cast<int>(j_crs["CourseId"].get<uint64_t>());
	CourseNode* crs = new CourseNode(crs_id, quarters, schedules);
	AugNode* crsNode = new AugNode(crs);
	return crsNode;
}

const CrsPairs JsonParser::parse_input(const char* input)
{
	map<int, AugNode*> temp; //used for DP in construction of the pre-req network
	vector<AugNode*> start_crses;
	vector<AugNode*> target_crses;
	json j_input = json::parse(input);
	for (json& j_crs : j_input)
	{
		parse_all_recursive(j_crs, j_input, temp);
	}
	//find start and target courses
	for (auto& crs : temp)
	{
		
		if (crs.second->course->prereq_nodes.empty())
		{
			//no pre-reqs (start course)
			start_crses.push_back(crs.second);
		}
		if (crs.second->course->postreq_nodes.empty())
		{
			//no post-reqs (target course)
			target_crses.push_back(crs.second);
		}
	}
	return CrsPairs(start_crses, target_crses);
}

AugNode* JsonParser::parse_all_recursive(json& j_crs, json& all_crses, map<int, AugNode*>& temp)
{
	cout << j_crs << endl<<endl;
	int crs_id = static_cast<int>(j_crs["CourseId"].get<uint64_t>());
	if (temp.find(crs_id) == temp.end())
	{ //only parse recursively if it has not been parsed
		AugNode* crsNode = parse_single_crs(j_crs);
		temp.insert(pair<int, AugNode*>(crs_id, crsNode)); //add to the list of parsed courses
		
		for (json& pre_req : j_crs["Prerequisites"])
		{
			int pre_req_id = static_cast<int>(pre_req.get<uint64_t>());
			for (json& j_pre_req : all_crses)
			{
				int id = static_cast<int>(j_pre_req["CourseId"].get<uint64_t>());
				if (id == pre_req_id)
				{
					//found pre-requisite, then parse and chain it (false indicates not a co-requisite)
					auto parsed_pre_req = pair<bool, AugNode*>(false, parse_all_recursive(j_pre_req, all_crses, temp));
					crsNode->course->prereq_nodes.push_back(parsed_pre_req);
					parsed_pre_req.second->course->postreq_nodes.push_back(pair<bool, AugNode*>(false,crsNode));
					break;
				}
			}
		}
		for (json& co_req : j_crs["Corequisites"])
		{
			int co_req_id = static_cast<int>(co_req.get<uint64_t>());
			for (json& j_co_req : all_crses)
			{
				int id = static_cast<int>(j_co_req["CourseId"].get<uint64_t>());
				if (id == co_req_id)
				{
					//found co-requisite, then parse and chain it (true indicates a co-requisite)
					auto parsed_co_req = pair<bool, AugNode*>(true, parse_all_recursive(j_co_req, all_crses, temp));
					crsNode->course->prereq_nodes.push_back(parsed_co_req);
					parsed_co_req.second->course->postreq_nodes.push_back(pair<bool, AugNode*>(true, crsNode));
					break;
				}
			}
		}

		
		return crsNode;
	}
	//return course directly from cache if it has been previously parsed
	return temp[crs_id];
}

string const JsonParser::generate_output_str(vector<DegreePlan>&  output)
{
	json j_out;
	
	for (auto& plan : output)
	{
		json j_plan;
		for (auto& qtr : plan)
		{
			json j_qtr;
			j_qtr["Quarter"] = static_cast<int>(qtr.first.quarter);
			j_qtr["Year"] = qtr.first.year;
			j_qtr["Courses"] = {};
			for (auto& crs : qtr.second)
			{
				json j_crs;
				j_crs["CourseId"] = crs->course->course_code;
				j_crs["Schedules"] = {};
				for (auto& sch : crs->course->schedules)
				{
					j_crs["Schedules"].push_back({sch.time.first,sch.time.second,static_cast<int>(sch.day)});
				}
				j_qtr["Courses"].push_back(j_crs);
			}
			j_plan["Quarters"].push_back(j_qtr);
		}
		j_out.push_back(j_plan);
	}
	return j_out.dump(4);
}