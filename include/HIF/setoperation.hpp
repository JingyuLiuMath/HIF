#pragma once
#ifndef SETOP_HPP
#define SETOP_HPP 1

#include "HIF.hpp"

namespace HIF {

// ---------------------------------------Find---------------------------------------
template <typename Scalar>
int FindFirstIndex(const vector<Scalar>& vec, Scalar value);

template <typename Scalar>
void FindAllIndex(const vector<Scalar>& vec, Scalar value, vector<int>& index);

template <typename Scalar>
void FindAllIndex_Sort(const vector<Scalar>& vec1, const vector<Scalar>& vec2, vector<int>& index);

template <typename Scalar>
void FindEqualIndex(const vector<Scalar>& vec1, const vector<Scalar>& vec2, vector<int>& index);

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

// ---------------------------------------Unique---------------------------------------
template <typename Scalar>
void Unique_Sort(vector<Scalar>& vec);

} // namespace HIF.

#endif // ifndef HIF_SETOP_HPP