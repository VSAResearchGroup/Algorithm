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
	//input: course pre-requisite network serialized as string
	virtual const CrsPairs parse_input(const char* input) = 0;

	//Generates a serialized version of the degree plans
	//output: a list of tentative degree plans C++ objects
	virtual const string generate_output_str(vector<DegreePlan>&  output) = 0;
};



#endif
