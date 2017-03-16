#ifndef COURSE_NODE_H
#define COURSE_NODE_H


#include <vector>
#include <map>

using namespace std;




struct AugNode;

struct CourseNode;
struct QuarterNode;


typedef vector<vector<AugNode*>> CourseMatrix;
typedef unsigned short ushort;
typedef map<QuarterNode, vector<CourseNode*>> DegreePlan;


enum QUARTER
{
	WINTER = 1,
	SPRING = 2,
	SUMMER = 3,
	FALL = 4
};

enum DAY_OF_WEEK
{
	MON = 1,
	TUE = 2,
	WED = 3,
	THUR = 4,
	FRI = 5
};

struct schedule
{
	DAY_OF_WEEK day;
	pair<int, int> time;
};

struct CourseNode
{
	int course_code;
	vector<QUARTER> quarters;
	vector<schedule> schedules;
	vector<int> post_reqs;
	bool isQueued;
	CourseNode(int code, vector<QUARTER> quarter, vector<schedule> schedules) : course_code(code), quarters(quarter), schedules(schedules), isQueued(false) {}
};





struct AugNode
{
	vector<int> course_ids; //list course Ids

	AugNode() {}
	AugNode(vector<int> course_ids) :course_ids(course_ids) {}
	~AugNode() { }
};




struct QuarterNode
{

	QUARTER quarter;
	unsigned short year;

	QuarterNode next_qtr();


	bool operator !=(const QuarterNode& rhs) const;

	bool operator ==(const QuarterNode& rhs) const;

	bool operator <(const QuarterNode& rhs) const;

};



//gets the next possible quarter a course can be taken after the current quarter
//crs: pointer to a course and its details
//curr_qtr: current quarter under consideration
//RETURNS: the next possible quarter, e.g. a course taken only in FALL 2016 will return FALL 2017
QuarterNode get_crs_next_feasible_qtr(CourseNode* crs, QuarterNode curr_qtr);



#endif