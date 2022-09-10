#ifndef ROOTFACTORIZATION_H
#define ROOTFACTORIZATION_H

#include "HIF.h"

namespace HIF {
template <typename Scalar> void HIFGraph<Scalar>::RootFactorization() {
  DEBUG_HIF(CallStackEntry cse("HIFGraph:RootFactorization"))

  TIMER_HIF(TimerStart(TIMER_EL))
  TIMER_HIF(TimerStart(TIMER_EL_LDLSOLVE))
  LDLSolve(AII_, AIIinv_);
  TIMER_HIF(TimerStop(TIMER_EL_LDLSOLVE))
  TIMER_HIF(TimerStop(TIMER_EL))

  // AII = 0.
  AII_.Empty();
}
} // namespace HIF.

#endif // ifndef ROOTFACTORIZATION_H
