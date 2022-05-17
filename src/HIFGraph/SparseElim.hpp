#include "HIF.hpp"

namespace HIF {

// Recursively sparse elimination.
template <typename Scalar>
void HIFGraph<Scalar>::RecursiveSparseElim(int whatlevel)
{
	if (level_ == whatlevel)
	{
		SparseElim();
	}
	else
	{
		if (endflag_ == 0)
		{
			for (int iter = 0; iter < 2; iter++)
			{
				children_[iter]->RecursiveSparseElim(whatlevel);
			}
		}
	}

}

// Sparse elimination.
template <typename Scalar>
void HIFGraph<Scalar>::SparseElim()
{
	// AII = LI * DI * LI^{T}.
	LDLSolve(AII_, AIIinv_);
	// AIIinvAIS = AII^{-1} * ASI^{T}.
	Transpose(ASI_, AIIinvAIS_);
	MultiplySolve(AIIinv_, AIIinvAIS_);
	// AII = 0, ASI = 0.
	AII.Empty();
	ASI.Empty();
}

} // namespace HIF.