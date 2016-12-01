#ifndef COURSE_NODE_H
#define COURSE_NODE_H
#include <vector>

struct AugNode;
struct Probe;

struct CourseNode
{
	int course_code;
	std::vector<AugNode*> postreq_nodes;
	std::vector<AugNode*> prereq_nodes;

	CourseNode(int code): course_code(code) {}
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





#endif