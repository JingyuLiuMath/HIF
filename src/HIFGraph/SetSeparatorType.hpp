#include "HIF.hpp"

namespace HIF {

// Set sepearator vertices' type.
template <typename Scalar>
void HIFGraph<Scalar>::SetSeparatorType()
{
	singlesep_.resize(nbnode_.size());

	vector<int> ordersep(sep_.size(), 0);
	
	for (int k = 0; k < nbnode_.size(); k++)
	{
		HIFGraph* nodek = nbnode_[k];
		vector<int> tmpindex;
		Intersect_Sort(sep_, nodek->nb_, tmpindex, 1);
		singlesep_[k] = tmpindex;
		for (int i = 0; i < tmpindex.size(); i++)
		{
			ordersep[tmpindex[i]] += 1;
		}
	}

	for (int k = 0; k < nbnode_.size(); k++)
	{
		vector<int> index_find_ordersep_1;
		FindAllIndex(ordersep, 1, index_find_ordersep_1);
		vector<int> index_sep;
		Intersect_Sort(index_find_ordersep_1, singlesep_[k], index_sep);
		singlesep_[k].clear();
		singlesep_[k].resize(index_sep.size());
		for (int i = 0; i < index_sep.size(); i++)
		{
			singlesep_[k][i] = sep_[index_sep[i]];
		}
	}

	complexsep_.resize(sep_.size());
	int actual_complexsep = 0;
	for (int i = 0; i < sep_.size(); i++)
	{
		if (ordersep[i] > 1)
		{
			complexsep_[actual_complexsep] = sep_[i];
			actual_complexsep++;
		}
	}
	complexsep_.erase(complexsep_.begin() + actual_complexsep, complexsep_.end());
}

} // namespace HIF.