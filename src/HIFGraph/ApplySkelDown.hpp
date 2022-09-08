#include "HIF.hpp"

namespace HIF {

// Phase 2 for applying skeletonization recusively.
template <typename Scalar>
void HIFGraph<Scalar>::RecursiveApplySkelDown(int whatlevel,
                                              const vector<int> &xcol) {
  DEBUG_HIF(CallStackEntry cse("HIFGraph:RecursiveApplySkelDown"))

  if (level_ == whatlevel) {
    ApplySkelDown(xcol);
  } else {
    if (endflag_ == 0) {
      for (int iter = 0; iter < 2; iter++) {
        childrennode_.Child(iter).RecursiveApplySkelDown(whatlevel, xcol);
      }
    }
  }
}

// Phase 2 for applying skeletonization.
template <typename Scalar>
void HIFGraph<Scalar>::ApplySkelDown(const vector<int> &xcol) {
  DEBUG_HIF(CallStackEntry cse("HIFGraph:ApplySkelDown"))

  for (int k = 0; k < nbinfo_.size(); k++) {
    HIFGraph &nbnodek = *nbnode_[k];
    SkelInfo &nbinfok = nbinfo_[k];
    if (nbinfok.skip == 1) {
      continue;
    }

    MatrixS copyvec; // Copy of updated vector.

    // Step 3.
    // xc2 = Lc2^{-T} * xc2 - (Ac2c2^{-1} * Ac2h1) * xh1 - (Ac2c2^{-1} *
    // Ah2c2^{T}) * xh2.
    copyvec = nbnodek.xS_(nbinfok.nodekindex_p22, xcol);
    Trmm(ELLR::LEFT, LOWER, TRANSPOSE, UNIT, Scalar(1), nbinfok.Ac2c2inv,
         copyvec);
    Gemm(NORMAL, NORMAL, Scalar(-1), nbinfok.Ac2c2invAc2h1,
         xS_(nbinfok.myindex_p11, xcol), Scalar(1), copyvec);
    Gemm(NORMAL, NORMAL, Scalar(-1), nbinfok.Ac2c2invAc2h2,
         nbnodek.xS_(nbinfok.nodekindex_p21, xcol), Scalar(1), copyvec);
    SubMatrixUpdate(nbnodek.xS_, nbinfok.nodekindex_p22, xcol, copyvec);

    // Step 2.
    // xc1 = Lc1^{-T} * xc1 - (Ac1c1^{-1} * Ah1c1^{T}) * xh1 - (Ac1c1^{-1} *
    // Ac2c1^{T}) * xc2 - (Ac1c1^{-1} * Ah2c1^{T}) * xh2.
    copyvec = xS_(nbinfok.myindex_p12, xcol);
    Trmm(ELLR::LEFT, LOWER, TRANSPOSE, UNIT, Scalar(1), nbinfok.Ac1c1inv,
         copyvec);
    Gemm(NORMAL, NORMAL, Scalar(-1), nbinfok.Ac1c1invAc1h1,
         xS_(nbinfok.myindex_p11, xcol), Scalar(1), copyvec);
    Gemm(NORMAL, NORMAL, Scalar(-1), nbinfok.Ac1c1invAc1c2,
         nbnodek.xS_(nbinfok.nodekindex_p22, xcol), Scalar(1), copyvec);
    Gemm(NORMAL, NORMAL, Scalar(-1), nbinfok.Ac1c1invAc1h2,
         nbnodek.xS_(nbinfok.nodekindex_p21, xcol), Scalar(1), copyvec);
    SubMatrixUpdate(xS_, nbinfok.myindex_p12, xcol, copyvec);

    // Step 1.
    // xh1 = xh1 - Th1c1 * xc1.
    copyvec = xS_(nbinfok.myindex_p11, xcol);
    Gemm(NORMAL, NORMAL, Scalar(-1), nbinfok.Th1c1,
         xS_(nbinfok.myindex_p12, xcol), Scalar(1), copyvec);
    SubMatrixUpdate(xS_, nbinfok.myindex_p11, xcol, copyvec);
    // xh2 = xh2 - Th2c2 * xc2.
    copyvec = nbnodek.xS_(nbinfok.nodekindex_p21, xcol);
    Gemm(NORMAL, NORMAL, Scalar(-1), nbinfok.Th2c2,
         nbnodek.xS_(nbinfok.nodekindex_p22, xcol), Scalar(1), copyvec);
    SubMatrixUpdate(nbnodek.xS_, nbinfok.nodekindex_p21, xcol, copyvec);
  }
}

} // namespace HIF.
