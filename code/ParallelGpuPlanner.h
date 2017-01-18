#ifndef PARALLEL_GPU_PLANNER_H
#define PARALLEL_GPU_PLANNER_H

#include <amp.h>
#include <queue>
#include <limits>
#include "IPlanner.h"

using namespace concurrency;



class ParallelGpuPlanner: public IPlanner
{
private:
	
	vector<unsigned int> construct_adjacency_matrix(vector<AugNode*>& startNodes,vector<AugNode*> endNodes,vector<int>& courses,vector<int>& endCourseIndices);

	void augment_gpu_matrix(vector<int>& courses, vector<unsigned int>& matrix, vector<int>& childSize, const int rowSize,vector<int>& endCourseIndex);

	void enqueue(array_view<int, 1>* queue) restrict(amp); //queue uses last three elements to maintain state

	int dequeue(array_view<int, 1>* queue) restrict(amp);

	bool empty(array_view<int, 1>* queue) restrict(amp);
	
public:
	map<AugNode*, CourseMatrix> phase1(vector<AugNode*>& startNodes, vector<AugNode*>& endNodes);

};

#endif
