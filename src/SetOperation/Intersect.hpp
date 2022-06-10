#include "HIF.hpp"

namespace HIF {

// Return true if the intersection of two sorted vectors vec1 and vec2 is not empty.
template <typename Scalar>
bool Intersect_Sort(const vector<Scalar>& vec1, const vector<Scalar>& vec2)
{
	int i = 0;
	int j = 0;
	while (i < vec1.size() && j < vec2.size())
	{
		if (vec1[i] < vec2[j])
		{
			i++;
		}
		else if (vec1[i] > vec2[j])
		{
			j++;
		}
		else
		{
			return true;
			i++;
			j++;
		}
	}
	return false;
}

// vec1[index1] = vec2[index2] = intersect(vec1, vec2) where vec1 and vec2 are two sorted vectors.
template <typename Scalar>
void Intersect_Sort(const vector<Scalar>& vec1, const vector<Scalar>& vec2,
	vector<int>& index1, vector<int>& index2)
{
	index1.resize(vec1.size());
	index2.resize(vec2.size());
	int actualsize_index1 = 0;
	int actualsize_index2 = 0;
	int i = 0;
	int j = 0;
	while (i < vec1.size() && j < vec2.size())
	{
		if (vec1[i] < vec2[j])
		{
			i++;
		}
		else if (vec1[i] > vec2[j])
		{
			j++;
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
}

// who == 1: vec1[index] = intersect(vec1, vec2) where vec1 and vec2 are two sorted vectors.
// who == 2: vec2[index] = intersect(vec1, vec2) where vec1 and vec2 are two sorted vectors.
template <typename Scalar>
void Intersect_Sort(const vector<Scalar>& vec1, const vector<Scalar>& vec2,
	vector<int>& index, int who)
{
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
	while (i < vec1.size() && j < vec2.size())
	{
		if (vec1[i] < vec2[j])
		{
			i++;
		}
		else if (vec1[i] > vec2[j])
		{
			j++;
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
}

// vec = intersect(vec1, vec2) where vec1 and vec2 are two sorted vectors.
template <typename Scalar>
void Intersect_Sort(const vector<Scalar>& vec1, const vector<Scalar>& vec2, vector<Scalar>& vec)
{
	vec.resize(vec1.size());
	int actualsize_vec = 0;
	int i = 0;
	int j = 0;
	while (i < vec1.size() && j < vec2.size())
	{
		if (vec1[i] < vec2[j])
		{
			i++;
		}
		else if (vec1[i] > vec2[j])
		{
			j++;
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
}

// vec = intersect(vec1, vec2) where vec1 and vec2 are two sorted vectors. 
// who == 1: vec1[index] = intersect(vec1, vec2) where vec1 and vec2 are two sorted vectors.
// who == 2: vec2[index] = intersect(vec1, vec2) where vec1 and vec2 are two sorted vectors.
template <typename Scalar>
void Intersect_Sort(const vector<Scalar>& vec1, const vector<Scalar>& vec2,
	vector<Scalar>& vec, vector<int>& index, int who)
{
	vec.resize(vec1.size());
	index.resize(vec1.size());
	int actualsize_vec_index = 0;
	int i = 0;
	int j = 0;
	while (i < vec1.size() && j < vec2.size())
	{
		if (vec1[i] < vec2[j])
		{
			i++;
		}
		else if (vec1[i] > vec2[j])
		{
			j++;
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
}

} // namespace HIF.
