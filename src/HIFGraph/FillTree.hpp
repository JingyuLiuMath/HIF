#include "HIF.hpp"

namespace HIF {

// Fill the tree structure with A. 
template <typename Scalar>
void HIFGraph<Scalar>::FillTree(const SparseMatrixS& A)
{
	DEBUG_HIF(CallStackEntry cse("HIFGraph:FillTree"))

	// We only fill the leaf nodes.
	if (endflag_ == 1)
	{	
		Diff_Sort(vtx_, sep_, intr_);
		
		FullMat(A, intr_, intr_, AII_);

		FullMat(A, sep_, intr_, ASI_);

		FullMat(A, sep_, sep_, ASS_);

		FullMat(A, nb_, sep_, ANS_);

		ShowMatrix(AII_, "AII");

		ShowMatrix(ASI_, "ASI");

		ShowMatrix(ASS_, "ASS");

		ShowMatrix(ANS_, "ANS");

		SetSeparatorType();
	}
	else
	{
		for (int iter = 0; iter < 2; iter++)
		{
			children_[iter]->FillTree(A);
		}
	}
}

} // namespace HIF.
