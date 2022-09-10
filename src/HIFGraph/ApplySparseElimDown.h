#ifndef HIF_SRC_HIFGRAPH_APPLYSPARSEELIMDOWN_H
#define HIF_SRC_HIFGRAPH_APPLYSPARSEELIMDOWN_H

#include "HIF.h"

namespace HIF {
// Phase 2 for applying sparse elimination recusively.
template <typename Scalar>
void HIFGraph<Scalar>::RecursiveApplySparseElimDown(int whatlevel) {
  DEBUG_HIF(CallStackEntry cse("HIFGraph:RecursiveApplySparseElimDown"))

  if (level_ == whatlevel) {
    ApplySparseElimDown();
  } else {
    if (endflag_ == 0) {
      for (int iter = 0; iter < 2; iter++) {
        childrennode_.Child(iter).RecursiveApplySparseElimDown(whatlevel);
      }
    }
  }
}

// Phase 2 for applying sparse elimination.
template <typename Scalar> void HIFGraph<Scalar>::ApplySparseElimDown() {
  DEBUG_HIF(CallStackEntry cse("HIFGraph:ApplySparseElimDown"))

  // xI = LI^{-T} * xI - (AII^{-1} * ASI^{T}) * xS.
  Trmm(ELLR::LEFT, LOWER, TRANSPOSE, UNIT, Scalar(1), AIIinv_, xI_);
  Gemm(NORMAL, NORMAL, Scalar(-1), AIIinvAIS_, xS_, Scalar(1), xI_);
}
} // namespace HIF.

#endif // ifndef HIF_SRC_HIFGRAPH_APPLYSPARSEELIMDOWN_H
