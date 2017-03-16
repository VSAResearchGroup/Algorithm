// RecEngine.cpp : Defines the exported functions for the DLL application.
//

#include "RecEngine.h"


using namespace std;



//build a sample (hardcoded) pre-preq network from the Math degree spec 
void build_graph()
{
	//ASSUME 101 - PREFIX FOR MATH, 100 - FOR BUSINESS, 102  - FOR ART
	/*    vector<AugNode*> startNodes;
		vector<DAY_OF_WEEK> temp_days = { DAY_OF_WEEK::MON,DAY_OF_WEEK::WED };
		vector<QUARTER> quarters = { FALL };
		struct AugNode* cosmo = new AugNode(new CourseNode(102060, quarters, pair<int, int>(39, 47), temp_days));

		startNodes.push_back(cosmo);
		temp_days = { DAY_OF_WEEK::TUE,DAY_OF_WEEK::THUR };
		struct AugNode* math=new AugNode(new CourseNode(101070, quarters, pair<int, int>(30, 38), temp_days));
		startNodes.push_back(math);
		temp_days = { DAY_OF_WEEK::MON,DAY_OF_WEEK::WED };
		struct AugNode* math0 = new AugNode(new CourseNode(101079, quarters, pair<int, int>(24, 32), temp_days));
		math->course->postreq_nodes.push_back(pair<bool, AugNode*>(true, math0));
		math0->course->prereq_nodes.push_back(pair<bool, AugNode*>(true, math));
		
		temp_days = { DAY_OF_WEEK::MON,DAY_OF_WEEK::WED };
		quarters = { SPRING };
		struct AugNode* busns =new AugNode (new CourseNode(100130,quarters, pair<int, int>(12, 20), temp_days));
		math0->course->postreq_nodes.push_back(pair<bool,AugNode*>(false,busns));

		busns->course->prereq_nodes.push_back(pair<bool, AugNode*>(false, math0));
		temp_days = { DAY_OF_WEEK::TUE,DAY_OF_WEEK::THUR };
		struct AugNode* math1 = new AugNode(new CourseNode(101080, quarters, pair<int, int>(4, 12), temp_days));
		math0->course->postreq_nodes.push_back(pair<bool, AugNode*>(false, math1));
		math1->course->prereq_nodes.push_back(pair<bool, AugNode*>(false, math0));

		temp_days = { DAY_OF_WEEK::TUE,DAY_OF_WEEK::THUR };
		quarters = {FALL};
		struct AugNode* math2 = new AugNode(new CourseNode(101085,quarters,pair<int,int>(4,12), temp_days));
		math1->course->postreq_nodes.push_back(pair<bool, AugNode*>(false, math2));
		math2->course->prereq_nodes.push_back(pair<bool, AugNode*>(false, math1));
		temp_days = { DAY_OF_WEEK::MON,DAY_OF_WEEK::WED };
		quarters = {WINTER};
		struct AugNode* math3 = new AugNode(new CourseNode(101100,quarters,pair<int,int>(20, 28), temp_days));
		math1->course->postreq_nodes.push_back(pair<bool, AugNode*>(false, math3));
		math3->course->prereq_nodes.push_back(pair<bool, AugNode*>(false, math1));
		temp_days = { DAY_OF_WEEK::MON,DAY_OF_WEEK::WED };
		struct AugNode* math4 = new AugNode(new CourseNode(101091, quarters, pair<int, int>(28, 36), temp_days));
		math1->course->postreq_nodes.push_back(pair<bool, AugNode*>(false, math4));
		math4->course->prereq_nodes.push_back(pair<bool, AugNode*>(false, math1));
		temp_days = { DAY_OF_WEEK::TUE,DAY_OF_WEEK::THUR };
		struct AugNode* math5 = new AugNode(new CourseNode(101092, quarters, pair<int, int>(28, 36), temp_days));
		math4->course->postreq_nodes.push_back(pair<bool, AugNode*>(false, math5));
		math5->course->prereq_nodes.push_back(pair<bool, AugNode*>(false, math4));
		temp_days = { DAY_OF_WEEK::TUE,DAY_OF_WEEK::THUR };
		struct AugNode* math6 =new AugNode(new CourseNode(101098, quarters, pair<int, int>(4, 12), temp_days));
		math4->course->postreq_nodes.push_back(pair<bool, AugNode*>(false, math6));
		math6->course->prereq_nodes.push_back(pair<bool, AugNode*>(false, math4));
		temp_days = { DAY_OF_WEEK::MON,DAY_OF_WEEK::WED };
		quarters = { FALL };
		struct AugNode* math7 = new AugNode(new CourseNode(101141, quarters, pair<int, int>(12, 20), temp_days));
		math5->course->postreq_nodes.push_back(pair<bool, AugNode*>(false, math7));
		math7->course->prereq_nodes.push_back(pair<bool, AugNode*>(false, math5));
		temp_days = { DAY_OF_WEEK::MON,DAY_OF_WEEK::WED };
		struct AugNode* math8 = new AugNode(new CourseNode(101138, quarters, pair<int, int>(12, 20), temp_days));
		math5->course->postreq_nodes.push_back(pair<bool, AugNode*>(false, math8));
		math8->course->prereq_nodes.push_back(pair<bool, AugNode*>(false, math5));
		temp_days = { DAY_OF_WEEK::MON,DAY_OF_WEEK::WED };
		quarters = {SPRING};
		struct AugNode* math9 =new AugNode(new CourseNode(101099, quarters, pair<int, int>(32, 40), temp_days));
		startNodes.push_back(math9);
		temp_days = { DAY_OF_WEEK::TUE,DAY_OF_WEEK::THUR };
		struct AugNode* math10 = new AugNode(new CourseNode(101146, quarters, pair<int, int>(32, 40), temp_days));
		math9->course->postreq_nodes.push_back(pair<bool, AugNode*>(false, math10));
		math10->course->prereq_nodes.push_back(pair<bool, AugNode*>(false, math9));
		math6->course->postreq_nodes.push_back(pair<bool, AugNode*>(false, math10));
		math10->course->prereq_nodes.push_back(pair<bool, AugNode*>(false, math6));
		temp_days = { DAY_OF_WEEK::TUE,DAY_OF_WEEK::THUR };
		quarters = { WINTER };
		struct AugNode* math11 = new AugNode(new CourseNode(101107, quarters, pair<int, int>(30, 38), temp_days));
		math9->course->postreq_nodes.push_back(pair<bool, AugNode*>(false, math11));
		math11->course->prereq_nodes.push_back(pair<bool, AugNode*>(false, math9));
		math6->course->postreq_nodes.push_back(pair<bool, AugNode*>(false, math11));
		math11->course->prereq_nodes.push_back(pair<bool, AugNode*>(false, math6));
		quarters = { FALL };
		struct AugNode* math12 = new AugNode(new CourseNode(101142, quarters, pair<int, int>(12, 20), temp_days));
		math7->course->postreq_nodes.push_back(pair<bool, AugNode*>(false, math12));
		math12->course->prereq_nodes.push_back(pair<bool, AugNode*>(false, math7));
		struct AugNode* math13 = new AugNode(new CourseNode(101148, quarters, pair<int, int>(12, 20), temp_days));
		math7->course->postreq_nodes.push_back(pair<bool, AugNode*>(false, math13));
		math13->course->prereq_nodes.push_back(pair<bool, AugNode*>(false, math7));
		math8->course->postreq_nodes.push_back(pair<bool, AugNode*>(false, math13));
		math13->course->prereq_nodes.push_back(pair<bool, AugNode*>(false, math8));
		temp_days = { DAY_OF_WEEK::MON,DAY_OF_WEEK::WED };
		quarters = { SPRING };
		struct AugNode* math14 = new AugNode(new CourseNode(101144, quarters, pair<int, int>(30, 38), temp_days));
		startNodes.push_back( math14);
		temp_days = { DAY_OF_WEEK::TUE,DAY_OF_WEEK::THUR };
		quarters = { WINTER };
		struct AugNode* math15 = new AugNode(new CourseNode(101151, quarters, pair<int, int>(14, 22), temp_days));
		math14->course->postreq_nodes.push_back(pair<bool, AugNode*>(false, math15));
		math15->course->prereq_nodes.push_back(pair<bool, AugNode*>(false, math14));
		math12->course->postreq_nodes.push_back(pair<bool, AugNode*>(false, math15));
		math15->course->prereq_nodes.push_back(pair<bool, AugNode*>(false, math12));
		temp_days = { DAY_OF_WEEK::TUE,DAY_OF_WEEK::THUR };
		quarters = { WINTER };
		struct AugNode* math16 = new AugNode(new CourseNode(101152, quarters, pair<int, int>(36, 44), temp_days));
		math15->course->postreq_nodes.push_back(pair<bool, AugNode*>(false, math16));
		math16->course->prereq_nodes.push_back(pair<bool, AugNode*>(false, math15));
		temp_days = { DAY_OF_WEEK::MON,DAY_OF_WEEK::WED };
		quarters = { WINTER };
		struct AugNode* math17 =new AugNode(new CourseNode(101163, quarters, pair<int, int>(30, 38), temp_days));
		math16->course->postreq_nodes.push_back(pair<bool, AugNode*>(false, math17));
		math17->course->prereq_nodes.push_back(pair<bool, AugNode*>(false, math16));
		quarters = { SPRING };
		struct AugNode* math18 = new AugNode(new CourseNode(101264, quarters, pair<int, int>(16, 24), temp_days));
		temp_days = { DAY_OF_WEEK::TUE,DAY_OF_WEEK::THUR};
		struct AugNode* math19 = new AugNode(new CourseNode(101260, quarters, pair<int, int>(20, 28), temp_days));
		temp_days = { DAY_OF_WEEK::MON,DAY_OF_WEEK::WED };
		quarters = { FALL };
		struct AugNode* math20 = new AugNode(new CourseNode(101261, quarters, pair<int, int>(0, 8), temp_days));

		math17->course->postreq_nodes.push_back(pair<bool, AugNode*>(false, math18));
		math18->course->prereq_nodes.push_back(pair<bool, AugNode*>(false, math17));
		math17->course->postreq_nodes.push_back(pair<bool, AugNode*>(false, math19));
		math19->course->prereq_nodes.push_back(pair<bool, AugNode*>(false, math17));
		math17->course->postreq_nodes.push_back(pair<bool, AugNode*>(false, math20));
		math20->course->prereq_nodes.push_back(pair<bool, AugNode*>(false, math17));
		vector<AugNode*> endNodes;
		endNodes.push_back( math18);
		endNodes.push_back(math19);
		endNodes.push_back( math20);
		return CrsPairs(startNodes, endNodes);*/
	
}


