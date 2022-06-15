#include "HIF.hpp"

namespace HIF {

// vec = unique(vec) where vec is a sorted vector.
template <typename Scalar>
void Unique_Sort(vector<Scalar>& vec)
{
	DEBUG_HIF(CallStackEntry cse("Unique_Sort"))

	if (vec.size() == 0)
	{
		return;
	}
	vector<Scalar> veccopy(vec.size());
	veccopy[0] = vec[0];
	int actualsize_veccopy = 1;
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
			veccopy[actualsize_veccopy] = vec[i];
			actualsize_veccopy++;
			i++;
			j++;
		}
	}
	veccopy.erase(veccopy.begin() + actualsize_veccopy, veccopy.end());
	vec.clear();
	vec.assign(veccopy.begin(), veccopy.end());
}

} // namespace HIF.
