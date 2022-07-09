#pragma once
#ifndef SETOP_HPP
#define SETOP_HPP 1

#include "HIF.hpp"

namespace HIF {

// ---------------------------------------Find---------------------------------------
template <typename Scalar>
int FindIndex_Sort(const vector<Scalar>& vec, const Scalar value);

template <typename Scalar>
void FindAllIndex(const vector<Scalar>& vec, const Scalar value, vector<int>& index);

template <typename Scalar>
void FindAllIndex_Sort(const vector<Scalar>& vec1, const vector<Scalar>& vec2, vector<int>& index);

// ---------------------------------------Intersect---------------------------------------
template <typename Scalar>
bool Intersect_Sort(const vector<Scalar>& vec1, const vector<Scalar>& vec2);

template <typename Scalar>
void Intersect_Sort(const vector<Scalar>& vec1, const vector<Scalar>& vec2,
	vector<int>& index1, vector<int>& index2);

template <typename Scalar>
void Intersect_Sort(const vector<Scalar>& vec1, const vector<Scalar>& vec2,
	vector<int>& index, int who);

template <typename Scalar>
void Intersect_Sort(const vector<Scalar>& vec1, const vector<Scalar>& vec2, vector<Scalar>& vec);

template <typename Scalar>
void Intersect_Sort(const vector<Scalar>& vec1, const vector<Scalar>& vec2,
	vector<Scalar>& vec, vector<int>& index, int who);

// ---------------------------------------Diff---------------------------------------
template <typename Scalar>
void Diff_Sort(const vector<Scalar>& vec1, const vector<Scalar>& vec2, vector<Scalar>& vec);

// ---------------------------------------AddElement---------------------------------------
template <typename Scalar>
void AddElement_Sort(vector<Scalar>& vec, Scalar value);

template <typename Scalar>
void AddVec_Sort(vector<Scalar>& vec1, const vector<Scalar>& vec2);

} // namespace HIF.

#endif // ifndef HIF_SETOP_HPP
