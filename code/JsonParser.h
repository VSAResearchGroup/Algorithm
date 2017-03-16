#ifndef JSON_INPUT_PARSER_H
#define JSON_INPUT_PARSER_H
#include <stack>
#include <vector>
#include <map>
#include "Parser.h"
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

//JSON string serializer and deserializer
class JsonParser : public Parser
{
private:
	int remap_qtr_for_output(QUARTER input);
	
public:
	JsonParser() {}
	//a virtual implementation of the Planner.parse_input designed for parsing a json string
	//input: serialized JSON string
	virtual const map<AugNode*, CourseMatrix> parse_input(const char* input, map<int, CourseNode*>* _crs_details);

	//a virtual implementation of the Planner.generate_output_str designed for generating a json string
	//output: a list of tentative degree plans C++ objects
	virtual const string generate_output_str(map<int,DegreePlan>&  output);
	
};

#endif