#include "HIF.hpp"

namespace HIF {

// Add a sorted vector vec2 to a sorted vector vec1.
template <typename Scalar>
void AddVec_Sort(vector<Scalar>& vec1, const vector<Scalar>& vec2)
{
	DEBUG_HIF(CallStackEntry cse("AddVec_Sort"))

	TIMER_HIF(TimerStart(TIMER_SETOP))
	/*vector<Scalar> vec;
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
	vec1.assign(vec.begin(), vec.end());*/

	vector<Scalar> vec(vec1.size() + vec2.size());
	std::merge(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), vec.begin());
	vec1.assign(vec.begin(), vec.end());
	TIMER_HIF(TimerStop(TIMER_SETOP))
}

} // namespace HIF.