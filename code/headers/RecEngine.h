#ifndef REC_ENGINE_H
#define REC_ENGINE_H

#include "stdafx.h"
void build_graph();
void clean_up(map<AugNode*, CourseMatrix>* paths, map<int, CourseNode*>* _crs_details);
void print_plans(vector<DegreePlan>& plans);

extern "C"
{
	__declspec(dllexport) void generate_plans(const char* input, char* output, size_t len);
}

#endif

