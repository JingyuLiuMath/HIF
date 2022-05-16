#include "HIF.hpp"

namespace HIF {

// The inverse of A by LDL, stored in A.
template <typename Scalar>
void LDLSolve(Matrix<Scalar>& A)
{
    LDL(A, false);
    TriangularInverse(LOWER, UNIT, A);
    auto d = GetDiagonal(A);
    for (int i = 0; i < d.Height(); ++i)
    {
        for (int j = 0; j < d.Width(); ++j)
        {
            d.Set(i, j, Scalar(1) / d.Get(i, j));
        }
    }
    SetDiagonal(A, d);
}

// The inverse of A by LDL, stored in Ainv.
template <typename Scalar>
void LDLSolve(Matrix<Scalar>& A, Matrix<Scalar>& Ainv)
{
    DEBUG_HIF(CallStackEntry cse("LDLSolve"))
    Ainv = A;
    LDLSolve(Ainv);
}

// X = A^{-1} * X.
template <typename Scalar>
void MultiplySolve(Matrix<Scalar>& Ainv, Matrix<Scalar>& X)
{
    Trmm(ELLR::LEFT, LOWER, NORMAL, UNIT, Scalar(1), Ainv, X);

    auto d = GetDiagonal(Ainv);
    DiagonalScale(ELLR::LEFT, NORMAL, d, X);

    Trmm(ELLR::LEFT, LOWER, TRANSPOSE, UNIT, Scalar(1), Ainv, X);
}

// Skeletonization we need.
template <typename Scalar>
void IDSolve(Matrix<Scalar>& skelmtx, Matrix<Scalar>& T, 
    vector<int>& p1, vector<int>& p2, const QRCtrl<Base<F>>& ctrl)
{
    Permutation P;
    ID(skelmtx, P, T, ctrl);
    int k = T.Height();
    p1.resize(T.Height());
    p2.resize(T.Width());
    Matrix<int> pmat(1, skelmtx.Width())
    for (int i = 0; i < pmat.Width(); i++)
    {
        pmat.Set(0, i, i);
    }
    P.PermuteCols(pmat);
    for (int i = 0; i < pmat.Width(); i++)
    {
        if (i < k)
        {
            p1[i] = pmat.Get(0, i);
        }
        else
        {
            p2[i - k] = pmat.Get(0, i);
        }
    }
    // skelmtx(:, p2) = skelmtx(:, p1) * T.
}

}