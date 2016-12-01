#include "SerialPlanner.h"
#include <queue>


void SerialPlanner::augment_graph(vector<AugNode*>& endNodes)
{
	std::queue<AugNode*> q;
	//augment graph
	for (auto course = endNodes.begin(); course != endNodes.end(); course++)
	{
		AugNode* crs = *course;
		NDPair nd;
		nd.courseCode = crs->course->course_code;
		nd.distance = 0;
		crs->distances.push_back(nd);
		crs->isQueued1 = true;
		q.push(crs);
	}

	while (!q.empty())
	{
		AugNode*  currNode = q.front();
		q.pop();
		for (auto node = currNode->course->postreq_nodes.begin(); node != currNode->course->postreq_nodes.end(); node++)
		{
			AugNode* nd = *node;

			for (auto distCell = nd->distances.begin(); distCell != nd->distances.end(); distCell++)
			{
				NDPair d = *distCell;
				d.distance++;
				currNode->distances.push_back(d);
			}


		}

		for (auto node = currNode->course->prereq_nodes.begin(); node != currNode->course->prereq_nodes.end(); node++)
		{
			AugNode* nd = *node;
			if (!nd->isQueued1)
			{
				q.push(*node);
				nd->isQueued1 = true;
			}
		}

	}


}

void SerialPlanner::probe_graph(vector<AugNode*>& startNodes, vector<AugNode*>& endNodes, map<AugNode*, CourseMatrix>& output)
{
	queue<AugNode*> q;

	for (auto endNode = endNodes.begin(); endNode != endNodes.end(); endNode++)
	{
		AugNode* en = *endNode;
		//send probe for each end node's pre-requisite

		for (auto node = en->course->prereq_nodes.begin(); node != en->course->prereq_nodes.end(); node++)
		{
			Probe probe;
			probe.originCrs = en->course->course_code;
			probe.path.push_back(en);
			AugNode* pre_req = *node;
			pre_req->currProbes.push_back(probe);
			if (!pre_req->isQueued2)
			{
				q.push(pre_req);
				pre_req->isQueued2 = true;
			}
		}

	}

	while (!q.empty())
	{
		AugNode* node = q.front();
		
		q.pop();
		if (node->course->prereq_nodes.empty())
		{
			//start node AKA end of path for each probe going through this node
			for (auto probe = node->currProbes.begin(); probe != node->currProbes.end(); probe++)
			{
				probe->path.push_back(node);
				if (output.find(probe->path[0]) == output.end())
				{
					CourseMatrix paths;
					paths.push_back(probe->path);
					output.insert(pair<AugNode*, CourseMatrix>(probe->path[0], paths));
				}
				else {
					//insertion sort
					CourseMatrix& paths = output[probe->path[0]];
					bool inserted = false;
					for (auto path = paths.begin(); path != paths.end(); path++)
					{
						vector<AugNode*> pt = *path;
						if (probe->path.size() <= pt.size())
						{
							inserted = true;
							paths.insert(path, probe->path);
							break;
						}
					}
					if (!inserted)
						paths.push_back(probe->path);
				}

			}


		}
		else
		{
			//intermediate node
			for (auto probe = node->currProbes.begin(); probe != node->currProbes.end(); probe++)
			{
				probe->path.push_back(node);
				Probe prb = *probe;
				for (auto pre_req = node->course->prereq_nodes.begin(); pre_req != node->course->prereq_nodes.end(); pre_req++)
				{
					AugNode* prq = *pre_req;
					for (auto ndp = prq->distances.begin(); ndp != prq->distances.end(); ndp++)
					{
						NDPair& dp = *ndp;
						if (dp.courseCode == prb.originCrs && dp.distance != -1)
						{
							//cut through first cell for probe's origin
							dp.distance = -1;
							prq->currProbes.push_back(prb);
							//queue pre-req node if not queued
							if (!prq->isQueued2)
							{
								q.push(prq);
								prq->isQueued2 = true;
							}
							break;
						}
					}

				}
			}
		}
		node->currProbes.clear();
	}
}



map<AugNode*, CourseMatrix> SerialPlanner::phase1(vector<AugNode*>& startNodes, vector<AugNode*>& endNodes)
{
	map<AugNode*, CourseMatrix> pathsForAllEnds;
	augment_graph(endNodes);
	probe_graph(startNodes, endNodes, pathsForAllEnds);
	return pathsForAllEnds;
}

