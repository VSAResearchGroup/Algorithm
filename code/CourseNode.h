#ifndef COURSE_NODE_H
#define COURSE_NODE_H


#include <vector>




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

struct CourseNode
{
	int course_code;
	std::vector<QUARTER> quarters;
	std::pair<int, int> schedule;
	std::vector<DAY_OF_WEEK> days;
	std::vector<std::pair<bool,AugNode*>> postreq_nodes;
	std::vector<std::pair<bool, AugNode*>> prereq_nodes;

	CourseNode(int code,std::vector<QUARTER> quarter,std::pair<int,int> schedule,std::vector<DAY_OF_WEEK> days): course_code(code),quarters(quarter),schedule(schedule),days(days) {}
};

struct NDPair
{
	int distance;
	int courseCode;
};

struct Probe
{
	std::vector<AugNode*> path;
	int originCrs;
};



struct AugNode
{
	CourseNode* course;
	std::vector<NDPair> distances;
	std::vector<Probe> currProbes;
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

#endif