#include <iostream>

#include "CoursePlanner.h"

using namespace std;

typedef pair<vector<AugNode*>, vector<AugNode*>> CrsPairs;
vector<int> time_slots;
void bootstrap()
{
	
	for(int i=1;i<=56;i++)
	{ 
		time_slots.push_back(i);
	}
}

//build a sample (hardcoded) pre-preq network from the Math degree spec 
const CrsPairs build_graph()
{
	//ASSUME 101 - PREFIX FOR MATH, 100 - FOR BUSINESS, 102  - FOR ART
	    vector<struct AugNode*> startNodes;
		vector<DAY_OF_WEEK> temp_days = { DAY_OF_WEEK::MON,DAY_OF_WEEK::WED };
		vector<QUARTER> quarters = { FALL };
		struct AugNode* cosmo = new AugNode(new CourseNode(102060, quarters, pair<int, int>(39, 47), temp_days));

		startNodes.push_back(cosmo);
		temp_days = { DAY_OF_WEEK::TUE,DAY_OF_WEEK::THUR };
		struct AugNode* math=new AugNode(new CourseNode(101070, quarters, pair<int, int>(30, 38), temp_days));
		startNodes.push_back(math);
		temp_days = { DAY_OF_WEEK::MON,DAY_OF_WEEK::WED };
		quarters = { SPRING };
		struct AugNode* busns =new AugNode (new CourseNode(100130,quarters, pair<int, int>(12, 20), temp_days));
		math->course->postreq_nodes.push_back(busns);

		busns->course->prereq_nodes.push_back(math);
		temp_days = { DAY_OF_WEEK::TUE,DAY_OF_WEEK::THUR };
		struct AugNode* math1 = new AugNode(new CourseNode(101080, quarters, pair<int, int>(4, 12), temp_days));
		math->course->postreq_nodes.push_back(math1);
		math1->course->prereq_nodes.push_back(math);

		temp_days = { DAY_OF_WEEK::TUE,DAY_OF_WEEK::THUR };
		quarters = {FALL};
		struct AugNode* math2 = new AugNode(new CourseNode(101085,quarters,pair<int,int>(4,12), temp_days));
		math1->course->postreq_nodes.push_back(math2);
		math2->course->prereq_nodes.push_back(math1);
		temp_days = { DAY_OF_WEEK::MON,DAY_OF_WEEK::WED };
		quarters = {WINTER};
		struct AugNode* math3 = new AugNode(new CourseNode(101100,quarters,pair<int,int>(20, 28), temp_days));
		math1->course->postreq_nodes.push_back(math3);
		math3->course->prereq_nodes.push_back(math1);
		temp_days = { DAY_OF_WEEK::MON,DAY_OF_WEEK::WED };
		struct AugNode* math4 = new AugNode(new CourseNode(101091, quarters, pair<int, int>(28, 36), temp_days));
		math1->course->postreq_nodes.push_back(math4);
		math4->course->prereq_nodes.push_back(math1);
		temp_days = { DAY_OF_WEEK::TUE,DAY_OF_WEEK::THUR };
		struct AugNode* math5 = new AugNode(new CourseNode(101092, quarters, pair<int, int>(28, 36), temp_days));
		math4->course->postreq_nodes.push_back(math5);
		math5->course->prereq_nodes.push_back(math4);
		temp_days = { DAY_OF_WEEK::TUE,DAY_OF_WEEK::THUR };
		struct AugNode* math6 =new AugNode(new CourseNode(101098, quarters, pair<int, int>(4, 12), temp_days));
		math4->course->postreq_nodes.push_back(math6);
		math6->course->prereq_nodes.push_back(math4);
		temp_days = { DAY_OF_WEEK::MON,DAY_OF_WEEK::WED };
		quarters = { FALL };
		struct AugNode* math7 = new AugNode(new CourseNode(101141, quarters, pair<int, int>(12, 20), temp_days));
		math5->course->postreq_nodes.push_back(math7);
		math7->course->prereq_nodes.push_back(math5);
		temp_days = { DAY_OF_WEEK::MON,DAY_OF_WEEK::WED };
		struct AugNode* math8 = new AugNode(new CourseNode(101138, quarters, pair<int, int>(12, 20), temp_days));
		math5->course->postreq_nodes.push_back(math8);
		math8->course->prereq_nodes.push_back(math5);
		temp_days = { DAY_OF_WEEK::MON,DAY_OF_WEEK::WED };
		quarters = {SPRING};
		struct AugNode* math9 =new AugNode(new CourseNode(101099, quarters, pair<int, int>(32, 40), temp_days));
		startNodes.push_back(math9);
		temp_days = { DAY_OF_WEEK::TUE,DAY_OF_WEEK::THUR };
		struct AugNode* math10 = new AugNode(new CourseNode(101146, quarters, pair<int, int>(32, 40), temp_days));
		math9->course->postreq_nodes.push_back(math10);
		math10->course->prereq_nodes.push_back(math9);
		math6->course->postreq_nodes.push_back(math10);
		math10->course->prereq_nodes.push_back(math6);
		temp_days = { DAY_OF_WEEK::TUE,DAY_OF_WEEK::THUR };
		quarters = { WINTER };
		struct AugNode* math11 = new AugNode(new CourseNode(101107, quarters, pair<int, int>(30, 38), temp_days));
		math9->course->postreq_nodes.push_back(math11);
		math11->course->prereq_nodes.push_back(math9);
		math6->course->postreq_nodes.push_back(math11);
		math11->course->prereq_nodes.push_back(math6);
		quarters = { FALL };
		struct AugNode* math12 = new AugNode(new CourseNode(101142, quarters, pair<int, int>(12, 20), temp_days));
		math7->course->postreq_nodes.push_back(math12);
		math12->course->prereq_nodes.push_back(math7);
		struct AugNode* math13 = new AugNode(new CourseNode(101148, quarters, pair<int, int>(12, 20), temp_days));
		math7->course->postreq_nodes.push_back(math13);
		math13->course->prereq_nodes.push_back(math7);
		math8->course->postreq_nodes.push_back(math13);
		math13->course->prereq_nodes.push_back(math8);
		temp_days = { DAY_OF_WEEK::MON,DAY_OF_WEEK::WED };
		quarters = { SPRING };
		struct AugNode* math14 = new AugNode(new CourseNode(101144, quarters, pair<int, int>(30, 38), temp_days));
		startNodes.push_back(math14);
		temp_days = { DAY_OF_WEEK::TUE,DAY_OF_WEEK::THUR };
		quarters = { WINTER };
		struct AugNode* math15 = new AugNode(new CourseNode(101151, quarters, pair<int, int>(14, 22), temp_days));
		math14->course->postreq_nodes.push_back(math15);
		math15->course->prereq_nodes.push_back(math14);
		math12->course->postreq_nodes.push_back(math15);
		math15->course->prereq_nodes.push_back(math12);
		temp_days = { DAY_OF_WEEK::TUE,DAY_OF_WEEK::THUR };
		quarters = { WINTER };
		struct AugNode* math16 = new AugNode(new CourseNode(101152, quarters, pair<int, int>(36, 44), temp_days));
		math15->course->postreq_nodes.push_back(math16);
		math16->course->prereq_nodes.push_back(math15);
		temp_days = { DAY_OF_WEEK::MON,DAY_OF_WEEK::WED };
		quarters = { WINTER };
		struct AugNode* math17 =new AugNode(new CourseNode(101163, quarters, pair<int, int>(30, 38), temp_days));
		math16->course->postreq_nodes.push_back(math17);
		math17->course->prereq_nodes.push_back(math16);
		quarters = { SPRING };
		struct AugNode* math18 = new AugNode(new CourseNode(101264, quarters, pair<int, int>(16, 24), temp_days));
		temp_days = { DAY_OF_WEEK::TUE,DAY_OF_WEEK::THUR};
		struct AugNode* math19 = new AugNode(new CourseNode(101260, quarters, pair<int, int>(20, 28), temp_days));
		temp_days = { DAY_OF_WEEK::MON,DAY_OF_WEEK::WED };
		quarters = { FALL };
		struct AugNode* math20 = new AugNode(new CourseNode(101261, quarters, pair<int, int>(0, 8), temp_days));

		math17->course->postreq_nodes.push_back(math18);
		math18->course->prereq_nodes.push_back(math17);
		math17->course->postreq_nodes.push_back(math19);
		math19->course->prereq_nodes.push_back(math17);
		math17->course->postreq_nodes.push_back(math20);
		math20->course->prereq_nodes.push_back(math17);
		vector<AugNode*> endNodes;
		endNodes.push_back(math18);
		endNodes.push_back(math19);
		endNodes.push_back(math20);
		return CrsPairs(startNodes, endNodes);
	
}


void post_order_removal(AugNode* node)
{
	if (node == nullptr)
		return;
	for (auto nd = node->course->postreq_nodes.begin(); nd != node->course->postreq_nodes.end(); nd++)
	{
		post_order_removal(*nd);
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

	auto req_graph = build_graph();
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
	//clean_up(req_graph.first);
	//cout << "Cleaned up" << endl;
	system("PAUSE");
	return 0;
}