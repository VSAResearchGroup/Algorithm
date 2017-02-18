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
	//parses a single course
	//j_crs: json object encapsulating details of a single course
	AugNode* parse_single_crs(json& j_crs);

	//parses the entire json input recursively
	//j_crs: JSON course object being parsed
	//all_crses: entire input encoded in a JSON object
	//temp: used for dynamic programming to prevent reparsing a parsed course while walking the tree
	AugNode* parse_all_recursive(json& j_crs, json& all_crses, map<int, AugNode*>& temp);
	
public:
	JsonParser();
	//a virtual implementation of the Planner.parse_input designed for parsing a json string
	//input: serialized JSON string
	virtual const CrsPairs parse_input(const char* input);

	//a virtual implementation of the Planner.generate_output_str designed for generating a json string
	//output: a list of tentative degree plans C++ objects
	virtual const string generate_output_str(vector<DegreePlan>&  output);
	
};

#endif