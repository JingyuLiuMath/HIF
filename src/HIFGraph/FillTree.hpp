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
		SparseMatrixS Aneed;
		
		/*El::Zeros(AII_, intr_.size(), intr_.size());
		Aneed = A(intr_, intr_);
		if ((Aneed.Height() > 0) && (Aneed.Width()> 0))
		{
			Aneed.QueueUpdate(Aneed.Height() - 1, Aneed.Width() - 1, Scalar(0));
			Aneed.ProcessQueues();
		}
		FullMat(Aneed, AII_);*/
		FullMat(A, intr_, intr_, AII_);

		/*El::Zeros(ASI_, sep_.size(), intr_.size());
		Aneed = A(sep_, intr_);
		if ((Aneed.Height() > 0) && (Aneed.Width() > 0))
		{
			Aneed.QueueUpdate(Aneed.Height() - 1, Aneed.Width() - 1, Scalar(0));
			Aneed.ProcessQueues();
		}
		FullMat(Aneed, ASI_);*/
		FullMat(A, sep_, intr_, ASI_);

		/*El::Zeros(ASS_, sep_.size(), sep_.size());
		Aneed = A(sep_, sep_);
		if ((Aneed.Height() > 0) && (Aneed.Width() > 0))
		{
			Aneed.QueueUpdate(Aneed.Height() - 1, Aneed.Width() - 1, Scalar(0));
			Aneed.ProcessQueues();
		}
		FullMat(Aneed, ASS_);*/
		FullMat(A, sep_, sep_, ASS_);

		/*El::Zeros(ANS_, nb_.size(), sep_.size());
		Aneed = A(nb_, sep_);
		if ((Aneed.Height() > 0) && (Aneed.Width() > 0))
		{
			Aneed.QueueUpdate(Aneed.Height() - 1, Aneed.Width() - 1, Scalar(0));
			Aneed.ProcessQueues();
		}
		FullMat(Aneed, ANS_);*/
		FullMat(A, nb_, sep_, ANS_);

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
