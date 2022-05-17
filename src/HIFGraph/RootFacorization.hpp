#include "HIF.hpp"

namespace HIF {

template <typename Scalar>
void HIFGraph<Scalar>::RootFactorization()
{
	LDLSolve(AII_, AIIinv_);
}

} // namespace HIF.