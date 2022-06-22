#include "HIF.hpp"

namespace HIF {

// Return true if the intersection of two sorted vectors vec1 and vec2 is not empty.
template <typename Scalar>
bool Intersect_Sort(const vector<Scalar>& vec1, const vector<Scalar>& vec2)
{
	DEBUG_HIF(CallStackEntry cse("Intersect_Sort"))

	TIMER_HIF(TimerStart(TIMER_SETOP))

	bool result = false;
	int i = 0;
	int j = 0;
	int starti = -1;
	int endi = -1;
	int startj = -1;
	int endj = -1;
	int tmpindex = -1;
	while (i < vec1.size() && j < vec2.size())
	{
		if (vec1[i] < vec2[j])
		{
			if (vec1.back() < vec2[j])
			{
				break;
			}
			// Find i where i is the min index such that vec1[i] >= vec2[j].
			starti = i + 1;
			endi = vec1.size() - 1;
			while (starti < endi)
			{
				tmpindex = (starti + endi) / 2;
				if (vec1[tmpindex] < vec2[j])
				{
					starti = tmpindex + 1;
				}
				else if (vec1[tmpindex] > vec2[j])
				{
					endi = tmpindex - 1;
				}
				else
				{
					starti = tmpindex;
					break;
				}
			}
			i = starti;
		}
		else if (vec1[i] > vec2[j])
		{
			if (vec1[i] > vec2.back())
			{
				break;
			}
			// Find j where j is the min index such that vec2[j] >= vec1[i].
			startj = j + 1;
			endj = vec2.size() - 1;
			while (startj < endj)
			{
				tmpindex = (startj + endj) / 2;
				if (vec2[tmpindex] < vec1[i])
				{
					startj = tmpindex + 1;
				}
				else if (vec2[tmpindex] > vec1[i])
				{
					endj = tmpindex - 1;
				}
				else
				{
					startj = tmpindex;
					break;
				}
			}
			j = startj;
		}
		else
		{
			result = true;
			break;
			i++;
			j++;
		}
	}

	TIMER_HIF(TimerStop(TIMER_SETOP))

	return result;
}

// vec1[index1] = vec2[index2] = intersect(vec1, vec2) where vec1 and vec2 are two sorted vectors.
template <typename Scalar>
void Intersect_Sort(const vector<Scalar>& vec1, const vector<Scalar>& vec2,
	vector<int>& index1, vector<int>& index2)
{
	DEBUG_HIF(CallStackEntry cse("Intersect_Sort"))

	TIMER_HIF(TimerStart(TIMER_SETOP))

	index1.resize(vec1.size());
	index2.resize(vec2.size());
	int actualsize_index1 = 0;
	int actualsize_index2 = 0;
	int i = 0;
	int j = 0;
	int starti = -1;
	int endi = -1;
	int startj = -1;
	int endj = -1;
	int tmpindex = -1;
	while (i < vec1.size() && j < vec2.size())
	{
		if (vec1[i] < vec2[j])
		{
			if (vec1.back() < vec2[j])
			{
				break;
			}
			// Find i where i is the min index such that vec1[i] >= vec2[j].
			starti = i + 1;
			endi = vec1.size() - 1;
			while (starti < endi)
			{
				tmpindex = (starti + endi) / 2;
				if (vec1[tmpindex] < vec2[j])
				{
					starti = tmpindex + 1;
				}
				else if (vec1[tmpindex] > vec2[j])
				{
					endi = tmpindex - 1;
				}
				else
				{
					starti = tmpindex;
					break;
				}
			}
			i = starti;
		}
		else if (vec1[i] > vec2[j])
		{
			if (vec1[i] > vec2.back())
			{
				break;
			}
			// Find j where j is the min index such that vec2[j] >= vec1[i].
			startj = j + 1;
			endj = vec2.size() - 1;
			while (startj < endj)
			{
				tmpindex = (startj + endj) / 2;
				if (vec2[tmpindex] < vec1[i])
				{
					startj = tmpindex + 1;
				}
				else if (vec2[tmpindex] > vec1[i])
				{
					endj = tmpindex - 1;
				}
				else
				{
					startj = tmpindex;
					break;
				}
			}
			j = startj;
		}
		else
		{
			index1[actualsize_index1] = i;
			actualsize_index1++;
			index2[actualsize_index2] = j;
			actualsize_index2++;
			i++;
			j++;
		}
	}
	index1.erase(index1.begin() + actualsize_index1, index1.end());
	index2.erase(index2.begin() + actualsize_index2, index2.end());

	TIMER_HIF(TimerStop(TIMER_SETOP))
}

// who == 1: vec1[index] = intersect(vec1, vec2) where vec1 and vec2 are two sorted vectors.
// who == 2: vec2[index] = intersect(vec1, vec2) where vec1 and vec2 are two sorted vectors.
template <typename Scalar>
void Intersect_Sort(const vector<Scalar>& vec1, const vector<Scalar>& vec2,
	vector<int>& index, int who)
{
	DEBUG_HIF(CallStackEntry cse("Intersect_Sort"))

	TIMER_HIF(TimerStart(TIMER_SETOP))

	if (who == 1)
	{
		index.resize(vec1.size());
	}
	else
	{
		index.resize(vec2.size());
	}
	int actualsize_index = 0;
	int i = 0;
	int j = 0;
	int starti = -1;
	int endi = -1;
	int startj = -1;
	int endj = -1;
	int tmpindex = -1;
	while (i < vec1.size() && j < vec2.size())
	{
		if (vec1[i] < vec2[j])
		{
			if (vec1.back() < vec2[j])
			{
				break;
			}
			// Find i where i is the min index such that vec1[i] >= vec2[j].
			starti = i + 1;
			endi = vec1.size() - 1;
			while (starti < endi)
			{
				tmpindex = (starti + endi) / 2;
				if (vec1[tmpindex] < vec2[j])
				{
					starti = tmpindex + 1;
				}
				else if (vec1[tmpindex] > vec2[j])
				{
					endi = tmpindex - 1;
				}
				else
				{
					starti = tmpindex;
					break;
				}
			}
			i = starti;
		}
		else if (vec1[i] > vec2[j])
		{
			if (vec1[i] > vec2.back())
			{
				break;
			}
			// Find j where j is the min index such that vec2[j] >= vec1[i].
			startj = j + 1;
			endj = vec2.size() - 1;
			while (startj < endj)
			{
				tmpindex = (startj + endj) / 2;
				if (vec2[tmpindex] < vec1[i])
				{
					startj = tmpindex + 1;
				}
				else if (vec2[tmpindex] > vec1[i])
				{
					endj = tmpindex - 1;
				}
				else
				{
					startj = tmpindex;
					break;
				}
			}
			j = startj;
		}
		else
		{
			if (who == 1)
			{
				index[actualsize_index] = i;
				actualsize_index++;
			}
			else if(who == 2)
			{
				index[actualsize_index] = j;
				actualsize_index++;
			}
			i++;
			j++;
		}
	}
	index.erase(index.begin() + actualsize_index, index.end());

	TIMER_HIF(TimerStop(TIMER_SETOP))
}

// vec = intersect(vec1, vec2) where vec1 and vec2 are two sorted vectors.
template <typename Scalar>
void Intersect_Sort(const vector<Scalar>& vec1, const vector<Scalar>& vec2, vector<Scalar>& vec)
{
	DEBUG_HIF(CallStackEntry cse("Intersect_Sort"))

	TIMER_HIF(TimerStart(TIMER_SETOP))

	vec.resize(vec1.size());
	int actualsize_vec = 0;
	int i = 0;
	int j = 0;
	int starti = -1;
	int endi = -1;
	int startj = -1;
	int endj = -1;
	int tmpindex = -1;
	while (i < vec1.size() && j < vec2.size())
	{
		if (vec1[i] < vec2[j])
		{
			if (vec1.back() < vec2[j])
			{
				break;
			}
			// Find i where i is the min index such that vec1[i] >= vec2[j].
			starti = i + 1;
			endi = vec1.size() - 1;
			while (starti < endi)
			{
				tmpindex = (starti + endi) / 2;
				if (vec1[tmpindex] < vec2[j])
				{
					starti = tmpindex + 1;
				}
				else if (vec1[tmpindex] > vec2[j])
				{
					endi = tmpindex - 1;
				}
				else
				{
					starti = tmpindex;
					break;
				}
			}
			i = starti;
		}
		else if (vec1[i] > vec2[j])
		{
			if (vec1[i] > vec2.back())
			{
				break;
			}
			// Find j where j is the min index such that vec2[j] >= vec1[i].
			startj = j + 1;
			endj = vec2.size() - 1;
			while (startj < endj)
			{
				tmpindex = (startj + endj) / 2;
				if (vec2[tmpindex] < vec1[i])
				{
					startj = tmpindex + 1;
				}
				else if (vec2[tmpindex] > vec1[i])
				{
					endj = tmpindex - 1;
				}
				else
				{
					startj = tmpindex;
					break;
				}
			}
			j = startj;
		}
		else
		{
			vec[actualsize_vec] = vec1[i];
			actualsize_vec++;
			i++;
			j++;
		}
	}
	vec.erase(vec.begin() + actualsize_vec, vec.end());

	TIMER_HIF(TimerStop(TIMER_SETOP))
}

// vec = intersect(vec1, vec2) where vec1 and vec2 are two sorted vectors. 
// who == 1: vec1[index] = intersect(vec1, vec2) where vec1 and vec2 are two sorted vectors.
// who == 2: vec2[index] = intersect(vec1, vec2) where vec1 and vec2 are two sorted vectors.
template <typename Scalar>
void Intersect_Sort(const vector<Scalar>& vec1, const vector<Scalar>& vec2,
	vector<Scalar>& vec, vector<int>& index, int who)
{
	DEBUG_HIF(CallStackEntry cse("Intersect_Sort"))

	TIMER_HIF(TimerStart(TIMER_SETOP))

	vec.resize(vec1.size());
	if (who == 1)
	index.resize(vec1.size());
	int actualsize_vec_index = 0;
	int i = 0;
	int j = 0;
	int starti = -1;
	int endi = -1;
	int startj = -1;
	int endj = -1;
	int tmpindex = -1;
	while (i < vec1.size() && j < vec2.size())
	{
		if (vec1[i] < vec2[j])
		{
			if (vec1.back() < vec2[j])
			{
				break;
			}
			// Find i where i is the min index such that vec1[i] >= vec2[j].
			starti = i + 1;
			endi = vec1.size() - 1;
			while (starti < endi)
			{
				tmpindex = (starti + endi) / 2;
				if (vec1[tmpindex] < vec2[j])
				{
					starti = tmpindex + 1;
				}
				else if (vec1[tmpindex] > vec2[j])
				{
					endi = tmpindex - 1;
				}
				else
				{
					starti = tmpindex;
					break;
				}
			}
			i = starti;
		}
		else if (vec1[i] > vec2[j])
		{
			if (vec1[i] > vec2.back())
			{
				break;
			}
			// Find j where j is the min index such that vec2[j] >= vec1[i].
			startj = j + 1;
			endj = vec2.size() - 1;
			while (startj < endj)
			{
				tmpindex = (startj + endj) / 2;
				if (vec2[tmpindex] < vec1[i])
				{
					startj = tmpindex + 1;
				}
				else if (vec2[tmpindex] > vec1[i])
				{
					endj = tmpindex - 1;
				}
				else
				{
					startj = tmpindex;
					break;
				}
			}
			j = startj;
		}
		else
		{
			vec[actualsize_vec_index] = vec1[i];
			if (who == 1)
			{
				index[actualsize_vec_index] = i;
			}
			else if (who == 2)
			{
				index[actualsize_vec_index] = j;
			}
			actualsize_vec_index++;
			i++;
			j++;
		}
	}
	vec.erase(vec.begin() + actualsize_vec_index, vec.end());
	index.erase(index.begin() + actualsize_vec_index, index.end());

	TIMER_HIF(TimerStop(TIMER_SETOP))
}

} // namespace HIF.
