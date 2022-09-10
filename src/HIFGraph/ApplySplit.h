#ifndef HIF_SRC_HIFGRAPH_APPLYSPLIT_H
#define HIF_SRC_HIFGRAPH_APPLYSPLIT_H

#include "HIF.h"

namespace HIF {
// Recusively send vectors' information from parent to children.
template <typename Scalar>
void HIFGraph<Scalar>::RecursiveApplySplit(int whatlevel,
                                           const vector<int> &xcol) {
  DEBUG_HIF(CallStackEntry cse("HIFGraph:RecursiveApplySplit"))

  if (level_ == whatlevel) {
    ApplySplit(xcol);
  } else {
    if (endflag_ == 0) {
      for (int iter = 0; iter < 2; iter++) {
        childrennode_.Child(iter).RecursiveApplySplit(whatlevel, xcol);
      }
    }
  }
}

// Send vectors' information from parent to children.
template <typename Scalar>
void HIFGraph<Scalar>::ApplySplit(const vector<int> &xcol) {
  DEBUG_HIF(CallStackEntry cse("HIFGraph:ApplySplit"))

  // We stand on the parent level.

  if (endflag_ == 1) {
    return;
  }

  for (int iter = 0; iter < 2; iter++) {
    SubMatrixUpdate(childrennode_.Child(iter).xS_,
                    childreninfo_[iter].cindex_intr, xcol,
                    xI_(childreninfo_[iter].myindex_intr, xcol));
    SubMatrixUpdate(childrennode_.Child(iter).xS_,
                    childreninfo_[iter].cindex_sep, xcol,
                    xS_(childreninfo_[iter].myindex_sep, xcol));
  }
}
} // namespace HIF.

#endif // ifndef HIF_SRC_HIFGRAPH_APPLYSPLIT_H
