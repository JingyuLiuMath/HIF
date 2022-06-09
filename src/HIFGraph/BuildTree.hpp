#include "HIF.hpp"

namespace HIF {

// Build tree structure according to graph partition.
template <typename Scalar>
void HIFGraph<Scalar>::BuildTree(const SparseMatrix<Scalar>& A, int minvtx)
{
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
	SparseMatrix<Scalar> tmpA = A(vtx_, vtx_);
	vector<int> p1, p2, sp1, sp2;
	std::cout << "adadad" << std::endl;
	GraphPart(tmpA, p1, p2, sp1, sp2);
	std::cout << "hhkh" << std::endl;
	/*std::cout << " level " << level_ << std::endl;
	std::cout << " seqnum " << seqnum_ << std::endl;
	std::cout << " p1 " << std::endl;
	for (int i = 0; i < p1.size(); i++)
	{
		std::cout << p1[i] << std::endl;
	}
	std::cout << " sp1 " << std::endl;
	for (int i = 0; i < sp1.size(); i++)
	{
		std::cout << sp1[i] << std::endl;
	}
	std::cout << " p2 " << std::endl;
	for (int i = 0; i < p2.size(); i++)
	{
		std::cout << p2[i] << std::endl;
	}
	std::cout << " sp2 " << std::endl;
	for (int i = 0; i < sp2.size(); i++)
	{
		std::cout << sp2[i] << std::endl;
	}*/
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
void HIFGraph<Scalar>::PassSeparatorNeighbor(const SparseMatrix<Scalar>& A)
{
	std::cout << "PassSeparatorNeighbor" << std::endl;
	SparseMatrix<Scalar> nbA = A(sep_, nb_);
	std::cout << "Pasdasdasdr" << std::endl;
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
				vector<int> index_addnb;
				for (int col = 0; col < nbA.Width(); col++)
				{
					if (nbA.Get(i, col) != Scalar(0))
					{
						index_addnb.push_back(col);
					}
				}
				std::cout << "7788" << std::endl;
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