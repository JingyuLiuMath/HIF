#include "HIF.hpp"

namespace HIF {

// The inverse of A by LDL, stored in A.
template <typename Scalar>
void LDLSolve(Matrix<Scalar>& A)
{
    DEBUG_HIF(CallStackEntry cse("LDLSolve"))

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

// X = A^{-1} * X where A^{-1} is stored in Ainv via its LDL factorization.
template <typename Scalar>
void MultiplySolve(Matrix<Scalar>& Ainv, Matrix<Scalar>& X)
{
    DEBUG_HIF(CallStackEntry cse("MultiplySolve"))

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
    DEBUG_HIF(CallStackEntry cse("IDSolve"))

    Permutation P;
    ID(skelmtx, P, T, ctrl);
    int k = T.Height(); // rank.
    p1.resize(T.Height());
    p2.resize(T.Width());
    Matrix<int> pmat(1, skelmtx.Width());
    for (int i = 0; i < pmat.Width(); i++)
    {
        pmat.Set(0, i, i);
    }
    P.PermuteCols(pmat);
    Matrix<int> p1mat(1, T.Height());
    Matrix<int> p2mat(1, T.Width());
    for (int i = 0; i < pmat.Width(); i++)
    {
        if (i < k)
        {
            p1mat.Set(0, i, pmat.Get(0, i));
        }
        else
        {
            p2mat.Set(0, i - k, pmat.Get(0, i));
        }
    }
    Permutation P1;
    Permutation P2;
    El::SortingPermutation(p1mat, P1);
    El::SortingPermutation(p2mat, P2);
    P1.PermuteCols(p1mat);
    P2.PermuteCols(p2mat);
    P1.PermuteRows(T);
    P2.PermuteCols(T);
    for (int i = 0; i < p1mat.Width(); i++)
    {
        p1[i] = p1mat.Get(0, i);
    }
    for (int i = 0; i < p2mat.Width(); i++)
    {
        p2[i] = p2mat.Get(0, i);
    }
    // skelmtx(:, p2) = skelmtx(:, p1) * T where p1 and p2 are sorted.
}

// A(rowindex, colindex) = newsubA where rowindex and colindex are sorted.
template <typename Scalar>
void SubMatrixUpdate(Matrix<Scalar>& A, const vector<int>& rowindex, const vector<int>& colindex,
    const Matrix<Scalar>& newsubA)
{
    DEBUG_HIF(CallStackEntry cse("SubMatrixUpdate"))

    for (int i = 0; i < rowindex.size(); i++)
    {
        for (int j = 0; j < colindex.size(); j++)
        {
            A.Set(rowindex[i], colindex[j], newsubA.Get(i, j));
        }
    }
}

// subA = A(rowindex, colindex) where rowindex and colindex are sorted.
template <typename Scalar>
void SubMatrixEqual(Matrix<Scalar>& subA, 
    const Matrix<Scalar>& A, const vector<int>& rowindex, const vector<int>& colindex)
{
    DEBUG_HIF(CallStackEntry cse("SubMatrixEqual"))

    for (int i = 0; i < rowindex.size(); i++)
    {
        for (int j = 0; j < colindex.size(); j++)
        {
            subA.Set(i, j, A.Get(rowindex[i], colindex[j]));
        }
    }
}

// vec = start:end-1.
void RangeVec(int start, int end, vector<int>& vec)
{
    DEBUG_HIF(CallStackEntry cse("RangeVec"))

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
    DEBUG_HIF(CallStackEntry cse("FullMat"))

    El::Zeros(A, sparseA.Height(), sparseA.Width());
    for (int i = 0; i < A.Height(); i++)
    {
        for (int j = 0; j < A.Width(); j++)
        {
            A.Set(i, j, sparseA.Get(i, j));
        }
    }
}

// A = full(sparseA(rowindex, colindex)) where rowindex and colindex are sorted.
template <typename Scalar>
void FullMat(const SparseMatrix<Scalar>& sparseA, const vector<int>& rowindex, const vector<int>& colindex,
    Matrix<Scalar>& A)
{
    DEBUG_HIF(CallStackEntry cse("FullMat"))

    El::Zeros(A, rowindex.size(), colindex.size());
    for (int i = 0; i < rowindex.size(); i++)
    {
        for (int j = 0; j < colindex.size(); j++)
        {
            A.Set(i, j, sparseA.Get(rowindex[i], colindex[j]));
        }
    }
}

// Show matrix.
template <typename Scalar>
void ShowMatrix(const Matrix<Scalar>& A, const string discription)
{
    DEBUG_HIF(CallStackEntry cse("HIFGraph:ShowMatrix"))

    std::cout << discription << std::endl;
    std::cout << "Height " << A.Height() << std::endl;
    std::cout << "Width " << A.Width() << std::endl;
    std::cout << " [ " << std::endl;
    for (int i = 0; i < A.Height(); i++)
    {
        for (int j = 0; j < A.Width(); j++)
        {
            std::cout << A.Get(i, j) << ",";
        }
        std::cout << ";" << std::endl;
    }
    std::cout << " ] " << std::endl;
}

// Show vector.
void ShowVector(const vector<int>& v, const string discription)
{
    DEBUG_HIF(CallStackEntry cse("HIFGraph:ShowVector"))

    std::cout << discription << std::endl;
    std::cout << " Size " << v.size() << std::endl;
    std::cout << " [ " << std::endl;
    for (int i = 0; i < v.size(); i++)
    {
        std::cout << v[i] << ";" << std::endl;
    }
    std::cout << " ] " << std::endl;
}

#define PROTOTYPE(Scalar) \
template void LDLSolve(Matrix<Scalar>& A); \
template void LDLSolve(Matrix<Scalar>& A, Matrix<Scalar>& Ainv); \
template void MultiplySolve(Matrix<Scalar>& A, Matrix<Scalar>& X); \
template void IDSolve(Matrix<Scalar>& skelmtx, Matrix<Scalar>& T, \
    vector<int>& p1, vector<int>& p2, const QRCtrl<Base<Scalar>>& ctrl); \
template void SubMatrixUpdate(Matrix<Scalar>& A, const vector<int>& rowindex, const vector<int>& colindex, \
    const Matrix<Scalar>& newsubA); \
template void SubMatrixEqual(Matrix<Scalar>& subA, \
    const Matrix<Scalar>& A, const vector<int>& rowindex, const vector<int>& colindex); \
template void FullMat(const SparseMatrix<Scalar>& sparseA, Matrix<Scalar>& A); \
template void FullMat(const SparseMatrix<Scalar>& sparseA, const vector<int>& rowindex, const vector<int>& colindex, \
    Matrix<Scalar>& A); \
template void ShowMatrix(const Matrix<Scalar>& A, const string discription);

PROTOTYPE(float)
PROTOTYPE(double)

} // namespace HIF.
