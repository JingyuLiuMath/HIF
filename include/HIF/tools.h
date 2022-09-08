#ifndef TOOLS_H
#define TOOLS_H

#include "HIF.h"

namespace HIF {
template <typename Scalar> void LDLSolve(Matrix<Scalar> &A);

template <typename Scalar>
void LDLSolve(Matrix<Scalar> &A, Matrix<Scalar> &Ainv);

template <typename Scalar>
void MultiplySolve(Matrix<Scalar> &Ainv, Matrix<Scalar> &X);

template <typename Scalar>
void IDSolve(Matrix<Scalar> &skelmtx, Matrix<Scalar> &T, vector<int> &p1,
             vector<int> &p2, const QRCtrl<Base<Scalar>> &ctrl);

template <typename Scalar>
void SubMatrixUpdate(Matrix<Scalar> &A, const vector<int> &rowindex,
                     const vector<int> &colindex,
                     const Matrix<Scalar> &newsubA);

template <typename Scalar>
void SubMatrixTransposeUpdate(Matrix<Scalar> &A, const vector<int> &rowindex,
                              const vector<int> &colindex,
                              const Matrix<Scalar> &newsubA);

template <typename Scalar>
void SubMatrixEqual(Matrix<Scalar> &subA, const Matrix<Scalar> &A,
                    const vector<int> &rowindex, const vector<int> &colindex);

void RangeVec(int start, int end, vector<int> &vec);

template <typename Scalar>
void FullMat(const SparseMatrix<Scalar> &sparseA, Matrix<Scalar> &A);

template <typename Scalar>
void FullMat(const SparseMatrix<Scalar> &sparseA, const vector<int> &rowindex,
             const vector<int> &colindex, Matrix<Scalar> &A);

template <typename Scalar>
void ShowMatrix(const Matrix<Scalar> &A, const string discription);

void ShowVector(const vector<int> &v, const string discription);
} // namespace HIF.

#endif // ifndef TOOLS_H
