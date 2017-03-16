#include "CourseNode.h"

QuarterNode QuarterNode::next_qtr()
{
	QuarterNode qtr = *this;
	switch (quarter)
	{
	case FALL:
		qtr.quarter = WINTER;
		qtr.year++;
		break;
	case WINTER:
		qtr.quarter = SPRING;
		break;
	case SPRING:
		qtr.quarter = SUMMER;
		break;
	case SUMMER:
		qtr.quarter = FALL;
		break;
	}
	return qtr;
}


bool QuarterNode::operator != (const QuarterNode& rhs) const
{
	return (static_cast<int>(quarter) != static_cast<int>(rhs.quarter)) || (year != rhs.year);
}

bool QuarterNode::operator ==(const QuarterNode& rhs) const
{
	return (static_cast<int>(quarter) == static_cast<int>(rhs.quarter)) && (year == rhs.year);
}

bool QuarterNode::operator <(const QuarterNode& rhs) const
{
	if (year != rhs.year)
	{
		return year < rhs.year;
	}
	else
	{
		return (static_cast<int>(quarter) < static_cast<int>(rhs.quarter));
	}
}


QuarterNode get_crs_next_feasible_qtr(CourseNode* crs, QuarterNode curr_qtr)
{
	int next_qtr_index = find(crs->quarters.begin(), crs->quarters.end(), curr_qtr.quarter) - crs->quarters.begin();
	QuarterNode output;
	output.year = curr_qtr.year;
	if ((++next_qtr_index) >= crs->quarters.size())
	{
		//cycle back to the first
		output.quarter = crs->quarters[0];
		output.year++;
		return output;
	}
	else
	{
		output.quarter = crs->quarters[next_qtr_index];
	}
	if (static_cast<int>(output.quarter) < static_cast<int>(curr_qtr.quarter))
	{
		output.year++;
	}
	return output;
}



assessment::assessment()
{
	//use 1.0 for now
	max_budget_score.first = 1.0; 
	max_credits_score.first = 1.0;
	time_of_day_score.first = 1.0;
}

void assessment::compute_aggregate()
{
	aggregate = (max_budget_score.first * max_budget_score.second + max_credits_score.first * max_credits_score.second +
		time_of_day_score.first * time_of_day_score.second) / (max_budget_score.first + max_credits_score.first +
			time_of_day_score.first);
}