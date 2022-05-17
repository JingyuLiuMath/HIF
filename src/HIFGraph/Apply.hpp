#include "HIF.hpp"

namespace HIF {

template <typename Scalar>
void HIFGraph<Scalar>::Apply(Matrix<Scalar>& b)
{
	// Fill vector in the tree.
	FillVecTree(b);

	for (int tmplevel = numlevels_; tmplevel >= 1; tmplevel--)
	{
		RecursiveApplySparseElimUp(tmplevel);
		RecursiveApplySkelUp(tmplevel);
		RecursiveApplyMerge(tmplevel - 1);
	}

	// Root apply.
	RootApply();

	for (int tmplevel = 1; tmplevel <= numlevels_; tmplevel++)
	{
		RecursiveApplySplit(tmplevel - 1);
		RecursiveApplySparseElimDown(tmplevel);
		RecursiveApplySkelDown(tmplevel);
	}
	
	GetSolution(b);

}

} // namespace HIF.