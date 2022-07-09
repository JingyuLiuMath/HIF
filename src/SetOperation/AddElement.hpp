#include "HIF.hpp"

namespace HIF {

// Add an element to a sorted vector vec.
template <typename Scalar>
void AddElement_Sort(vector<Scalar>& vec, Scalar value)
{
	DEBUG_HIF(CallStackEntry cse("Intersect_Sort"))

	TIMER_HIF(TimerStart(TIMER_SETOP))
	if (vec.size() == 0)
	{
		vec.push_back(value);
		TIMER_HIF(TimerStop(TIMER_SETOP))
		return;
	}
	int startsearch = 0;
	int endsearch = vec.size() - 1;
	int tmpindex = -1;
	int index = -1;
	if (vec.front() > value)
	{
		vec.insert(vec.begin(), value);
		TIMER_HIF(TimerStop(TIMER_SETOP))
		return;
	}
	if (vec.back() < value)
	{
		vec.push_back(value);
		TIMER_HIF(TimerStop(TIMER_SETOP))
		return;
	}
	while (startsearch <= endsearch)
	{
		tmpindex = (startsearch + endsearch) / 2;
		if (vec[tmpindex] < value)
		{
			startsearch = tmpindex + 1;
		}
		else if (vec[tmpindex] > value)
		{
			endsearch = tmpindex - 1;
		}
		else
		{
			startsearch = tmpindex;
			break;
		}
	}
	index = startsearch;  // index is the min index such that vec[index] >= value. 
	if (vec[index] != value)
	{
		vec.insert(vec.begin() + index, value);
	}
	TIMER_HIF(TimerStop(TIMER_SETOP))
}

// Add a sorted vector vec2 to a sorted vector vec1.
template <typename Scalar>
void AddVec_Sort(vector<Scalar>& vec1, const vector<Scalar>& vec2)
{
	DEBUG_HIF(CallStackEntry cse("Intersect_Sort"))

	TIMER_HIF(TimerStart(TIMER_SETOP))
	vector<Scalar> vec;
	int i = 0;
	int j = 0;
	while (i < vec1.size() && j < vec2.size())
	{
		if (vec1[i] < vec2[j])
		{
			vec.push_back(vec1[i]);
			i++;
		}
		else if (vec1[i] > vec2[j])
		{
			vec.push_back(vec2[j]);
			j++;
		}
		else
		{
			vec.push_back(vec1[i]);
			i++;
			j++;
		}
	}
	while (i < vec1.size())
	{
		vec.push_back(vec1[i]);
		i++;
	}
	while (j < vec2.size())
	{
		vec.push_back(vec2[j]);
		j++;
	}
	vec1.assign(vec.begin(), vec.end());
	TIMER_HIF(TimerStop(TIMER_SETOP))
}

} // namespace HIF.