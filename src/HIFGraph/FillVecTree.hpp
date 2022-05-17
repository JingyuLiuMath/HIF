#include "HIF.hpp"

namespace HIF {

// Fill the tree structure with b.
template <typename Scalar>
void HIFGraph<Scalar>::FillVecTree(const Matrix<Scalar>& b)
{
	if (endflag_ == 0)
	{
		for (int iter = 0; iter < 2; iter++)
		{
			children_[iter]->FillVecTree(b);
		}
	}
	else
	{
		xI_(b(intr_, b.Width());
		xS_(b(sep_, b.Width());
	}
}

} // namespace HIF.