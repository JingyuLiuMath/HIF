#include "HIF.hpp"

namespace HIF {

// Fill the tree structure with A. 
template <typename Scalar>
void HIFGraph<Scalar>::FillTree(const SparseMatrix<Scalar>& A)
{
	// We only fill the leaf nodes.
	if (endflag_ == 1)
	{	
		std::cout << " level " << level_ << std::endl;
		std::cout << " seqnum " << seqnum_ << std::endl;

		for (int i = 0; i < sep_.size(); i++)
		{
			std::cout << " sep_[i] " << sep_[i] << std::endl;
		}
		for (int i = 0; i < nb_.size(); i++)
		{
			std::cout << " nb_[i] " << nb_[i] << std::endl;
		}

		Diff_Sort(vtx_, sep_, intr_);

		for (int i = 0; i < intr_.size(); i++)
		{
			std::cout << " intr_[i] " << intr_[i] << std::endl;
		}
		

		SparseMatrixS Aneed;
		
		std::cout << "Jyliu 1" << std::endl;
		
		El::Zeros(AII_, intr_.size(), intr_.size());
		Aneed = A(intr_, intr_);
		Aneed.QueueUpdate(Aneed.Height() - 1, Aneed.Width() - 1, Scalar(0));
		Aneed.ProcessQueues();
		FullMat(Aneed, AII_);
		Aneed.Empty();

		std::cout << "Jyliu 2" << std::endl;
		
		El::Zeros(ASI_, sep_.size(), intr_.size());
		Aneed = A(sep_, intr_);
		Aneed.QueueUpdate(Aneed.Height() - 1, Aneed.Width() - 1, Scalar(0));
		Aneed.ProcessQueues();
		FullMat(Aneed, ASI_);
		Aneed.Empty();

		std::cout << "Jyliu 3" << std::endl;

		El::Zeros(ASS_, sep_.size(), sep_.size());
		Aneed = A(sep_, sep_);
		Aneed.QueueUpdate(Aneed.Height() - 1, Aneed.Width() - 1, Scalar(0));
		Aneed.ProcessQueues();
		FullMat(Aneed, ASS_);
		Aneed.Empty();

		std::cout << "Jyliu 4" << std::endl;
		
		El::Zeros(ANS_, nb_.size(), sep_.size());
		Aneed = A(nb_, sep_);
		Aneed.QueueUpdate(Aneed.Height() - 1, Aneed.Width() - 1, Scalar(0));
		Aneed.ProcessQueues();
		FullMat(Aneed, ANS_);
		Aneed.Empty();

		std::cout << "Jyliu 5" << std::endl;
		
		SetSeparatorType();

		std::cout << "Jyliu 6" << std::endl;
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