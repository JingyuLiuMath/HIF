#include "HIF.hpp"

namespace HIF {

// Find the first index of value in vec. If no index is found, return -1. 
template <typename Scalar>
int FindFirstIndex(const vector<Scalar>& vec, const Scalar value)
{
	int index = -1;
	for (int i = 0; i < vec.size(); i++)
	{
		if (vec[i] == value)
		{
			index = i;
			break;
		}
	}
	return index;
}

// vec[index] = value.
template <typename Scalar>
void FindAllIndex(const vector<Scalar>& vec, const Scalar value, vector<int>& index)
{
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
}

// vec2[index] = vec1 where vec1 and vec2 are two sorted vectors. Here we assume vec1 is a subset of vec2.
template <typename Scalar>
void FindAllIndex_Sort(const vector<Scalar>& vec1, const vector<Scalar>& vec2, vector<int>& index)
{
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
}

} // namespace HIF.