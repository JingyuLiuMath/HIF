#include "HIF.hpp"

namespace HIF {

template <typename Scalar>
void HIFGraph<Scalar>::GetSolution(Matrix<Scalar>& b, const vector<int>& xcol)
{
	if (endflag_ == 0)
	{
		for (int iter = 0; iter < 2; iter++)
		{
			children_[iter]->GetSolution(b, xcol);
		}
	}
	else
	{
		SubMatrixUpdate(b, intr_, xcol, xI_);
		SubMatrixUpdate(b, sep_, xcol, xS_);
	}
}

} // namespcae HIF.