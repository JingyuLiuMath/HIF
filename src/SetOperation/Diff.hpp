#include "HIF.hpp"

namespace HIF {

// vec = setdiff(vec1, vec2) where vec1 and vec2 are two sorted vectors. vec is also sorted.
template <typename Scalar>
void Diff_Sort(const vector<Scalar>& vec1, const vector<Scalar>& vec2, vector<Scalar>& vec)
{
	vector<int> index;
	Intersect_Sort(vec1, vec2, index, 1); // vec1[index] == intersect(vec1, vec2).
	int i = 0;
	int j = 0;
	while (j < vec1.size())
	{
		if (i < index.size())
		{
			if (j == index[i])
			{
				j++;
				i++;
			}
			else if (j < index[i])
			{
				vec.push_pack(vec1[j]);
				j++;
			}
			else
			{
				i++;
			}
		}
		else
		{
			vec.push_pack(vec1[j]);
			j++;
		}

	}
}

} // namespace HIF.