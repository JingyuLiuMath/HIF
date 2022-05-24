#include "HIF.hpp"

namespace HIF {

// vec = unique(vec) where vec is a sorted vector.
template <typename Scalar>
void Unique_Sort(vector<Scalar>& vec)
{
	if (vec.size() == 0)
	{
		return;
	}
	vector<Scalar> veccopy;
	veccopy.push_back(vec[0]);
	int i = 1; // index of vec.
	int j = 0; // index of veccopy.
	while (i < vec.size())
	{
		if (vec[i] == veccopy[j])
		{
			i++;
		}
		else
		{
			veccopy.push_back(vec[i]);
			i++;
			j++;
		}
	}
	vec.clear();
	vec.assign(veccopy.begin(), veccopy.end());
}

} // namespace HIF.