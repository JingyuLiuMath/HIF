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
			index1.push_back(i);
			index2.push_back(j);
			i++;
			j++;
		}
	}
}

// who == 1: vec1[index] = intersect(vec1, vec2) where vec1 and vec2 are two sorted vectors.
// who == 2: vec2[index] = intersect(vec1, vec2) where vec1 and vec2 are two sorted vectors.
template <typename Scalar>
void Intersect_Sort(const vector<Scalar>& vec1, const vector<Scalar>& vec2,
	vector<int>& index, int who)
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
			if (who == 1)
			{
				index.push_back(i);
			}
			else if(who == 2)
			{
				index.push_back(j);
			}
			i++;
			j++;
		}
	}
}

// vec = intersect(vec1, vec2) where vec1 and vec2 are two sorted vectors.
template <typename Scalar>
void Intersect_Sort(const vector<Scalar>& vec1, const vector<Scalar>& vec2, vector<Scalar>& vec)
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
			vec.push_back(vec1[i]);
			i++;
			j++;
		}
	}
}

// vec = intersect(vec1, vec2) where vec1 and vec2 are two sorted vectors. 
// who == 1: vec1[index] = intersect(vec1, vec2) where vec1 and vec2 are two sorted vectors.
// who == 2: vec2[index] = intersect(vec1, vec2) where vec1 and vec2 are two sorted vectors.
template <typename Scalar>
void Intersect_Sort(const vector<Scalar>& vec1, const vector<Scalar>& vec2,
	vector<Scalar>& vec, vector<int>& index, int who)
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
			vec.push_back(vec1[i]);
			if (who == 1)
			{
				index.push_back(i);
			}
			else if (who == 2)
			{
				index.push_back(j);
			}
			i++;
			j++;
		}
	}
}

} // namespace HIF.
