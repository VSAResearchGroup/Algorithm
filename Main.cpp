#include <iostream>

#include "CoursePlanner.h"

using namespace std;

typedef pair<vector<AugNode*>, vector<AugNode*>> CrsPairs;

//build a sample (hardcoded) pre-preq network from the Math degree spec 
const CrsPairs build_graph()
{
	//ASSUME 100 - PREFIX FOR MATH, 101 - FOR BUSINESS, 102  - FOR ART
	    vector<struct AugNode*> startNodes;
	
		struct AugNode* cosmo=new AugNode(new CourseNode(102060));
		startNodes.push_back(cosmo);
		struct AugNode* math=new AugNode(new CourseNode(101070));
		startNodes.push_back(math);
		struct AugNode* busns =new AugNode (new CourseNode(100130));
		math->course->postreq_nodes.push_back(busns);

		busns->course->prereq_nodes.push_back(math);

		struct AugNode* math1 = new AugNode(new CourseNode(101080));
		math->course->postreq_nodes.push_back(math1);
		math1->course->prereq_nodes.push_back(math);

		struct AugNode* math2 = new AugNode(new CourseNode(101085));
		math1->course->postreq_nodes.push_back(math2);
		math2->course->prereq_nodes.push_back(math1);
		struct AugNode* math3 = new AugNode(new CourseNode(101100));
		math1->course->postreq_nodes.push_back(math3);
		math3->course->prereq_nodes.push_back(math1);
		struct AugNode* math4 = new AugNode(new CourseNode(101091));
		math1->course->postreq_nodes.push_back(math4);
		math4->course->prereq_nodes.push_back(math1);
		struct AugNode* math5 = new AugNode(new CourseNode(101092));
		math4->course->postreq_nodes.push_back(math5);
		math5->course->prereq_nodes.push_back(math4);
		struct AugNode* math6 =new AugNode(new CourseNode(101098));
		math4->course->postreq_nodes.push_back(math6);
		math6->course->prereq_nodes.push_back(math4);
		struct AugNode* math7 = new AugNode(new CourseNode(101141));
		math5->course->postreq_nodes.push_back(math7);
		math7->course->prereq_nodes.push_back(math5);
		struct AugNode* math8 = new AugNode(new CourseNode(101138));
		math5->course->postreq_nodes.push_back(math8);
		math8->course->prereq_nodes.push_back(math5);
		struct AugNode* math9 =new AugNode(new CourseNode(101099));
		startNodes.push_back(math9);

		struct AugNode* math10 = new AugNode(new CourseNode(101146));
		math9->course->postreq_nodes.push_back(math10);
		math10->course->prereq_nodes.push_back(math9);
		math6->course->postreq_nodes.push_back(math10);
		math10->course->prereq_nodes.push_back(math6);
		struct AugNode* math11 = new AugNode(new CourseNode(101107));
		math9->course->postreq_nodes.push_back(math11);
		math11->course->prereq_nodes.push_back(math9);
		math6->course->postreq_nodes.push_back(math11);
		math11->course->prereq_nodes.push_back(math6);

		struct AugNode* math12 = new AugNode(new CourseNode(101142));
		math7->course->postreq_nodes.push_back(math12);
		math12->course->prereq_nodes.push_back(math7);
		struct AugNode* math13 = new AugNode(new CourseNode(101148));
		math7->course->postreq_nodes.push_back(math13);
		math13->course->prereq_nodes.push_back(math7);
		math8->course->postreq_nodes.push_back(math13);
		math13->course->prereq_nodes.push_back(math8);

		struct AugNode* math14 = new AugNode(new CourseNode(101144));
		startNodes.push_back(math14);

		struct AugNode* math15 = new AugNode(new CourseNode(101151));
		math14->course->postreq_nodes.push_back(math15);
		math15->course->prereq_nodes.push_back(math14);
		math12->course->postreq_nodes.push_back(math15);
		math15->course->prereq_nodes.push_back(math12);

		struct AugNode* math16 = new AugNode(new CourseNode(101152));
		math15->course->postreq_nodes.push_back(math16);
		math16->course->prereq_nodes.push_back(math15);

		struct AugNode* math17 =new AugNode(new CourseNode(101163));
		math16->course->postreq_nodes.push_back(math17);
		math17->course->prereq_nodes.push_back(math16);

		struct AugNode* math18 = new AugNode(new CourseNode(101264));
		struct AugNode* math19 = new AugNode(new CourseNode(101260));
		struct AugNode* math20 = new AugNode(new CourseNode(101261));

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

int main(int argc, char** argv)
{

	auto req_graph = build_graph();
	cout << "Graph built" << endl;
	CoursePlanner planner(new SerialPlanner());
	auto paths = planner.planPhase1(req_graph.first,req_graph.second);
	cout << "Planned" << endl;
	print_paths(paths);
	//clean_up(req_graph.first);
	//cout << "Cleaned up" << endl;
	system("PAUSE");
	return 0;
}