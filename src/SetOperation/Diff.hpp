#include "HIF.hpp"

namespace HIF {

// vec = setdiff(vec1, vec2) where vec1 and vec2 are two sorted vectors. vec is also sorted.
template <typename Scalar>
void Diff_Sort(const vector<Scalar>& vec1, const vector<Scalar>& vec2, vector<Scalar>& vec)
{
	DEBUG_HIF(CallStackEntry cse("Diff_Sort"))

	TIMER_HIF(TimerStart(TIMER_SETOP))
	int i = 0;
	int j = 0;
	int startj = -1;
	int endj = -1;
	int tmpindex = -1;
	while (i < vec1.size() && j < vec2.size())
	{
		if (vec1[i] < vec2[j])
		{
			vec.push_back(vec1[i]);
			i++;
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
			while (startj <= endj)
			{
				tmpindex = startj + (endj - startj) / 2;
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
			i++;
			j++;
		}
	}
	while (i < vec1.size())
	{
		vec.push_back(vec1[i]);
		i++;
	}
	TIMER_HIF(TimerStop(TIMER_SETOP))£»
}

} // namespace HIF.
