#ifndef COURSE_NODE_H
#define COURSE_NODE_H


#include <vector>
#include <map>

using namespace std;




struct AugNode;
struct Probe;

enum QUARTER
{
	WINTER =1,
	SPRING = 2,
	SUMMER =3,
	FALL = 4
};

enum DAY_OF_WEEK
{
	MON = 1,
	TUE =2,
	WED =3,
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
	vector<pair<bool,AugNode*>> postreq_nodes;
	vector<pair<bool, AugNode*>> prereq_nodes;

	CourseNode(int code,vector<QUARTER> quarter,vector<schedule> schedules): course_code(code),quarters(quarter),schedules(schedules) {}
};

struct NDPair
{
	int distance;
	int courseCode;
};

struct Probe
{
	vector<AugNode*> path;
	int originCrs;
};



struct AugNode
{
	CourseNode* course;
	vector<NDPair> distances;
	vector<Probe> currProbes;
	bool isQueued1; //to prevent recomputations
	bool isQueued2; 
	AugNode(CourseNode* course) :course(course), isQueued1(false), isQueued2(false){}
	~AugNode() { delete course; }
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
QuarterNode get_crs_next_feasible_qtr(AugNode* crs, QuarterNode curr_qtr);


typedef vector<vector<AugNode*>> CourseMatrix;
typedef unsigned short ushort;
typedef map<QuarterNode, vector<AugNode*>> DegreePlan;
#endif