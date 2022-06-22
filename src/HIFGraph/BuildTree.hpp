#include "HIF.hpp"

namespace HIF {

// Build tree structure according to graph partition.
template <typename Scalar>
void HIFGraph<Scalar>::BuildTree(const SparseMatrixS& A, int minvtx)
{
	DEBUG_HIF(CallStackEntry cse("HIFGraph:BuildTree"))

	sort(vtx_.begin(), vtx_.end());
	sort(sep_.begin(), sep_.end());
	sort(nb_.begin(), nb_.end());

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

	// TIMER_GETSUBMATRIX
	/*SparseMatrixS ASub(vtx_.size(), vtx_.size());
	const int* targetA = A.LockedTargetBuffer();
	const int* offsetA = A.LockedOffsetBuffer();
	const Scalar* valueA = A.LockedValueBuffer();
	int Irow = -1;
	int nnzIrow = -1;
	int startIrow = -1;
	int endIrow = -1;
	int i = -1;
	int j = -1;
	int starti = -1;
	int endi = -1;
	int tmpindex = 1;
	int loop = 0;
	for (int row = 0; row < vtx_.size(); row++)
	{
		Irow = vtx_[row];
		startIrow = offsetA[Irow];
		endIrow = offsetA[Irow + 1];
		j = 0;
		starti = startIrow;
		endi = endIrow - 1;
		while (starti < endi)
		{
			tmpindex = (starti + endi) / 2;
			if (targetA[tmpindex] < vtx_[0])
			{
				starti = tmpindex + 1;
			}
			else if (targetA[tmpindex] > vtx_[0])
			{
				endi = tmpindex - 1;
			}
			else
			{
				starti = tmpindex;
				break;
			}
		}
		i = starti;
		while ((i < endIrow) && (j < vtx_.size()))
		{
			if (targetA[i] < vtx_[j])
			{
				i++;
				loop++;
			}
			else if (targetA[i] > vtx_[j])
			{
				j++;
				loop++;
			}
			else
			{
				ASub.QueueUpdate(row, j, valueA[i]);
				i++;
				j++;
				loop++;
			}
		}
	}
	INFO_HIF
	(
		Log(
			"       I_size        J_size        loop"
		);
		Log(
			setw(14), vtx_.size(),
			setw(14), vtx_.size(),
			setw(12), loop
		);
	)
	TIMER_HIF(TimerStart(TIMER_PROCESSQ))
	ASub.ProcessQueues();
	TIMER_HIF(TimerStop(TIMER_PROCESSQ))*/

	vector<int> p1, p2, sp1, sp2;
	GraphPart(tmpA, p1, p2, sp1, sp2);
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

	/*if ((sep_.size() < 20) && (nb_.size() < 20))
	{
		ShowVector(sep_, "sep");
		ShowVector(nb_, "nb");
		const int* sourcetnbA = nbA.LockedSourceBuffer();
		const int* targetnbA = nbA.LockedTargetBuffer();
		const Scalar* valuenbA = nbA.LockedValueBuffer();
		int nnznbA = nbA.NumEntries();
		for (int t = 0; t < nnznbA; t++)
		{
			std::cout << sourcetnbA[t] << " " << targetnbA[t] << " " << valuenbA[t] << std::endl;
		}
	}*/
	// TIMER_GETSUBMATRIX
	/*SparseMatrixS ASub(sep_.size(), nb_.size());
	const int* targetA = A.LockedTargetBuffer();
	const int* offsetA = A.LockedOffsetBuffer();
	const Scalar* valueA = A.LockedValueBuffer();
	int Irow = -1;
	int nnzIrow = -1;
	int startIrow = -1;
	int endIrow = -1;
	int i = -1;
	int j = -1;
	int starti = -1;
	int endi = -1;
	int tmpindex = 1;
	int loop = 0;
	for (int row = 0; row < sep_.size(); row++)
	{
		Irow = sep_[row];
		startIrow = offsetA[Irow];
		endIrow = offsetA[Irow + 1];
		j = 0;
		starti = startIrow;
		endi = endIrow - 1;
		while (starti < endi)
		{
			tmpindex = (starti + endi) / 2;
			if (targetA[tmpindex] < nb_[0])
			{
				starti = tmpindex + 1;
			}
			else if (targetA[tmpindex] > nb_[0])
			{
				endi = tmpindex - 1;
			}
			else
			{
				starti = tmpindex;
				break;
			}
		}
		i = starti;
		while ((i < endIrow) && (j < nb_.size()))
		{
			if (targetA[i] < nb_[j])
			{
				i++;
				loop++;
			}
			else if (targetA[i] > nb_[j])
			{
				j++;
				loop++;
			}
			else
			{
				ASub.QueueUpdate(row, j, valueA[i]);
				i++;
				j++;
				loop++;
			}
		}
	}
	INFO_HIF
	(
		Log(
			"       I_size        J_size        loop"
		);
		Log(
			setw(14), sep_.size(),
			setw(14), nb_.size(),
			setw(12), loop
		);
	)
	TIMER_HIF(TimerStart(TIMER_PROCESSQ))
	ASub.ProcessQueues();
	TIMER_HIF(TimerStop(TIMER_PROCESSQ))*/

	for (int i = 0; i < sep_.size(); i++)
	{
		int sepi = sep_[i];
		for (int iter = 0; iter < 2; iter++)
		{
			HIFGraph* childnode = children_[iter];
			if (FindFirstIndex<int>(childnode->vtx_, sepi) == -1)
			{
				continue;
			}
			else
			{
				if (FindFirstIndex<int>(childnode->sep_, sepi) == -1)
				{
					// Pass sepi.
					(childnode->sep_).push_back(sepi);
				}
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
					if (FindFirstIndex(childnode->nb_, addnbj) == -1)
					{
						(childnode->nb_).push_back(addnbj);
					}
				}
			}
		}
	}
}

} // namespace HIF.
