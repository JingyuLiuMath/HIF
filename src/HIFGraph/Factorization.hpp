#include "HIF.hpp"

namespace HIF {

// Factorization
template <typename Scalar>
void HIFGraph<Scalar>::Factorization(int HIFbutton, double tol)
{
	std::cout << "Start factorization" << std::endl;
	for (int tmplevel = numlevels_; tmplevel >= 1; tmplevel--)
	{
		// Sparse elimination.
		RecursiveSparseElim(tmplevel);

		// Skeletonization.
		// RecursiveSkel(tmplevel, tol);
		RecursiveSkel(tmplevel, HIFbutton, tol);
		// Merge.
		RecursiveMerge(tmplevel - 1);
	}

	// Root factorization.
	RootFactorization();

	std::cout << "Finish factorization" << std::endl;
}

} // namespace HIF.