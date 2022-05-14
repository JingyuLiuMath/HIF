#include "HIF.hpp"

#include "./BuildTree.hpp"
#include "./SetNeighborNode.hpp"
#include "./FillTree.hpp"
#include "./SetSeparatorType.hpp"

namespace HIF {

// Initialization.
template<typename Scalar>
HIFGraph<Scalar>::HIFGraph
(const SparseMatrix<Scalar>& A, int minvtx)
{
	for (int i = 0; i < Height(A); i++)
		vtx_[i] = i;
	level_ = 0;
	seqnum_ = 0;

	BuildTree(A, minvtx);
	SetNeighborNode();
	FillTree(A);
}

// Create a HIFGraph class.
template<typename Scalar>
HIFGraph<Scalar>::HIFGraph
(int level, int seqnum,
vector<int>& vtx, vector<int>& sep, vector<int>& nb)
{
	level_ = level;
	seqnum_ = seqnum;
	vtx_.assign(vtx.begin(), vtx.begin() + vtx.size());
	sep_.assign(sep.begin(), sep.begin() + sep.size());
	nb_.assign(nb.begin(), nb.begin() + nb.size());
}

// Clear HIFGraph.
template<typename Scalar>
HIFGraph<Scalar>::~HIFGraph()
{
	// TODO: clear HIFGraph.
}

template class HIFGraph<float>;
template class HIFGraph<double>;

} // namespace HIF.