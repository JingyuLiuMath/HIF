#include "HIF.hpp"

namespace HIF {

// vec = unique(vec) where vec is a sorted vec.
template <typename Scalar> void Unique_Sort(vector<Scalar> &vec) {
  DEBUG_HIF(CallStackEntry cse("Unique_Sort"))

  TIMER_HIF(TimerStart(TIMER_SETOP))
  if (vec.size() == 0) {
    return;
  }
  vector<Scalar> vectmp;
  vectmp.push_back(vec[0]);
  for (int i = 1; i < vec.size(); i++) {
    if (vectmp.back() != vec[i]) {
      vectmp.push_back(vec[i]);
    }
  }
  vec.assign(vectmp.begin(), vectmp.end());
  TIMER_HIF(TimerStop(TIMER_SETOP))
}

} // namespace HIF.
