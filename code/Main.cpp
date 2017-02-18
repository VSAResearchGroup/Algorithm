#include <iostream>

#include "CoursePlanner.h"
#include "JsonParser.h"
#include <fstream>
#include <sstream>

using namespace std;





void post_order_removal(AugNode* node)
{
	if (node == nullptr)
		return;
	for (auto nd = node->course->postreq_nodes.begin(); nd != node->course->postreq_nodes.end(); nd++)
	{
		post_order_removal(nd->second);
	}
	node->course->postreq_nodes.clear();
	delete node;
	node = nullptr;
}

void clean_up(vector<AugNode*>& startNodes)
{
	for (auto node = startNodes.begin(); node != startNodes.end(); node++)
	{
		post_order_removal(*node);
	}
}

void print_paths(map<AugNode*, CourseMatrix>& paths)
{
	for (auto endCrs = paths.begin(); endCrs != paths.end(); endCrs++)
	{
		cout << endCrs->first->course->course_code << endl<< "--------------" << endl;
		for (auto crsPath = endCrs->second.begin(); crsPath != endCrs->second.end(); crsPath++)
		{
			for (auto crs = (*crsPath).rbegin(); crs != (*crsPath).rend(); crs++)
			{
				cout << (*crs)->course->course_code << "-----";
			}
			cout << endl;
		}
		cout << endl << endl << endl;
	}
}

void print_plans(vector<DegreePlan>& plans)
{
	for (auto plan = plans.begin(); plan != plans.end(); plan++)
	{
		for (auto qtr = plan->begin(); qtr != plan->end(); qtr++)
		{
			switch (qtr->first.quarter)
			{
			case FALL:
				cout << "FALL ";
				break;
			case WINTER:
				cout << "WINTER ";
				break;
			case SPRING:
				cout << "SPRING ";
				break;
			case SUMMER:
				cout << "SUMMER ";
				break;
			}
			cout << qtr->first.year << "(";
			for (auto crs = qtr->second.begin(); crs != qtr->second.end(); crs++)
			{
				cout << (*crs)->course->course_code << ",";
			}
			cout <<") ";
		}
		cout << endl << endl;
	}
}

int main(int argc, char** argv)
{
	fstream fs;
	fs.open("input.json", fstream::in | fstream::out);
	stringstream ss;
	ss << fs.rdbuf();
	string input = ss.str();
	fs.close();
	Parser* parser = new JsonParser();
	auto req_graph = parser->parse_input(input.c_str());

	cout << "Graph built" << endl;
	CoursePlanner planner(new SerialPlanner());
	auto paths = planner.planPhase1(req_graph.first,req_graph.second);
	cout << "Phase 1 Completed" << endl;
	print_paths(paths);
	QuarterNode start_qtr;
	start_qtr.quarter = FALL;
	start_qtr.year = 2016;
	auto plans = planner.planPhase2(paths, start_qtr);
	print_plans(plans);
	string json_result = parser->generate_output_str(plans);
	delete parser;
	cout << "JSON Output: " << json_result << endl;
	fs.open("output.json", fstream::in | fstream::out | fstream::trunc);
	fs << json_result;
	fs.close();
	cout << "Phase 2 Completed" << endl;
	//clean_up(req_graph.first);
	//cout << "Cleaned up" << endl;
	system("PAUSE");
	return 0;
}