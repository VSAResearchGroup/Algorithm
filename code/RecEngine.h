#ifndef REC_ENGINE_H
#define REC_ENGINE_H

#include "stdafx.h"
void build_graph();
void post_order_removal(AugNode* node);
void clean_up(vector<AugNode*>& startNodes);
void print_paths(map<AugNode*, CourseMatrix>& paths);
void print_plans(vector<DegreePlan>& plans);

extern "C"
{
	__declspec(dllexport) void generate_plans(const char* input, char* output, size_t len);
}

#endif

