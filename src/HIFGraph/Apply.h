#ifndef HIF_SRC_HIFGRAPH_APPLY_H
#define HIF_SRC_HIFGRAPH_APPLY_H

#include "HIF.h"

namespace HIF {
// b = A^{-1} b where A^{-1} is given by HIF.
template <typename Scalar> void HIFGraph<Scalar>::Apply(MatrixS &b) {
  DEBUG_HIF(CallStackEntry cse("HIFGraph:Apply"))

  vector<int> xcol;
  RangeVec(0, b.Width(), xcol);

  // Fill vector in the tree.
  TIMER_HIF(TimerStart(TIMER_FILLVEC))
  FillVecTree(b, xcol);
  TIMER_HIF(TimerStop(TIMER_FILLVEC))

  TIMER_HIF(TimerStart(TIMER_APPLY))

  // ApplyUp.
  for (int tmplevel = numlevels_; tmplevel >= 1; tmplevel--) {
    RecursiveApplySparseElimUp(tmplevel);
    RecursiveApplySkelUp(tmplevel, xcol);
    RecursiveApplyMerge(tmplevel - 1, xcol);
  }

  // Root apply.
  RootApply();

  // ApplyDown.
  for (int tmplevel = 1; tmplevel <= numlevels_; tmplevel++) {
    RecursiveApplySplit(tmplevel - 1, xcol);
    RecursiveApplySkelDown(tmplevel, xcol);
    RecursiveApplySparseElimDown(tmplevel);
  }

  TIMER_HIF(TimerStop(TIMER_APPLY))

  // GetSolution.
  GetSolution(b, xcol);
}
} // namespace HIF.

#endif // ifndef HIF_SRC_HIFGRAPH_APPLY_H
