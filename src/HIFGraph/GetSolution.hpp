#include "HIF.hpp"

namespace HIF {

template <typename Scalar>
void HIFGraph<Scalar>::GetSolution(MatrixS& b, const vector<int>& xcol)
{
	DEBUG_HIF(CallStackEntry cse("HIFGraph:GetSolution"))

	if (endflag_ == 1)
	{
		SubMatrixUpdate(b, intr_, xcol, xI_);
		SubMatrixUpdate(b, sep_, xcol, xS_);
		xI_.Empty();
		xS_.Empty();
	}
	else
	{
		xI_.Empty();
		xS_.Empty();
		for (int iter = 0; iter < 2; iter++)
		{
			childrennode_.Child(iter).GetSolution(b, xcol);
		}
	}
}

} // namespcae HIF.
