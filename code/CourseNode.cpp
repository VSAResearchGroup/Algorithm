#include "CourseNode.h"



float CourseNode::compute_no_of_credits()
{
	CourseSchedule sch = crs_schedules[0];
	float total_credits = 0.0;
	for (auto& interval : sch.schedules)
	{
		total_credits += (interval.time.second - interval.time.first) / 6.0;
	}
	total_credits *= 1.2;
	return total_credits;
}

CourseSchedule CourseNode::get_current_schedule()
{
	return crs_schedules[current_sch_index];
}
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

bool QuarterNode::operator<=(const QuarterNode& rhs) const
{
	return (*this) < rhs || (*this) == rhs;
}


bool QuarterNode::operator >=(const QuarterNode& rhs) const
{
	if (this->year > rhs.year)
	{
		return true;
	}
	
	if (this->year == rhs.year)
	{
		return this->quarter >= rhs.quarter;
	}

	return false;
}

bool rank_index::operator != (const rank_index& rhs) const
{
	return  this->index != rhs.index;
}

bool rank_index::operator == (const rank_index& rhs) const
{
	return   this->index == rhs.index;
}


bool rank_index::operator < (const rank_index& rhs) const
{
	return  this->index < rhs.index;
}






QuarterNode get_crs_next_feasible_qtr(CourseNode* crs, QuarterNode curr_qtr)
{
	QuarterNode output;
	output.year = curr_qtr.year;
	do {
		
		int past_index = crs->current_sch_index;
		crs->current_sch_index = (1 + crs->current_sch_index) % crs->crs_schedules.size();


		output.quarter = crs->get_current_schedule().qtr;

		if (crs->get_current_schedule().qtr <= crs->crs_schedules[past_index].qtr)
		{
				output.year++;
		}

	} while (output <= curr_qtr);
	

	
	return output;
}



QuarterNode get_crs_prev_feasible_qtr(CourseNode* crs, QuarterNode curr_qtr)
{
	QuarterNode output;
	output.year = curr_qtr.year;
	do {

		int next_index = crs->current_sch_index;
		crs->current_sch_index = ( crs->current_sch_index -1  < 0 ?  crs->crs_schedules.size() - 1 : crs->current_sch_index -1 );


		output.quarter = crs->get_current_schedule().qtr;

		if (crs->get_current_schedule().qtr >= crs->crs_schedules[next_index].qtr)
		{
			output.year --;
		}
	} while (output >= curr_qtr);

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
	aggregate =  (max_budget_score.first * max_budget_score.second + max_credits_score.first * max_credits_score.second +
		time_of_day_score.first * time_of_day_score.second) / (max_budget_score.first + max_credits_score.first +
			time_of_day_score.first);
}