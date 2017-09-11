#ifndef PLAN_PRUNNER_H
#define PLAN_PRUNNER_H

#include "CourseNode.h"



//Helps in assessing plan's closeness to user preference
class PlanPrunner
{

private:
	/*
	a mock implementation of quarter course fees computation
	credits: number of credits to be taken that quarter
	RETURNS: total cost
	*/
	static float compute_cost(float credits);

public:
	/*
	computes an aggregate score on a scale 0 - 1 representing how suitable a plan is for students based on their preferences
	plan: degree plan under consideration
	tod_pref: preferred time of day (day, evening, or either)
	max_credit_pref: maximum number of credits to be taken per quarter
	max_budget_pref: approximate budget limit per quarter
	RETURNS: aggregate score for each assessment metric
	*/
	static assessment assess_plan(const DegreePlan& plan,TIME_OF_DAY tod_pref, int max_credit_pref,float max_budget_pref);

	/*
	computes suitability for time of day on a scale of 0 - 1
	plan: degree plan under consideration
	tod_pref: bitflag for preferred time of day(morning, afternoon, evening, or either)
	RETURNS: a normalized score for the preferred time of day metric
	*/
	static float compute_tod_preference(const DegreePlan& plan, TIME_OF_DAY tod_pref);

	/*
	computes suitability for maximum quarter credits on a scale of 0 - 1
	plan: degree plan under consideration
	max_credit_pref: maximum number of credits to be taken per quarter
	RETURNS: a normalized score for the maximum number of quarter credits metric
	*/
	static float compute_max_credit_preference(const DegreePlan& plan, int max_credit_pref);

	/*
	computes suitability for maximum quarter credits on a scale of 0 - 1
	plan: degree plan under consideration
	max_budget_pref: approximate budget limit per quarter
	RETURNS: a normalized score for the maximum quater budget metric
	*/
	static float compute_max_budget_preference(const DegreePlan& plan, float max_budget_pref);

};

#endif
