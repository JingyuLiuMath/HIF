#include "HIF.hpp"

#include "./BuildTree.hpp"
#include "./SetNeighborNode.hpp"
#include "./FillTree.hpp"
#include "./SetSeparatorType.hpp"

#include "./Factorization.hpp"
#include "./SparseElim.hpp"
#include "./Skel.hpp"
#include "./Merge.hpp"
#include "./RootFacorization.hpp"

#include "./Apply.hpp"
#include "./FillVecTree.hpp"
#include "./ApplySparseElimUp.hpp"
#include "./ApplySkelUp.hpp"
#include "./ApplyMerge.hpp"
#include "./RootApply.hpp"
#include "./ApplySplit.hpp"
#include "./ApplySkelDown.hpp"
#include "./ApplySparseElimDown.hpp"
#include "./GetSolution.hpp"

#include "./DebugTools.hpp"

namespace HIF {

// Create a HIFGraph class.
template <typename Scalar>
HIFGraph<Scalar>::HIFGraph
(const SparseMatrixS& A, int minvtx)
{
	DEBUG_HIF(CallStackEntry cse("HIFGraph:HIFGraph"))

	vtx_.resize(A.Height());
	for (int i = 0; i < vtx_.size(); i++)
	{
		vtx_[i] = i;
	}
	level_ = 0;
	seqnum_ = 0;

	TIMER_HIF(TimerStart(TIMER_INIT_BUILD))
	BuildTree(A, minvtx);
	TIMER_HIF(TimerStop(TIMER_INIT_BUILD))

	TIMER_HIF(TimerStart(TIMER_INIT_SETNB))
	SetNeighborNode();
	TIMER_HIF(TimerStop(TIMER_INIT_SETNB))

	TIMER_HIF(TimerStart(TIMER_INIT_FILL))
	FillTree(A);
	TIMER_HIF(TimerStop(TIMER_INIT_FILL))

	/*INFO_HIF
	(
		Log("=================================================",
			"=========================");
		Log("          ", 
			"  MatrixSize    minvtx    numlevels    tol");
		Log(setw(17), A.Height(),
			setw(8), minvtx,
			setw(8), numlevels_,
			setw(10), scientific, setprecision(2), Tol());
	)*/
}

// Create a HIFGraph class.
template <typename Scalar>
HIFGraph<Scalar>::HIFGraph
(int level, int seqnum,
vector<int>& vtx, vector<int>& sep, vector<int>& nb)
{
	DEBUG_HIF(CallStackEntry cse("HIFGraph:HIFGraph"))

	level_ = level;
	seqnum_ = seqnum;
	vtx_.assign(vtx.begin(), vtx.end());
	sep_.assign(sep.begin(), sep.end());
	nb_.assign(nb.begin(), nb.end());
}

// Clear HIFGraph.
template <typename Scalar>
HIFGraph<Scalar>::~HIFGraph()
{
	// Nothing need to be done.
}

template class HIFGraph<float>;
template class HIFGraph<double>;

} // namespace HIF.
