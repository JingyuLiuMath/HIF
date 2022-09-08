#include "HIF.h"

namespace HIF {

// Find the index of value in vec. If no index is found, return -1.
template <typename Scalar>
int FindIndex_Sort(const vector<Scalar> &vec, const Scalar value) {
  DEBUG_HIF(CallStackEntry cse("FindIndex_Sort"))

  TIMER_HIF(TimerStart(TIMER_SETOP))
  int startsearch = 0;
  int endsearch = vec.size() - 1;
  int tmpindex = -1;
  int index = -1;

  if (vec.size() == 0) {
    return -1;
  }
  if (vec.front() > value) {
    TIMER_HIF(TimerStop(TIMER_SETOP))
    return -1;
  }
  if (vec.back() < value) {
    TIMER_HIF(TimerStop(TIMER_SETOP))
    return -1;
  }
  while (startsearch <= endsearch) {
    tmpindex = startsearch + (endsearch - startsearch) / 2;
    if (vec[tmpindex] < value) {
      startsearch = tmpindex + 1;
    } else if (vec[tmpindex] > value) {
      endsearch = tmpindex - 1;
    } else {
      TIMER_HIF(TimerStop(TIMER_SETOP))
      return tmpindex;
    }
  }
  TIMER_HIF(TimerStop(TIMER_SETOP))
  return -1;
}

// vec[index] = value.
template <typename Scalar>
void FindAllIndex(const vector<Scalar> &vec, const Scalar value,
                  vector<int> &index) {
  DEBUG_HIF(CallStackEntry cse("FindAllIndex"))

  TIMER_HIF(TimerStart(TIMER_SETOP))
  for (int i = 0; i < vec.size(); i++) {
    if (vec[i] == value) {
      index.push_back(i);
    }
  }
  TIMER_HIF(TimerStop(TIMER_SETOP))
}

// vec2[index] = vec1 where vec1 and vec2 are two sorted vectors. Here we assume
// vec1 is a subset of vec2.
template <typename Scalar>
void FindAllIndex_Sort(const vector<Scalar> &vec1, const vector<Scalar> &vec2,
                       vector<int> &index) {
  DEBUG_HIF(CallStackEntry cse("FindAllIndex_Sort"))

  TIMER_HIF(TimerStart(TIMER_SETOP))
  int i = 0;
  int j = 0;
  int startj = -1;
  int endj = -1;
  int tmpindex = -1;
  while (i < vec1.size()) {
    if (vec1[i] == vec2[j]) {
      index.push_back(j);
      i++;
      j++;
    } else {
      // vec1[i] > vec2[j]
      startj = j + 1;
      endj = vec2.size() - 1;
      while (startj <= endj) {
        tmpindex = startj + (endj - startj) / 2;
        if (vec2[tmpindex] < vec1[i]) {
          startj = tmpindex + 1;
        } else if (vec2[tmpindex] > vec1[i]) {
          endj = tmpindex - 1;
        } else {
          startj = tmpindex;
          break;
        }
      }
      j = startj;
    }
  }
  TIMER_HIF(TimerStop(TIMER_SETOP))
}

} // namespace HIF.
