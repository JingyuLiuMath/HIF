#include "HIF.hpp"

namespace HIF {

// Recusively send vectors' information from children to parent.
template <typename Scalar>
void HIFGraph<Scalar>::RecursiveApplyMerge(int whatlevel, const vector<int>& xcol)
{
	if (level_ == whatlevel)
	{
		ApplyMerge(xcol);
	}
	else
	{
		if (endflag_ == 0)
		{
			for (int iter = 0; iter < 2; iter++)
			{
				children_[iter]->RecursiveApplyMerge(whatlevel,xcol);
			}
		}
	}
}

// Send vectors' information from children to parent.
template <typename Scalar>
void HIFGraph<Scalar>::ApplyMerge(const vector<int>& xcol)
{
	// We stand on the parent level.

	if (endflag_ == 1)
	{
		return;
	}

	MatrixS copyvec; // Copy of updated vector.

	// xI.
	// An intr of the parent only belongs to the sep of one of its children. 
	// We assign xI from the children's xS.
	El::Zeros(xI_, intr_.size(), xcol.size());
	for (int iter = 0; iter < 2; iter++)
	{
		copyvec = (children_[iter]->xS_)(childreninfo_[iter].cindex_intr,xcol);
		SubMatrixUpdate(xI_, childreninfo_[iter].myindex_intr, xcol, copyvec);
		copyvec.Empty();
	}

	// xS.
	// A sep of the parent only belongs to the sep of one of its children.
	// We assign xS from the child's xS.
	El::Zeros(xS_, sep_.size(), xcol.size());
	for (int iter = 0; iter < 2; iter++)
	{
		copyvec = (children_[iter]->xS_)(childreninfo_[iter].cindex_sep, xcol);
		SubMatrixUpdate(xS_, childreninfo_[iter].myindex_sep, xcol, copyvec);
		copyvec.Empty();
	}
}

} // namespace HIF.