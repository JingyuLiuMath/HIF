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
		
		MatrixS Aneed;

		std::cout << "Jyliu 1" << std::endl;
		El::Zeros(AII_, intr_.size(), intr_.size());
		vector<int> rangeintr;
		RangeVec(0, intr_.size(), rangeintr);
		FullMat(A(intr_, intr_), Aneed);
		SubMatrixUpdate(AII_, rangeintr, rangeintr, Aneed);
		Aneed.Empty();

		std::cout << "Jyliu 2" << std::endl;
		El::Zeros(ASI_, sep_.size(), intr_.size());
		vector<int> rangesep;
		RangeVec(0, sep_.size(), rangeintr);
		FullMat(A(sep_, intr_), Aneed);
		SubMatrixUpdate(ASI_, rangesep, rangeintr, Aneed);
		Aneed.Empty();

		El::Zeros(ASS_, sep_.size(), sep_.size());
		FullMat(A(sep_, sep_), Aneed);
		SubMatrixUpdate(ASS_, rangesep, rangesep, Aneed);
		Aneed.Empty();

		El::Zeros(ANS_, nb_.size(), sep_.size());
		vector<int> rangenb;
		RangeVec(0, nb_.size(), rangenb);
		FullMat(A(nb_, sep_), Aneed);
		SubMatrixUpdate(ANS_, rangenb, rangesep, Aneed);

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