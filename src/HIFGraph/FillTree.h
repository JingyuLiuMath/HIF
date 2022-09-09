#ifndef FILLTREE_H
#define FILLTREE_H

#include "HIF.h"

namespace HIF {
// Fill the tree structure with A.
template <typename Scalar>
void HIFGraph<Scalar>::FillTree(const SparseMatrixS &A) {
  DEBUG_HIF(CallStackEntry cse("HIFGraph:FillTree"))

  // We only fill the leaf nodes.
  if (endflag_ == 1) {
    Diff_Sort(vtx_, sep_, intr_);

    FullMat(A, intr_, intr_, AII_);

    FullMat(A, sep_, intr_, ASI_);

    FullMat(A, sep_, sep_, ASS_);

    FullMat(A, nb_, sep_, ANS_);

    SetSeparatorType();
  } else {
    for (int iter = 0; iter < 2; iter++) {
      childrennode_.Child(iter).FillTree(A);
    }
  }
}
} // namespace HIF.
#endif // ifndef FILLTREE_H
