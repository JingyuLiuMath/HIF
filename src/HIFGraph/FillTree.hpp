#include "HIF.hpp"

namespace HIF {

// Fill the tree structure with A. 
template <typename Scalar>
void HIFGraph<Scalar>::FillTree(const SparseMatrix<Scalar>& A)
{
	// We only fill the leaf nodes.
	if (endflag_ == 1£©
	{
		Diff_Sort(vtx_,sep_,intr_);
		
		El::Zeros(AII_, intr_.size(), intr_.size);
		El::Zeros(ASI_, sep_.size(), intr_.size);
		El::Zeros(ASS_, sep_.size(), sep_.size);
		El::Zeros(ANS_, nb_.size(), sep_.size);

		AII_ = A(intr_, intr_);
		ASI_ = A(sep_, intr_);
		ASS_ = A(sep_, sep_);
		ANS_ = A(nb_, sep_);

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

}