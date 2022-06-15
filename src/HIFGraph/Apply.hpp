#include "HIF.hpp"

namespace HIF {

template <typename Scalar>
void HIFGraph<Scalar>::Apply(Matrix<Scalar>& b)
{
	DEBUG_HIF(CallStackEntry cse("HIFGraph:Apply"))

	std::cout << "Start apply" << std::endl;
	vector<int> xcol;
	RangeVec(0, b.Width(), xcol);
	
	// Fill vector in the tree.
	FillVecTree(b, xcol);
	
	for (int tmplevel = numlevels_; tmplevel >= 1; tmplevel--)
	{
		RecursiveApplySparseElimUp(tmplevel);
		RecursiveApplySkelUp(tmplevel, xcol);
		RecursiveApplyMerge(tmplevel - 1, xcol);
	}
	
	// Root apply.
	RootApply();

	for (int tmplevel = 1; tmplevel <= numlevels_; tmplevel++)
	{
		RecursiveApplySplit(tmplevel - 1, xcol);
		RecursiveApplySkelDown(tmplevel, xcol);
		RecursiveApplySparseElimDown(tmplevel);
	}
	
	GetSolution(b, xcol);

	std::cout << "Finish apply" << std::endl;
}

} // namespace HIF.