void clean_up(map<AugNode*, CourseMatrix>* paths, map<int, CourseNode*>* _crs_details)
{
	for (auto it = paths->begin(); it != paths->end(); it++)
	{
		CourseMatrix& matrix = it->second;
		for (int i = 0; i < matrix.size(); i++)
		{
			for (int j = 0; j < matrix[i].size(); j++)
			{
				delete matrix[i][j];
			}
		}
		delete it->first;
	}

	for (auto it = _crs_details->begin(); it != _crs_details->end(); it++)
	{
		delete it->second;
	}

	delete _crs_details;
}


void generate_plans(const char* input, char* output, size_t len)
{

	
	Parser* parser = new JsonParser();

	map<int, CourseNode*>* _crs_details = new map<int, CourseNode*>(); //holds all other course information like class schedules, etc
	auto paths = parser->parse_input(input, _crs_details);

	cout << "Graph built" << endl;
	CoursePlanner planner(new SerialPlanner(_crs_details));

	QuarterNode start_qtr;
	start_qtr.quarter = FALL;
	start_qtr.year = 2016;
	auto plans = planner.planPhase2(paths, start_qtr,TIME_OF_DAY::DAY,10,3500);
	
	string json_result = parser->generate_output_str(plans);
	delete parser;
	strcpy_s(output, len, json_result.c_str());
	cout << "Phase 2 Completed" << endl;
	clean_up(&paths, _crs_details);

}