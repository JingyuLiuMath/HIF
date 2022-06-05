#pragma once
#ifndef GRAPHPARTITION_HPP
#define GRAPHPARTITION_HPP 1

#include "HIF.hpp"

namespace HIF{

extern "C"
{
    // void gk_free(void**, ...);
    ctrl_t* libmetis__SetupCtrl(moptype_et, int*, int, int, float*, float*);
    // void MlevelNodeBisectionMultiple(ctrl_t*, graph_t*);
}

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
