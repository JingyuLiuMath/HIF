#include "HIF.hpp"

namespace HIF {

// vec = setdiff(vec1, vec2) where vec1 and vec2 are two sorted vectors. vec is also sorted.
template <typename Scalar>
void Diff_Sort(const vector<Scalar>& vec1, const vector<Scalar>& vec2, vector<Scalar>& vec)
{
	DEBUG_HIF(CallStackEntry cse("Diff_Sort"))

	vec.resize(vec1.size());
	int i = 0;
	int j = 0;
	int actualsize_vec = 0;
	while (i < vec1.size() && j < vec2.size())
	{
		if (vec1[i] < vec2[j])
		{
			vec[actualsize_vec] = vec1[i];
			actualsize_vec++;
			i++;
		}
		else if (vec1[i] > vec2[j])
		{
			j++;
		}
		else
		{
			i++;
			j++;
		}
	}
	while (i < vec1.size())
	{
		vec[actualsize_vec] = vec1[i];
		actualsize_vec++;
		i++;
	}
	vec.erase(vec.begin() + actualsize_vec, vec.end());
}

} // namespace HIF.
