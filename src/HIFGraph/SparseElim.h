#ifndef HIF_SRC_HIFGRAPH_SPARSEELIM_H
#define HIF_SRC_HIFGRAPH_SPARSEELIM_H

#include "HIF.h"

namespace HIF {
// Recursively sparse elimination.
template <typename Scalar>
void HIFGraph<Scalar>::RecursiveSparseElim(int whatlevel) {
  DEBUG_HIF(CallStackEntry cse("HIFGraph:RecursiveSparseElim"))

  if (level_ == whatlevel) {
    SparseElim();
  } else {
    if (endflag_ == 0) {
      for (int iter = 0; iter < 2; iter++) {
        childrennode_.Child(iter).RecursiveSparseElim(whatlevel);
      }
    }
  }
}

// Sparse elimination.
template <typename Scalar> void HIFGraph<Scalar>::SparseElim() {
  DEBUG_HIF(CallStackEntry cse("HIFGraph:SparseElim"))

  TIMER_HIF(TimerStart(TIMER_EL))

  // AII = LI * DI * LI^{T}. We store LI^{-1} and DI^{-1} in AIIinv_.
  TIMER_HIF(TimerStart(TIMER_EL_LDLSOLVE))
  LDLSolve(AII_, AIIinv_);
  TIMER_HIF(TimerStop(TIMER_EL_LDLSOLVE))

  // AIIinvAIS = AII^{-1} * ASI^{T}.
  Transpose(ASI_, AIIinvAIS_);
  MultiplySolve(AIIinv_, AIIinvAIS_);

  // ASS = ASS - ASI * AII^ {-1} * ASI^ {T}.
  Gemm(NORMAL, NORMAL, Scalar(-1), ASI_, AIIinvAIS_, Scalar(1), ASS_);

  TIMER_HIF(TimerStop(TIMER_EL))

  // AII = 0, ASI = 0.
  AII_.Empty();
  ASI_.Empty();
}
} // namespace HIF.

#endif // ifndef HIF_SRC_HIFGRAPH_SPARSEELIM_H
