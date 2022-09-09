#include "HIF.h"

//#include "./BuildTree.h"
#include "./FillTree.h"
#include "./SetNeighborNode.h"
#include "./SetSeparatorType.h"

#include "./Factorization.h"
#include "./Merge.h"
#include "./RootFacorization.h"
#include "./Skel.h"
#include "./SparseElim.h"

#include "./Apply.h"
#include "./ApplyMerge.h"
#include "./ApplySkelDown.h"
#include "./ApplySkelUp.h"
#include "./ApplySparseElimDown.h"
#include "./ApplySparseElimUp.h"
#include "./ApplySplit.h"
#include "./FillVecTree.h"
#include "./GetSolution.h"
#include "./RootApply.h"

#include "./DebugTools.h"

namespace HIF {

// Create a HIFGraph class.
template <typename Scalar>
HIFGraph<Scalar>::HIFGraph(const SparseMatrixS &A, int minvtx) {
  DEBUG_HIF(CallStackEntry cse("HIFGraph:HIFGraph"))

  vtx_.resize(A.Height());
  for (int i = 0; i < vtx_.size(); i++) {
    vtx_[i] = i;
  }
  level_ = 0;
  seqnum_ = 0;

  TIMER_HIF(TimerStart(TIMER_INIT_BUILD))
  BuildTree(A, minvtx);
  SetNeighborNode();
  TIMER_HIF(TimerStop(TIMER_INIT_BUILD))

  TIMER_HIF(TimerStart(TIMER_INIT_FILL))
  FillTree(A);
  TIMER_HIF(TimerStop(TIMER_INIT_FILL))

  INFO_HIF(
      Log("==================================================",
          "==================================================",
          "==================================================");
      Log("        matrixsize        minvtx        numlevels        tolerance");
      Log(setw(18), A.Height(), setw(14), minvtx, setw(17), numlevels_,
          setw(17), scientific, setprecision(2), Tol());)

  // SetOpTime are accumulative.
  INFO_HIF(
      Log("==================================================",
          "==================================================",
          "==================================================");
      Log("        BuildTree        FillTree        SetOperation        Metis",
          "        Pass        SetSep2");
      Log(setw(17), scientific, setprecision(2), TimerTotal(TIMER_INIT_BUILD),
          setw(16), scientific, setprecision(2), TimerTotal(TIMER_INIT_FILL),
          setw(20), scientific, setprecision(2), TimerTotal(TIMER_SETOP),
          setw(13), scientific, setprecision(2), TimerTotal(TIMER_METIS),
          setw(12), scientific, setprecision(2), TimerTotal(TIMER_PASS),
          setw(15), scientific, setprecision(2), TimerTotal(TIMER_SETSEP2));)
}

// Create a HIFGraph class.
template <typename Scalar>
HIFGraph<Scalar>::HIFGraph(int level, int seqnum, const vector<int> &vtx,
                           const vector<int> &sep, const vector<int> &nb) {
  DEBUG_HIF(CallStackEntry cse("HIFGraph:HIFGraph"))

  level_ = level;
  seqnum_ = seqnum;
  vtx_.assign(vtx.begin(), vtx.end());
  sep_.assign(sep.begin(), sep.end());
  nb_.assign(nb.begin(), nb.end());
}

// Clear HIFGraph.
template <typename Scalar> HIFGraph<Scalar>::~HIFGraph() {
  // Nothing need to be done.
}

template class HIFGraph<float>;
template class HIFGraph<double>;

} // namespace HIF.
