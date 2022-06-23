#include "HIF.hpp"

namespace HIF {

// Find the first index of value in vec. If no index is found, return -1. 
template <typename Scalar>
int FindFirstIndex(const vector<Scalar>& vec, const Scalar value)
{
	DEBUG_HIF(CallStackEntry cse("FindFirstIndex"))

	TIMER_HIF(TimerStart(TIMER_SETOP))
	int index = -1;
	for (int i = 0; i < vec.size(); i++)
	{
		if (vec[i] == value)
		{
			index = i;
			break;
		}
	}
	TIMER_HIF(TimerStop(TIMER_SETOP))
	return index;
}


// Find the index of value in vec. If no index is found, return -1. 
template <typename Scalar>
int FindIndex_Sort(const vector<Scalar>& vec, const Scalar value)
{
	DEBUG_HIF(CallStackEntry cse("FindFirstIndex"))
	
	TIMER_HIF(TimerStart(TIMER_SETOP))
	int startsearch = 0;
	int endsearch = vec.size() - 1;
	int tmpindex = -1;
	int index = -1;

	if (vec.front() > value)
	{
		TIMER_HIF(TimerStop(TIMER_SETOP))
		return index;
	}
	if (vec.back() < value)
	{
		TIMER_HIF(TimerStop(TIMER_SETOP))
		return index;
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
		TIMER_HIF(TimerStop(TIMER_SETOP))
		return -1;
	}

	TIMER_HIF(TimerStop(TIMER_SETOP))
	return index;
}

// vec[index] = value.
template <typename Scalar>
void FindAllIndex(const vector<Scalar>& vec, const Scalar value, vector<int>& index)
{
	DEBUG_HIF(CallStackEntry cse("FindAllIndex"))

	TIMER_HIF(TimerStart(TIMER_SETOP))

	index.resize(vec.size());
	int actualsize_index = 0;
	for (int i = 0; i < vec.size(); i++)
	{
		if (vec[i] == value)
		{
			index[actualsize_index] = i;
			actualsize_index++;
		}
	}
	index.erase(index.begin() + actualsize_index, index.end());

	TIMER_HIF(TimerStop(TIMER_SETOP))
}

// vec2[index] = vec1 where vec1 and vec2 are two sorted vectors. Here we assume vec1 is a subset of vec2.
template <typename Scalar>
void FindAllIndex_Sort(const vector<Scalar>& vec1, const vector<Scalar>& vec2, vector<int>& index)
{
	DEBUG_HIF(CallStackEntry cse("FindAllIndex_Sort"))

	TIMER_HIF(TimerStart(TIMER_SETOP))
	index.resize(vec1.size());
	int actualsize_index = 0;
	int i = 0;
	int j = 0;
	while (i < vec1.size())
	{
		if (vec1[i] == vec2[j])
		{
			index[actualsize_index] = j;
			actualsize_index++;
			i++;
			j++;
		}
		else
		{
			j++;
		}
	}
	index.erase(index.begin() + actualsize_index, index.end());
	TIMER_HIF(TimerStop(TIMER_SETOP))
}

} // namespace HIF.
