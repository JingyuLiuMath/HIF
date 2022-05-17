#include "HIF.hpp"

namespace HIF {

// Phase 1 for applying skeletonization recusively.
template <typename Scalar>
void HIFGraph<Scalar>::RecursiveApplySkelUp(int whatlevel, const vector<int>& xcol)
{
	if (level_ == whatlevel)
	{
		ApplySkelUp(xcol);
	}
	else
	{
		if (endflag_ == 0)
		{
			for (int iter = 0; iter < 2; iter++)
			{
				children_[iter]->RecursiveApplySkelUp(whatlevel, xcol);
			}
		}
	}

}

// Phase 1 for applying skeletonization.
template <typename Scalar>
void HIFGraph<Scalar>::ApplySkelUp(const vector<int>& xcol)
{
	for (int k = 0; k < nbnode_.size(); k++)
	{
		HIFGraph* nbnodek = nbnode_[k];
		SkelInfo& nbinfok = nbinfo_[k];
		if (nbinfok.skip == 1)
		{
			continue;
		}

		MatrixS copyvec; // Copy of updated vector.
		// Step 1.
		// xc1 = xc1 - Th1c1^{T} * xh1.
		copyvec = xS_(nbinfok.myindex_p12, xcol);
		Gemm(TRANSPOSE, NORMAL,
			Scalar(-1), nbinfok.Th1c1, xS_(nbinfok.myindex_p11, xcol),
			Scalar(1), copyvec);
		SubMatrixUpdate(xS_, nbinfok.myindex_p12, xcol, copyvec);
		copyvec.Empty();
		// xc2 = xc2 - Th2c2^{T} * xh2.
		copyvec = (nbnodek->xS_)(nbinfok.nodekindex_p22, xcol);
		Gemm(TRANSPOSE, NORMAL,
			Scalar(-1), nbinfok.Th2c2, (nbnodek->xS_)(nbinfok.nodekindex_p21, xcol),
			Scalar(1), copyvec);
		SubMatrixUpdate(nbnodek->xS_, nbinfok.nodekindex_p22, xcol, copyvec);
		copyvec.Empty();

		// Step 2.
		// xh1 = xh1 - (Ac1c1^{-1} * Ah1c1^{T})^{T} * xc1.
		copyvec = xS_(nbinfok.myindex_p11, xcol);
		Gemm(TRANSPOSE, NORMAL,
			Scalar(-1), nbinfok.Ac1c1invAc1h1, xS_(nbinfok.myindex_p12, xcol),
			Scalar(1), copyvec);
		SubMatrixUpdate(xS_, nbinfok.myindex_p11, xcol, copyvec);
		copyvec.Empty();
		// xc2 = xc2 - (Ac1c1^{-1} * Ac2c1^{T})^{T} * xc1.
		copyvec = (nbnodek->xS_)(nbinfok.nodekindex_p22, xcol);
		Gemm(TRANSPOSE, NORMAL,
			Scalar(-1), nbinfok.Ac1c1invAc1c2, xS_(nbinfok.myindex_p12, xcol),
			Scalar(1), copyvec);
		SubMatrixUpdate(nbnodek->xS_, nbinfok.nodekindex_p22, xcol, copyvec);
		copyvec.Empty();
		// xh2 = xh2 - (Ac1c1^{-1} * Ah2c1^{T})^{T} * xc1.
		copyvec = (nbnodek->xS_)(nbinfok.nodekindex_p21, xcol);
		Gemm(TRANSPOSE, NORMAL,
			Scalar(-1), nbinfok.Ac1c1invAc1h2, xS_(nbinfok.myindex_p12, xcol),
			Scalar(1), copyvec);
		SubMatrixUpdate(nbnodek->xS_, nbinfok.nodekindex_p21, xcol, copyvec);
		copyvec.Empty();
		// xc1 = Lc1^{-1} * xc1.
		copyvec = xS_(nbinfok.myindex_p12, xcol);
		Trmm(ELLR::LEFT, LOWER, NORMAL, UNIT, Scalar(1), nbinfok.Ac1c1inv, copyvec);
		SubMatrixUpdate(xS_, nbinfok.myindex_p12, xcol, copyvec);
		copyvec.Empty();

		// Step 3.
		// xh1 = xh1 - (Ac2c2^{-1} * Ac2h1)^{T} * xc2.
		copyvec = xS_(nbinfok.myindex_p11, xcol);
		Gemm(TRANSPOSE, NORMAL,
			Scalar(-1), nbinfok.Ac2c2invAc2h1, (nbnodek->xS_)(nbinfok.nodekindex_p22, xcol),
			Scalar(1), copyvec);
		SubMatrixUpdate(xS_, nbinfok.myindex_p11, xcol, copyvec);
		copyvec.Empty();
		// xh2 = xh2 - (Ac2c2^{-1} * Ah2c2^{T})^{T} * xc2.
		copyvec = (nbnodek->xS_)(nbinfok.nodekindex_p21, xcol);
		Gemm(TRANSPOSE, NORMAL,
			Scalar(-1), nbinfok.Ac2c2invAc2h2, (nbnodek->xS_)(nbinfok.nodekindex_p22, xcol),
			Scalar(1), copyvec);
		SubMatrixUpdate(nbnodek->xS_, nbinfok.nodekindex_p21, xcol, copyvec);
		copyvec.Empty();
		// xc2 = Lc2^{-1} * xc2.
		copyvec = (nbnodek->xS_)(nbinfok.nodekindex_p22, xcol);
		Trmm(ELLR::LEFT, LOWER, NORMAL, UNIT, Scalar(1), nbinfok.Ac2c2inv, copyvec);
		SubMatrixUpdate(nbnodek->xS_, nbinfok.myindex_p22, xcol, copyvec);
		copyvec.Empty();

		// xc1 = Dc1^{-1} * xc1. xc2 = Dc2^{-1} * xc2. We only apply D once.
		MatrixS D1 = GetDiagonal(nbinfok.Ac1c1inv);
		copyvec = xS_(nbinfok.myindex_p12, xcol);
		DiagonalScale(ELLR::LEFT, NORMAL, D1, copyvec);
		SubMatrixUpdate(xS_, nbinfok.myindex_p12, xcol, copyvec);
		copyvec.Empty();
		MatrixS D2 = GetDiagonal(nbinfok.Ac2c2inv);
		copyvec = (nbnodek->xS_)(nbinfok.nodekindex_p22, xcol);
		DiagonalScale(ELLR::LEFT, NORMAL, D2, copyvec);
		SubMatrixUpdate(nbnodek->xS_, nbinfok.nodekindex_p22, xcol, copyvec);
		copyvec.Empty();
	}
}

} // namespace HIF.