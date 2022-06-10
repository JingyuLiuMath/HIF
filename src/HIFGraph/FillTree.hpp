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
		

		// DebugLiu
		/*std::cout << "intr" << std::endl;
		ShowVector(intr_);
		std::cout << "sep" << std::endl;
		ShowVector(sep_);
		std::cout << "nb" << std::endl;
		ShowVector(nb_);*/

		El::Zeros(AII_, intr_.size(), intr_.size());
		Aneed = A(intr_, intr_);
		if ((Aneed.Height() > 0) && (Aneed.Width()> 0))
		{
			Aneed.QueueUpdate(Aneed.Height() - 1, Aneed.Width() - 1, Scalar(0));
			Aneed.ProcessQueues();
		}
		FullMat(Aneed, AII_);
		Aneed.Empty();
		
		El::Zeros(ASI_, sep_.size(), intr_.size());
		Aneed = A(sep_, intr_);
		if ((Aneed.Height() > 0) && (Aneed.Width() > 0))
		{
			Aneed.QueueUpdate(Aneed.Height() - 1, Aneed.Width() - 1, Scalar(0));
			Aneed.ProcessQueues();
		}
		FullMat(Aneed, ASI_);
		Aneed.Empty();

		El::Zeros(ASS_, sep_.size(), sep_.size());
		Aneed = A(sep_, sep_);
		if ((Aneed.Height() > 0) && (Aneed.Width() > 0))
		{
			Aneed.QueueUpdate(Aneed.Height() - 1, Aneed.Width() - 1, Scalar(0));
			Aneed.ProcessQueues();
		}
		FullMat(Aneed, ASS_);
		Aneed.Empty();
		
		El::Zeros(ANS_, nb_.size(), sep_.size());
		Aneed = A(nb_, sep_);
		if ((Aneed.Height() > 0) && (Aneed.Width() > 0))
		{
			Aneed.QueueUpdate(Aneed.Height() - 1, Aneed.Width() - 1, Scalar(0));
			Aneed.ProcessQueues();
		}
		FullMat(Aneed, ANS_);
		Aneed.Empty();
		
		// DebugLiu
		/*std::cout << "AII" << std::endl;
		ShowMatrix(AII_);
		std::cout << "ASI" << std::endl;
		ShowMatrix(ASI_);
		std::cout << "ASS" << std::endl;
		ShowMatrix(ASS_);
		std::cout << "ANS" << std::endl;
		ShowMatrix(ANS_);*/

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