#include "HIF.hpp"

namespace HIF {

// Build tree structure according to graph partition.
template <typename Scalar>
void HIFGraph<Scalar>::BuildTree(const SparseMatrixS& A, int minvtx)
{
	DEBUG_HIF(CallStackEntry cse("HIFGraph:BuildTree"))

	// TODO: remove sort.
	/*sort(vtx_.begin(), vtx_.end());
	sort(sep_.begin(), sep_.end());
	sort(nb_.begin(), nb_.end());*/

	// Don't partition if the number of vtx is less than minvtx.
	if (vtx_.size() <= minvtx)
	{
		numlevels_ = level_;
		endflag_ = 1;
		return;
	}

	// Partition.

	TIMER_HIF(TimerStart(TIMER_GETSUBMATRIX))
	SparseMatrixS tmpA = A(vtx_, vtx_);
	TIMER_HIF(TimerStop(TIMER_GETSUBMATRIX))

	vector<int> p1, p2, sp1, sp2;
	GraphPart(tmpA, p1, p2, sp1, sp2); // p1, p2, sp1, sp2 are sorted.
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
	children_[0] = new HIFGraph(level_ + 1, 2 * seqnum_,
		vtx1, sep1, sep2);
	children_[1] = new HIFGraph(level_ + 1, 2 * seqnum_ + 1,
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
	for (int i = 0; i < sep_.size(); i++)
	{
		int sepi = sep_[i];
		for (int iter = 0; iter < 2; iter++)
		{
			HIFGraph* childnode = children_[iter];
			/*if (FindIndex_Sort(childnode->vtx_, sepi) == -1)
			{
				continue;
			}*/
			if (FindFirstIndex(childnode->vtx_, sepi) == -1)
			{
				continue;
			}
			else
			{
				// Pass sep.
				/*if (FindFirstIndex<int>(childnode->sep_, sepi) == -1)
				{
					(childnode->sep_).push_back(sepi);
				}*/
				AddElement_Sort(childnode->sep_, sepi);
				// Pass nb.				
				vector<int> index_addnb(nbA.Width());
				int actualsize_index_addnb = 0;
				const int* targetnbA = nbA.LockedTargetBuffer();
				const int* offsetnbA = nbA.LockedOffsetBuffer();
				for (int k = offsetnbA[i]; k < offsetnbA[i + 1]; k++)
				{
					index_addnb[actualsize_index_addnb] = targetnbA[k];
					actualsize_index_addnb++;
				}
				index_addnb.erase(index_addnb.begin() + actualsize_index_addnb, index_addnb.end());
				for (int j = 0; j < index_addnb.size(); j++)
				{
					int addnbj = nb_[index_addnb[j]];
					/*if (FindFirstIndex(childnode->nb_, addnbj) == -1)
					{
						(childnode->nb_).push_back(addnbj);
					}*/
					AddElement_Sort(childnode->nb_, addnbj);
				}
			}
		}
	}
	TIMER_HIF(TimerStop(TIMER_PASS))
}

} // namespace HIF.
