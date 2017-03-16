#ifndef PARSER_H
#define PARSER_H

#include <vector>

#include "CourseNode.h"

using namespace std;

typedef pair<vector<AugNode*>, vector<AugNode*>> CrsPairs;

//a unified interface for string serializers and deserializers
class Parser
{
public:
	//Parses input from serialized string to concrete c++ objects
	//input: course pre-requisite network paths serialized as string
	virtual const map<AugNode*, CourseMatrix> parse_input(const char* input, map<int, CourseNode*>* _crs_details) = 0;

	//Generates a serialized version of the degree plans
	//output: a list of tentative degree plans C++ objects
	virtual const string generate_output_str(map<int, DegreePlan>&  output) = 0;
};



#endif
