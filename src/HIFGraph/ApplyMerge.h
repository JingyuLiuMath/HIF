#ifndef HIF_SRC_HIFGRAPH_APPLYMERGE_H
#define HIF_SRC_HIFGRAPH_APPLYMERGE_H

#include "HIF.h"

namespace HIF {
// Recusively send vectors' information from children to parent.
template <typename Scalar>
void HIFGraph<Scalar>::RecursiveApplyMerge(int whatlevel,
                                           const vector<int> &xcol) {
  DEBUG_HIF(CallStackEntry cse("HIFGraph:RecursiveApplyMerge"))

  if (level_ == whatlevel) {
    ApplyMerge(xcol);
  } else {
    if (endflag_ == 0) {
      for (int iter = 0; iter < 2; iter++) {
        childrennode_.Child(iter).RecursiveApplyMerge(whatlevel, xcol);
      }
    }
  }
}

// Send vectors' information from children to parent.
template <typename Scalar>
void HIFGraph<Scalar>::ApplyMerge(const vector<int> &xcol) {
  DEBUG_HIF(CallStackEntry cse("HIFGraph:ApplyMerge"))

  // We stand on the parent level.
  if (endflag_ == 1) {
    return;
  }

  // xI.
  // An intr of the parent only belongs to the sep of one of its children.
  // We assign xI from the children's xS.
  //
  // xS.
  // A sep of the parent only belongs to the sep of one of its children.
  // We assign xS from the child's xS.
  El::Zeros(xI_, intr_.size(), xcol.size());
  El::Zeros(xS_, sep_.size(), xcol.size());
  for (int iter = 0; iter < 2; iter++) {
    SubMatrixUpdate(
        xI_, childreninfo_[iter].myindex_intr, xcol,
        childrennode_.Child(iter).xS_(childreninfo_[iter].cindex_intr, xcol));
    SubMatrixUpdate(
        xS_, childreninfo_[iter].myindex_sep, xcol,
        childrennode_.Child(iter).xS_(childreninfo_[iter].cindex_sep, xcol));
  }
}
} // namespace HIF.

#endif // ifndef HIF_SRC_HIFGRAPH_APPLYMERGE_H
