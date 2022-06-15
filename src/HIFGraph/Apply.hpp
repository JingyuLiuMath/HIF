#include "HIF.hpp"

namespace HIF {

template <typename Scalar>
void HIFGraph<Scalar>::Apply(MatrixS& b)
{
	DEBUG_HIF(CallStackEntry cse("HIFGraph:Apply"))

	vector<int> xcol;
	RangeVec(0, b.Width(), xcol);
	
	// Fill vector in the tree.
	FillVecTree(b, xcol);
	
	// ApplyUp.
	for (int tmplevel = numlevels_; tmplevel >= 1; tmplevel--)
	{
		RecursiveApplySparseElimUp(tmplevel);
		RecursiveApplySkelUp(tmplevel, xcol);
		RecursiveApplyMerge(tmplevel - 1, xcol);
	}
	
	// Root apply.
	RootApply();

	// ApplyDown.
	for (int tmplevel = 1; tmplevel <= numlevels_; tmplevel++)
	{
		RecursiveApplySplit(tmplevel - 1, xcol);
		RecursiveApplySkelDown(tmplevel, xcol);
		RecursiveApplySparseElimDown(tmplevel);
	}
	
	// GetSolution.
	GetSolution(b, xcol);
}

} // namespace HIF.
