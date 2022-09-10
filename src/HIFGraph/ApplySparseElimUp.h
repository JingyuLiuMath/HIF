#ifndef APPLYSPARSEELIMUP_H
#define APPLYSPARSEELIMUP_H

#include "HIF.h"

namespace HIF {
// Phase 1 for applying sparse elimination recusively.
template <typename Scalar>
void HIFGraph<Scalar>::RecursiveApplySparseElimUp(int whatlevel) {
  DEBUG_HIF(CallStackEntry cse("HIFGraph:RecursiveApplySparseElimUp"))

  if (level_ == whatlevel) {
    ApplySparseElimUp();
  } else {
    if (endflag_ == 0) {
      for (int iter = 0; iter < 2; iter++) {
        childrennode_.Child(iter).RecursiveApplySparseElimUp(whatlevel);
      }
    }
  }
}

// Phase 1 for applying sparse elimination.
template <typename Scalar> void HIFGraph<Scalar>::ApplySparseElimUp() {
  DEBUG_HIF(CallStackEntry cse("HIFGraph:ApplySparseElimUp"))

  // xS = xS - (AII^{-1} * ASI^{T})^{T} * xI.
  Gemm(TRANSPOSE, NORMAL, Scalar(-1), AIIinvAIS_, xI_, Scalar(1), xS_);
  // xI = LI^{-1} * xI.
  Trmm(ELLR::LEFT, LOWER, NORMAL, UNIT, Scalar(1), AIIinv_, xI_);

  // xI = DI^{-1} * xI. We only apply D once.
  auto D = GetDiagonal(AIIinv_);
  DiagonalScale(ELLR::LEFT, NORMAL, D, xI_);
}
} // namespace HIF.

#endif // ifndef APPLYSPARSEELIMUP_H
