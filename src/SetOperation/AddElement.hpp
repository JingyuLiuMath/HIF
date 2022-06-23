#include "HIF.hpp"

namespace HIF
{

// Add an element to a sorted vector vec.
template <typename Scalar>
void AddElement_Sort(vector<Scalar> vec, Scalar value)
{
	DEBUG_HIF(CallStackEntry cse("Intersect_Sort"))

	TIMER_HIF(TimerStart(TIMER_SETOP))
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
	while (startsearch < endsearch)
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

}