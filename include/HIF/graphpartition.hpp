#pragma once
#ifndef GRAPHPARTITION_HPP
#define GRAPHPARTITION_HPP 1

#include "HIF.hpp"

namespace HIF{

template <typename Scalar>
void GraphPart(SparseMatrix<Scalar>& A,
    vector<int>& p1, vector<int>& p2,
    vector<int>& sep1, vector<int>& sep2);

template <typename Scalar>
void MetisPart(SparseMatrix<Scalar>& A,
    vector<int>& p1, vector<int>& p2, vector<int>& sep);

template <typename Scalar>
void MetisSepPart(SparseMatrix<Scalar>& A,
    vector<int>& p1, vector<int>& p2, vector<int>& sep);

void Accumarray(const vector<int>& vec, vector<int>& count);

void Cumsum(const vector<int>& vec, vector<int>& a);

} // namespace HIF.

#endif //METISMEX_HPP
