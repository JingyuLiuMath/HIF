#include "HIF.hpp"

namespace HIF {

template <typename Scalar>
void HIFGraph<Scalar>::RootFactorization()
{
	DEBUG_HIF(CallStackEntry cse("HIFGraph:RootFactorization"))

	LDLSolve(AII_, AIIinv_);
	// AII = 0.
	AII_.Empty();
}

} // namespace HIF.
