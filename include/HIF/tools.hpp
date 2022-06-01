#pragma once
#ifndef TOOLS_HPP
#define TOOLS_HPP 1

#include "HIF.hpp"

namespace HIF {

template <typename Scalar>
void LDLSolve(Matrix<Scalar>& A);

template <typename Scalar>
void LDLSolve(Matrix<Scalar>& A, Matrix<Scalar>& Ainv);

template <typename Scalar>
void MultiplySolve(Matrix<Scalar>& Ainv, Matrix<Scalar>& X);

template <typename Scalar>
void IDSolve(Matrix<Scalar>& skelmtx, Matrix<Scalar>& T,
    vector<int>& p1, vector<int>& p2, const QRCtrl<Base<Scalar>>& ctrl);

template <typename Scalar>
void SubMatrixUpdate(Matrix<Scalar>& A, const vector<int>& rowindex, const vector<int>& colindex,
    Matrix<Scalar>& newsubA);

void RangeVec(int start, int end, vector<int>& vec);

template <typename Scalar>
void FullMat(const SparseMatrix<Scalar>& sparseA, Matrix<Scalar>& A);

} // namespace HIF.

#endif // ifndef HIF_TOOLS_HPP
