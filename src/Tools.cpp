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
    Ainv = A;
    LDLSolve(Ainv);
}

// X = A^{-1} * X where A^{-1} is stored in Ainv.
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
    vector<int>& p1, vector<int>& p2, const QRCtrl<Base<Scalar>>& ctrl)
{
    Permutation P;
    ID(skelmtx, P, T, ctrl);
    int k = T.Height();
    p1.resize(T.Height());
    p2.resize(T.Width());
    Matrix<int> pmat(1, skelmtx.Width());
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

// A(rowindex, colindex) = newsubA.
template <typename Scalar>
void SubMatrixUpdate(Matrix<Scalar>& A, const vector<int>& rowindex, const vector<int>& colindex,
    Matrix<Scalar>& newsubA)
{
    for (int i = 0; i < rowindex.size(); i++)
    {
        for (int j = 0; j < colindex.size(); j++)
        {
            A.Set(rowindex[i], colindex[j], newsubA.Get(i, j));
        }
    }
}

// vec = start:end-1.
void RangeVec(int start, int end, vector<int>& vec)
{
    vec.resize(end - start);
    for (int i = start; i < end; i++)
    {
        vec[i - start] = i;
    }
}

// Full mat.
template <typename Scalar>
void FullMat(const SparseMatrix<Scalar>& sparseA, Matrix<Scalar>& A)
{
    std::cout << sparseA.Height() << " " << sparseA.Width() << std::endl;
    std::cout << A.Height() << " " << A.Width() << std::endl;
    for (int i = 0; i < A.Height(); i++)
    {
        for (int j = 0; j < A.Width(); j++)
        {
            std::cout << " i " << i << " j " << j << std::endl;
            int offset = sparseA.Offset(i, j);
            std::cout << offset << std::endl;
            std::cout << sparseA.NumEntries() << std::endl;
            std::cout << sparseA.Row(offset) << std::endl;
            std::cout << sparseA.Col(offset) << std::endl;
            std::cout << sparseA.Size() << std::endl;
            std::cout << "sparseA.Get(i, j) " << sparseA.Get(i, j) << std::endl;
            A.Set(i, j, sparseA.Get(i, j));
        }
    }
    std::cout << "FullMat" << sparseA.Width() << std::endl;
}


#define PROTOTYPE(Scalar) \
template void LDLSolve( Matrix<Scalar>& A ); \
template void LDLSolve( Matrix<Scalar>& A, \
        Matrix<Scalar>& Ainv ); \
template void MultiplySolve( Matrix<Scalar>& A, \
        Matrix<Scalar>& X ); \
template void IDSolve(Matrix<Scalar>& skelmtx, \
    Matrix<Scalar>& T, \
    vector<int>& p1, \
    vector<int>& p2, \
    const QRCtrl<Base<Scalar>>& ctrl); \
template void SubMatrixUpdate(Matrix<Scalar>& A, \
    const vector<int>& rowindex, \
    const vector<int>& colindex, \
    Matrix<Scalar>& newsubA); \
template void FullMat(const SparseMatrix<Scalar>& sparseA, \
    Matrix<Scalar>& A);

PROTOTYPE(float)
PROTOTYPE(double)

} // namespace HIF.