#include "ParallelGpuPlanner.h"


void ParallelGpuPlanner::enqueue(array_view<int, 1>* queue) restrict(amp)
{
	
}


vector<unsigned int> ParallelGpuPlanner::construct_adjacency_matrix(vector<AugNode*>& startNodes, vector<AugNode*> endNodes, vector<int>& courses, vector<int>& endCourseIndices)
{
	queue<AugNode*> q;
	vector<bool> temp; //represents a single matrix row
	for (auto sn = startNodes.begin(); sn != startNodes.end(); sn++)
	{
		q.push(*sn);
	}
	map<int, int> indexMap;
	
	//1st pass to determine the row/col size of matrix
	int rowSize = 0;
	while (!q.empty())
	{
		AugNode* node = q.front();
		q.pop();
		if (find(endNodes.begin(), endNodes.end(), node) != endNodes.end())
		{
			endCourseIndices.push_back(rowSize);
		}
		indexMap.insert(pair<int, int>(node->course->course_code, rowSize++));
		temp.push_back(false);
		courses.push_back(node->course->course_code);
		for (auto n = node->course->postreq_nodes.begin(); n != node->course->postreq_nodes.end(); n++)
		{
			AugNode* nd = *n;
			if (!nd->isQueued1)
			{
				q.push(nd);
				nd->isQueued1 = true;
			}
		}

	}
	
	vector<unsigned int> matrix(rowSize*rowSize, UINT_MAX);
	//2nd pass to build matrix
	for (auto sn = startNodes.begin(); sn != startNodes.end(); sn++)
	{
		q.push(*sn);
	}

	//1st pass to determine the row/col size of matrix
	int rowOffset = 0;
	while (!q.empty())
	{
		AugNode* node = q.front();
		q.pop();
		int edgeCount = 0;
		for (auto n = node->course->postreq_nodes.begin(); n != node->course->postreq_nodes.end(); n++)
		{
			AugNode* nd = *n;
			matrix[rowOffset + indexMap[nd->course->course_code]] = indexMap[nd->course->course_code];
			matrix[rowSize * indexMap[nd->course->course_code] + indexMap[node->course->course_code]] = indexMap[node->course->course_code] | (2<< (sizeof(int) * 8 -1)); //set the first bit to indicate parenthood
			if (!nd->isQueued2)
			{
				q.push(nd);
				nd->isQueued2 = true;
			}
			++edgeCount;
		}
		rowOffset += rowSize;		
	
	}
	return matrix;
}

void ParallelGpuPlanner::augment_gpu_matrix(vector<int>& courses, vector<unsigned int>& matrix, vector<int>& childSize,const int rowSize,vector<int>& endCourseIndex)
{
	
	struct NdPair_G
	{
		int crs_origin;
		int distance;
	};
	array_view<unsigned int, 2> matrix_g(rowSize, rowSize, matrix);
	array_view<int, 1> courses_g(courses.size(), courses);
	vector<int> cell_count(courses.size(), 0);
	array_view<int, 1> cell_count_g(cell_count.size(),cell_count);
	array_view<int, 1> endIndices_g(endCourseIndex.size(), endCourseIndex);
	unsigned int flag = 2 << (sizeof(int) * 8 - 1);
	int cell_temp[] = { 0 };
	array_view<int, 1> total_cells(1, cell_temp);
	parallel_for_each(endIndices_g.extent,[=](index<1> idx) restrict(amp){
		cell_count_g[endIndices_g[idx]] = 1;
		total_cells[0]++;
		int distance = 0;
		struct NdPair_G nd;
		nd.crs_origin = courses_g[endIndices_g[idx]];
		nd.distance = 0;
		for (int i = 0; i < courses_g.extent[0]; i++)
		{
			index<2> id(idx[0], i);
			if ((matrix_g[id] != UINT_MAX) && (matrix_g[id] & flag))
			{
				
			}
		}
	});
}
