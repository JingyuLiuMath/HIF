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
		
		std::cout << " level " << level_ << std::endl;
		std::cout << " seqnum " << seqnum_ << std::endl;
		for (int i = 0; i < intr_.size(); i++)
		{
			std::cout << " intr_[i] " << intr_[i] << std::endl;
		}
		for (int i = 0; i < sep_.size(); i++)
		{
			std::cout << " sep_[i] " << sep_[i] << std::endl;
		}
		for (int i = 0; i < intr_.size(); i++)
		{
			std::cout << " nb_[i] " << nb_[i] << std::endl;
		}
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
		RangeVec(0, sep_.size(), rangesep);
		FullMat(A(sep_, intr_), Aneed);
		SubMatrixUpdate(ASI_, rangesep, rangeintr, Aneed);
		Aneed.Empty();

		std::cout << "Jyliu 3" << std::endl;
		El::Zeros(ASS_, sep_.size(), sep_.size());
		FullMat(A(sep_, sep_), Aneed);
		SubMatrixUpdate(ASS_, rangesep, rangesep, Aneed);
		Aneed.Empty();

		std::cout << "Jyliu 4" << std::endl;
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