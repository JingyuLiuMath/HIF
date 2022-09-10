#ifndef HIF_SRC_SETOPERATION_ADDELEMENT_H
#define HIF_SRC_SETOPERATION_ADDELEMENT_H

#include "HIF.h"

namespace HIF {
// Add a sorted vector vec2 to a sorted vector vec1.
template <typename Scalar>
void AddVec_Sort(vector<Scalar> &vec1, const vector<Scalar> &vec2) {
  DEBUG_HIF(CallStackEntry cse("AddVec_Sort"))

  TIMER_HIF(TimerStart(TIMER_SETOP))
  if (vec2.size() == 0) {
    TIMER_HIF(TimerStop(TIMER_SETOP))
    return;
  }
  if (vec1.size() == 0) {
    vec1.assign(vec2.begin(), vec2.end());
    TIMER_HIF(TimerStop(TIMER_SETOP))
    return;
  }
  if (vec1.back() < vec2.front()) {
    for (int k = 0; k < vec2.size(); k++) {
      vec1.push_back(vec2[k]);
    }
    TIMER_HIF(TimerStop(TIMER_SETOP))
    return;
  }
  vector<Scalar> vec;
  if (vec1.front() > vec2.back()) {
    vec.assign(vec2.begin(), vec2.end());
    for (int k = 0; k < vec1.size(); k++) {
      vec.push_back(vec1[k]);
    }
    vec1.assign(vec.begin(), vec.end());
    TIMER_HIF(TimerStop(TIMER_SETOP))
    return;
  }
  int i = 0;
  int j = 0;
  while (i < vec1.size() && j < vec2.size()) {
    if (vec1[i] < vec2[j]) {
      vec.push_back(vec1[i]);
      i++;
    } else if (vec1[i] > vec2[j]) {
      vec.push_back(vec2[j]);
      j++;
    } else {
      vec.push_back(vec1[i]);
      i++;
      j++;
    }
  }
  while (i < vec1.size()) {
    vec.push_back(vec1[i]);
    i++;
  }
  while (j < vec2.size()) {
    vec.push_back(vec2[j]);
    j++;
  }
  vec1.assign(vec.begin(), vec.end());
  TIMER_HIF(TimerStop(TIMER_SETOP))
}
} // namespace HIF.

#endif // ifndef HIF_SRC_SETOPERATION_ADDELEMENT_H
