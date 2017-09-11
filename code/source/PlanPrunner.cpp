#include "../headers/PlanPrunner.h"



assessment PlanPrunner::assess_plan(const DegreePlan& plan, TIME_OF_DAY tod_pref, int max_credit_pref, float max_budget_pref)
{
	assessment results;
	results.time_of_day_score.second = compute_tod_preference(plan, tod_pref);
	results.max_credits_score.second = compute_max_credit_preference(plan, max_credit_pref);
	results.max_budget_score.second = compute_max_budget_preference(plan, max_budget_pref);
	results.compute_aggregate();
	return results;

}


float PlanPrunner::compute_tod_preference(const DegreePlan& plan, TIME_OF_DAY tod_pref)
{
	if (tod_pref == 7)
	{ //all last 3 bits set
		return 1.0; //anything goes (so max score)
	}

	float sum_score = 0.0; //used to hold cumulative score for each quarter
	for (auto quarter = plan.begin(); quarter != plan.end(); quarter++)
	{
		float score = 1.0;
		if (quarter->second.size() == 0)
		{
			sum_score += score;
			continue; //avoid division by zero
		}
		float decrement = 1.0 / quarter->second.size();
		
		for (auto& crs : quarter->second)
		{
			TIME_OF_DAY crs_bit_flag;
			int start_time = crs->get_current_schedule().schedules[0].time.first;

			if (start_time < NOON_MARKER)
			{
				crs_bit_flag = 1 << 2;
			}
			else if(start_time < EVENING_MARKER)
			{
				crs_bit_flag = 1 << 1;
			}
			else
			{
				crs_bit_flag = 1;
			}

			if (!(tod_pref & crs_bit_flag))
			{
				score -= decrement;
				break;
			}

		}
		sum_score += score;
	}
	//return the average score from each quarter
	return sum_score / plan.size();

}



float PlanPrunner::compute_max_credit_preference(const DegreePlan& plan, int max_credit_pref)
{


	float sum_score = 0.0; //used to hold cumulative score for each quarter
	for (auto quarter = plan.begin(); quarter != plan.end(); quarter++)
	{
		float score = 1.0;
		
		//compute approximate number of credits
		float credits = 0.0;
		for (auto& crs : quarter->second)
		{
			float crdt = 0.0;
			crdt += crs->compute_no_of_credits();
			crdt *= 1.2; //conversion rate from hours to credits is h * 1.2c
			credits += crdt;
		}
		if (credits > max_credit_pref)
		{
			//once no of credits is twice the max allowed or more, score for that quarter is 0
			score -= fminf(0, (credits - max_credit_pref) / max_credit_pref);
		}

		sum_score += score;
	}
	//return the average score from each quarter
	return sum_score / plan.size();

}


float PlanPrunner::compute_max_budget_preference(const DegreePlan& plan, float max_budget_pref)
{


	float sum_score = 0.0; //used to hold cumulative score for each quarter
	for (auto quarter = plan.begin(); quarter != plan.end(); quarter++)
	{
		float score = 1.0;

		//compute approximate cost
		float total_cost = 0.0;
		float credits = 0.0;
		for (auto& crs : quarter->second)
		{
			float crdt = 0.0;
			
			crdt += crs->compute_no_of_credits();
			crdt *= 1.2; //conversion rate from hours to credits is h * 1.2c
			credits += crdt;
		}
		total_cost = compute_cost(credits);
		
		//use bi-directional interpolation to approximate the score from both inequalities sides
		score -= fminf(0, fabsf(total_cost - max_budget_pref) / max_budget_pref);
		

		sum_score += score;
	}
	//return the average score from each quarter
	return sum_score / plan.size();

}


float PlanPrunner::compute_cost(float credits)
{
	const float cost_per_credit = 500.0;
	int free_credits_cap = 12;
	if (credits < free_credits_cap)
	{
		return credits * cost_per_credit;
	}
	return free_credits_cap * cost_per_credit;
}