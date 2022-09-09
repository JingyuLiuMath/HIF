#ifndef DEBUGTOOLS_H
#define DEBUGTOOLS_H

#include "HIF.h"

namespace HIF {
// Show partition results.
template <typename Scalar> void HIFGraph<Scalar>::ShowPartition() {
  DEBUG_HIF(CallStackEntry cse("HIFGraph:ShowPartition"))

  if (endflag_ == 1) {
    std::cout << " curren level " << level_ << std::endl;
    std::cout << " curren seqnum " << seqnum_ << std::endl;
    ShowVector(vtx_, "vtx");
    ShowVector(sep_, "sep");
    ShowVector(nb_, "nb");
  } else {
    for (int iter = 0; iter < 2; iter++) {
      childrennode_.Child(iter).ShowPartition();
    }
  }
}
} // namespace HIF.
#endif // ifndef DEBUGTOOLS_H
