#include "HIF.hpp"

namespace HIF {

// Fill the tree structure with A. 
template <typename Scalar>
void HIFGraph<Scalar>::FillTree(const SparseMatrix<Scalar>& A)
{
	// We only fill the leaf nodes.
	if (endflag_ == 1£©
	{
		Diff_Sort(vtx_, sep_, intr_);
		
		/*AII_ = A(intr_, intr_);
		ASI_ = A(sep_, intr_);
		ASS_ = A(sep_, sep_);
		ANS_ = A(nb_, sep_);*/

		El::Zeros(AII_, intr_.size(), intr_.size());
		vector<int> rangeintr;
		RangeVec(0, intr_.size(), rangeintr);
		SubMatrixUpdate(AII, rangeintr, rangeintr, A(intr_, intr_));

		El::Zeros(ASI_, sep_.size(), intr_.size());
		vector<int> rangesep;
		RangeVec(0, sep_.size(), rangeintr);
		SubMatrixUpdate(ASI_, rangesep, rangeintr, A(sep_, intr_));

		El::Zeros(ASS_, sep_.size(), sep_.size());
		SubMatrixUpdate(ASS_, rangesep, rangesep, A(sep_, sep_));

		El::Zeros(ANS_, nb_.size(), sep_.size());
		vector<int> rangenb;
		RangeVec(0, nb_.size(), rangenb);
		SubMatrixUpdate(ANS_, rangenb, rangesep, A(nb_, sep_));

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