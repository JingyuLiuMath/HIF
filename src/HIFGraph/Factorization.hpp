#include "HIF.hpp"

namespace HIF {

// Factorization
template <typename Scalar>
void HIFGraph<Scalar>::Factorization(double tol)
{
	for (int tmplevel = numlevels_; tmplevel >= 1; tmplevel--)
	{
		// Sparse elimination.
		RecursiveSparseElim(tmplevel);

		// Skeletonization.
		RecursiveSkel(tmplevel, tol);

		// Merge.
		RecursiveMerge(tmplevel - 1);
	}

	// Root factorization.
	RootFactorization();
}

} // namespace HIF.