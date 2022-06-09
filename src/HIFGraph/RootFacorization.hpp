#include "HIF.hpp"

namespace HIF {

template <typename Scalar>
void HIFGraph<Scalar>::RootFactorization()
{
	std::cout << "RootFactorization" << std::endl;
	LDLSolve(AII_, AIIinv_);
}

} // namespace HIF.