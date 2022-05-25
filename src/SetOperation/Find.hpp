#include "HIF.hpp"

namespace HIF {

// Find the first index of value in vec. If no index is found, return -1. 
template <typename Scalar>
int FindFirstIndex(const vector<Scalar>& vec, Scalar value)
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
void FindAllIndex(const vector<Scalar>& vec, Scalar value, vector<int>& index)
{
	for (int i = 0; i < vec.size(); i++)
	{
		if (vec[i] == value)
		{
			index.push_back(i);
		}
	}
}

// vec2[index] = vec1 where vec1 and vec2 are two sorted vectors. Here we assume vec1 is a subset of vec2.
template <typename Scalar>
void FindAllIndex_Sort(const vector<Scalar>& vec1, const vector<Scalar>& vec2, vector<int>& index)
{
	int i = 0;
	int j = 0;
	while (i < vec1.size())
	{
		if (vec1[i] == vec2[j])
		{
			index.push_back(j);
			i++;
			j++;
		}
		else
		{
			j++;
		}
	}
}

// A[rowi, colj] != 0.
template <typename Scalar>
FindAllNonzeroIndexMat(const SparseMatrix<Scalar>& A, vector<int>& row, vector<int>& col)
{
	for (int i = 0; i < A.Height(); i++)
	{
		for (int j = 0; j < A.Width(); j++)
		{
			if (A.Get(i, j) != 0)
			{
				row.push_back(i);
				col.push_back(j);
			}
		}
	}
}

// vec1[index] = vec2[index] where vec1 and vec2 are two vectors of the same length.
template <typename Scalar>
FindEqualIndex(const vector<Scalar>& vec1, const vector<Scalar>& vec2, vector<int>& index)
{
	for (int i = 0; i < vec1.size(); i++)
	{
		if (vec1[i] == vec2[i])
		{
			index.push_back[i];
		}
	}
}

} // namespace HIF.