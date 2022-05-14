#include "HIF.hpp"

namespace HIF {

template <typename Scalar>
void HIFGraph<Scalar>::SetSeparatorType()
{
	vector<int> ordersep(sep_.size(), 0);
	for (int k = 0; k < nbnode_.size(); k++)
	{
		HIFGraph* nodek = nbnode_[k];
		vector<int> tmp1;
		// Intersect_Sort(sep_, nodek->nb_, tmp1, tmp2);
		// [~, tmp, ~] = intersect(obj.sep, nodek.nb);
		// obj.singlesep{k} = tmp;
		singlesep_.push_back(tmp1);
		for (int i = 0; i < tmp1.size(); i++)
		{
			ordersep[tmp1[i]] += 1;
		}
	}

	for (int k = 0; k < nbnode_.size(); k++)
	{
		//  obj.singlesep{k} = obj.sep(intersect(find(ordersep == 1),obj.singlesep{k}));
	}

	//  obj.complexsep = obj.sep(ordersep > 1);
}

} // namespace HIF