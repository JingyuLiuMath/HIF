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

}