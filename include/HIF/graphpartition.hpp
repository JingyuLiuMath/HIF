#pragma once
#ifndef GRAPHPARTITION_HPP
#define GRAPHPARTITION_HPP 1

#include "HIF.hpp"

#include "metis.h"
#include "/opt/local/metis/5.1.0/libmetis/*.h"

namespace HIF{

template <typename Scalar>
void GraphPart(const SparseMatrix<Scalar>& A,
    vector<int>& p1, vector<int>& p2,
    vector<int>& sep1, vector<int>& sep2);

template <typename Scalar>
void MetisPart(const SparseMatrix<Scalar>& A,
    vector<int>& p1, vector<int>& p2, vector<int>& sep);

template <typename Scalar>
void MetisSepPart(const SparseMatrix<Scalar>& A,
    vector<int>& p1, vector<int>& p2, vector<int>& sep);

void Accumarray(const vector<int>& vec, vector<int>& count);

void Cumsum(const vector<int>& vec, vector<int>& a);

} // namespace HIF.

#endif //METISMEX_HPP
