#include "HIF.hpp"

namespace HIF {

// Build tree structure according to graph partition.
template <typename Scalar>
void HIFGraph<Scalar>::BuildTree(const SparseMatrixS& A, int minvtx)
{
	DEBUG_HIF(CallStackEntry cse("HIFGraph:BuildTree"))

	// Don't partition if the number of vtx is less than minvtx.
	if (vtx_.size() <= minvtx)
	{
		numlevels_ = level_;
		endflag_ = 1;
		return;
	}

	// Partition.
	/*TIMER_HIF(TimerStart(TIMER_GETSUBMATRIX))
	SparseMatrixS tmpA = A(vtx_, vtx_);
	TIMER_HIF(TimerStop(TIMER_GETSUBMATRIX))*/

	vector<int> p1, p2, sp1, sp2;
	// GraphPart(tmpA, p1, p2, sp1, sp2); // p1, p2, sp1, sp2 are sorted.
	GraphPart(A(vtx_, vtx_), p1, p2, sp1, sp2);
	vector<int> vtx1, vtx2, sep1, sep2;
	vtx1.resize(p1.size());
	for (int i = 0; i < vtx1.size(); i++)
	{
		vtx1[i] = vtx_[p1[i]];
	}
	vtx2.resize(p2.size());
	for (int i = 0; i < vtx2.size(); i++)
	{
		vtx2[i] = vtx_[p2[i]];
	}
	sep1.resize(sp1.size());
	for (int i = 0; i < sep1.size(); i++)
	{
		sep1[i] = vtx_[sp1[i]];
	}
	sep2.resize(sp2.size());
	for (int i = 0; i < sep2.size(); i++)
	{
		sep2[i] = vtx_[sp2[i]];
	}
	
	// Create children HIFGraph.
	children_.resize(2);
	children_[0] = new HIFGraph<Scalar>(level_ + 1, 2 * seqnum_,
		vtx1, sep1, sep2);
	children_[1] = new HIFGraph<Scalar>(level_ + 1, 2 * seqnum_ + 1,
		vtx2, sep2, sep1);

	// Send information to children.
	PassSeparatorNeighbor(A);

	// Recursively build tree.
	for (int iter = 0; iter < 2; iter++)
	{
		children_[iter]->BuildTree(A, minvtx);
	}

	// Set numlevels.
	numlevels_ = std::max(children_[0]->numlevels_, children_[1]->numlevels_);
}

// Pass parent's sep, nb to children.
template <typename Scalar>
void HIFGraph<Scalar>::PassSeparatorNeighbor(const SparseMatrixS& A)
{
	DEBUG_HIF(CallStackEntry cse("HIFGraph:PassSeparatorNeighbor"))
	
	TIMER_HIF(TimerStart(TIMER_GETSUBMATRIX))
	SparseMatrixS nbA = A(sep_, nb_);
	TIMER_HIF(TimerStop(TIMER_GETSUBMATRIX))

	TIMER_HIF(TimerStart(TIMER_PASS))
	vector<int> addsep1;
	vector<int> addnb1;
	vector<int> addsep2;
	vector<int> addnb2;
	
	//for (int i = 0; i < sep_.size(); i++)
	//{
	//	int sepi = sep_[i];
	//	for (int iter = 0; iter < 2; iter++)
	//	{
	//		HIFGraph* childnode = children_[iter];
	//		if (FindIndex_Sort(childnode->vtx_, sepi) == -1)
	//		{
	//			continue;
	//		}
	//		else
	//		{
	//			// Pass sep.
	//			AddElement_Sort(childnode->sep_, sepi);
	//			// Pass nb.				
	//			vector<int> index_addnb(nbA.Width());
	//			int actualsize_index_addnb = 0;
	//			const int* targetnbA = nbA.LockedTargetBuffer();
	//			const int* offsetnbA = nbA.LockedOffsetBuffer();
	//			for (int k = offsetnbA[i]; k < offsetnbA[i + 1]; k++)
	//			{
	//				index_addnb[actualsize_index_addnb] = targetnbA[k];
	//				actualsize_index_addnb++;
	//			}
	//			index_addnb.erase(index_addnb.begin() + actualsize_index_addnb, index_addnb.end());
	//			for (int j = 0; j < index_addnb.size(); j++)
	//			{
	//				int addnbj = nb_[index_addnb[j]];
	//				AddElement_Sort(childnode->nb_, addnbj);
	//			}
	//		}
	//	}
	//}

	for (int i = 0; i < sep_.size(); i++)
	{
		int sepi = sep_[i];
		for (int iter = 0; iter < 2; iter++)
		{
			HIFGraph* childnode = children_[iter];
			if (FindIndex_Sort(childnode->vtx_, sepi) == -1)
			{
				continue;
			}
			else
			{
				if (iter == 0)
				{
					// Pass sep.
					addsep1.push_back(sepi);
					// Pass nb.				
					const int* targetnbA = nbA.LockedTargetBuffer();
					const int* offsetnbA = nbA.LockedOffsetBuffer();
					for (int k = offsetnbA[i]; k < offsetnbA[i + 1]; k++)
					{
						addnb1.push_back(nb_[targetnbA[k]]);
					}
				}
				else
				{
					// Pass sep.
					addsep2.push_back(sepi);
					// Pass nb.				
					const int* targetnbA = nbA.LockedTargetBuffer();
					const int* offsetnbA = nbA.LockedOffsetBuffer();
					for (int k = offsetnbA[i]; k < offsetnbA[i + 1]; k++)
					{
						addnb2.push_back(nb_[targetnbA[k]]);
					}
				}
			}
		}
	}
	sort(addnb1.begin(), addnb1.end());
	sort(addnb2.begin(), addnb2.end());
	std::unique(addnb1.begin(), addnb1.end());
	std::unique(addnb2.begin(), addnb2.end());
	AddVec_Sort(children_[0]->sep_, addsep1);
	AddVec_Sort(children_[1]->sep_, addsep2);
	AddVec_Sort(children_[0]->nb_, addnb1);
	AddVec_Sort(children_[1]->nb_, addnb2);
	TIMER_HIF(TimerStop(TIMER_PASS))
}

} // namespace HIF.
