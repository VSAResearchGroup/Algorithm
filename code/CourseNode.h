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
typedef unsigned char TIME_OF_DAY;



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

struct DayTimeFlags
{
	static const TIME_OF_DAY morning = 1 << 2; //0100
	static const TIME_OF_DAY noon = 1 << 1; //0010
	static const TIME_OF_DAY evening = 1; //0001
};

const int NOON_MARKER = 72; //marks the integral value of time when noon classes (12PM)
const int EVENING_MARKER = 102; //marks the integral value of time when evening classes starts (5PM)

const int MAX_POSSIBLE_CREDITS_QTR = 25;

struct schedule
{
	DAY_OF_WEEK day;
	pair<int, int> time;
};

struct CourseSchedule
{
	QUARTER qtr;
	vector<schedule> schedules;
};

struct CourseNode
{
	int course_code;
	vector<CourseSchedule> crs_schedules;
	int current_sch_index;
	vector<int> pre_reqs;
	bool isQueued;
	bool isFloating;
	CourseNode(int code,vector<CourseSchedule> schedules): course_code(code),crs_schedules(schedules), current_sch_index(0), isQueued(false), isFloating(false) {}
	CourseSchedule get_current_schedule();
	float compute_no_of_credits();
};





struct AugNode
{
	vector<int> course_ids; //list course Ids

	AugNode() {}
	AugNode(vector<int> course_ids) :course_ids(course_ids){}
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




//a structure that encapsulates the score of a plan in each dimension (from 0 -1) and their respective weights
struct assessment
{
	pair<float,float> time_of_day_score;
	pair<float, float> max_credits_score;
	pair<float, float> max_budget_score;

	float aggregate;

	assessment();
	void compute_aggregate();
};

#endif