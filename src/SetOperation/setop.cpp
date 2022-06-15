#include "HIF.hpp"

#include "./Diff.hpp"
#include "./Find.hpp"
#include "./Intersect.hpp"
#include "./Unique.hpp"

namespace HIF{

#define PROTOTYPE_SET(Scalar) \
template int FindFirstIndex(const vector<Scalar>& vec, const Scalar value); \
template void FindAllIndex(const vector<Scalar>& vec, const Scalar value, vector<int>& index); \
template void FindAllIndex_Sort(const vector<Scalar>& vec1, \
	const vector<Scalar>& vec2, vector<int>& index); \
template bool Intersect_Sort(const vector<Scalar>& vec1, const vector<Scalar>& vec2); \
template void Intersect_Sort(const vector<Scalar>& vec1, const vector<Scalar>& vec2, \
	vector<int>& index1, vector<int>& index2); \
template void Intersect_Sort(const vector<Scalar>& vec1, const vector<Scalar>& vec2, \
	vector<int>& index, int who); \
template void Intersect_Sort(const vector<Scalar>& vec1, const vector<Scalar>& vec2, \
	vector<Scalar>& vec); \
template void Intersect_Sort(const vector<Scalar>& vec1, const vector<Scalar>& vec2, \
	vector<Scalar>& vec, vector<int>& index, int who); \
template void Diff_Sort(const vector<Scalar>& vec1, const vector<Scalar>& vec2, \
	vector<Scalar>& vec); \
template void Unique_Sort(vector<Scalar>& vec);

PROTOTYPE_SET(int)
PROTOTYPE_SET(float)
PROTOTYPE_SET(double)

} // namespace HIF.
