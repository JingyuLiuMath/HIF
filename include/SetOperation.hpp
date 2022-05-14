// SetDiff
// intersect
namespace HIF {

// Find the first index of value in vec. If no index is found, return -1. 
template <typename Scalar>
int FindFirstIndex(vector<Scalar>& vec, Scalar value)
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

// Find all nonzero index in vec. If no index is found, return an empty vector.
template <typename Scalar>
void FindAllNonzeroIndex(vector<Scalar>& vec, vector<int>& index)
{
	index.clear();
	for (int i = 0; i < vec.size(); i++)
	{
		if (vec[i] != 0)
		{
			index.push_back(i);
		}
	}
}

} // namespace HIF.