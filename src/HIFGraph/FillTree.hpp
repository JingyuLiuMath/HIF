#include "HIF.hpp"

namespace HIF {

// Fill the tree structure with A. 
template <typename Scalar>
void HIFGraph<Scalar>::FillTree(const SparseMatrix<Scalar>& A)
{
	// We only fill the leaf nodes.
	if (endflag_ == 1)
	{	
		Diff_Sort(vtx_, sep_, intr_);
		
		SparseMatrixS Aneed;
		
		El::Zeros(AII_, intr_.size(), intr_.size());
		Aneed = A(intr_, intr_);
		Aneed.QueueUpdate(Aneed.Height() - 1, Aneed.Width() - 1, Scalar(0));
		Aneed.ProcessQueues();
		FullMat(Aneed, AII_);
		Aneed.Empty();
		
		El::Zeros(ASI_, sep_.size(), intr_.size());
		Aneed = A(sep_, intr_);
		Aneed.QueueUpdate(Aneed.Height() - 1, Aneed.Width() - 1, Scalar(0));
		Aneed.ProcessQueues();
		FullMat(Aneed, ASI_);
		Aneed.Empty();

		El::Zeros(ASS_, sep_.size(), sep_.size());
		Aneed = A(sep_, sep_);
		Aneed.QueueUpdate(Aneed.Height() - 1, Aneed.Width() - 1, Scalar(0));
		Aneed.ProcessQueues();
		FullMat(Aneed, ASS_);
		Aneed.Empty();
		
		El::Zeros(ANS_, nb_.size(), sep_.size());
		Aneed = A(nb_, sep_);
		Aneed.QueueUpdate(Aneed.Height() - 1, Aneed.Width() - 1, Scalar(0));
		Aneed.ProcessQueues();
		FullMat(Aneed, ANS_);
		Aneed.Empty();
		
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