#include "HIF.hpp"

namespace HIF {

template <typename Scalar>
void HIFGraph<Scalar>::RootFactorization()
{
	std::cout << "RootFactorization" << std::endl;
	LDLSolve(AII_, AIIinv_);
	// AII = 0.
	AII_.Empty();
}

} // namespace HIF.