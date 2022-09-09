#ifndef FILLVECTREE_H
#define FILLVECTREE_H

#include "HIF.h"

namespace HIF {
// Fill the tree structure with b.
template <typename Scalar>
void HIFGraph<Scalar>::FillVecTree(const MatrixS &b, const vector<int> &xcol) {
  DEBUG_HIF(CallStackEntry cse("HIFGraph:FillVecTree"))

  if (endflag_ == 1) {
    xI_ = b(intr_, xcol);
    xS_ = b(sep_, xcol);
  } else {
    for (int iter = 0; iter < 2; iter++) {
      childrennode_.Child(iter).FillVecTree(b, xcol);
    }
  }
}
} // namespace HIF.
#endif // ifndef FILLVECTREE_H
