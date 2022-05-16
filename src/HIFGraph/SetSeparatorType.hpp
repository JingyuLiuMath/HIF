#include "HIF.hpp"

namespace HIF {

// Set sepearator vertices' type.
template <typename Scalar>
void HIFGraph<Scalar>::SetSeparatorType()
{
	vector<int> ordersep(sep_.size(), 0);

	for (int k = 0; k < nbnode_.size(); k++)
	{
		HIFGraph* nodek = nbnode_[k];
		vector<int> tmp;
		Intersect_Sort(sep_, nodek->nb_, tmp, 1);
		singlesep_.push_back(tmp);
		for (int i = 0; i < tmp.size(); i++)
		{
			ordersep[tmp[i]] += 1;
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
	
	for (int i = 0; i < sep_.size(); i++)
	{
		if (ordersep[i] > 1)
		{
			complexsep_.push_back(sep_[i]);
		}
	}
}

} // namespace HIF.