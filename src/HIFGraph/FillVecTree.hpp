#include "HIF.hpp"

namespace HIF {

// Fill the tree structure with b.
template <typename Scalar>
void HIFGraph<Scalar>::FillVecTree(const MatrixS& b, const vector<int>& xcol)
{
	DEBUG_HIF(CallStackEntry cse("HIFGraph:FillVecTree"))

	if (endflag_ == 0)
	{
		for (int iter = 0; iter < 2; iter++)
		{
			children_[iter]->FillVecTree(b, xcol);
		}
	}
	else
	{
		xI_ = b(intr_, xcol);
		xS_ = b(sep_, xcol);
	}
}

} // namespace HIF.
