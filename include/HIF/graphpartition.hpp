#ifndef GRAPHPARTITION_HPP
#define GRAPHPARTITION_HPP 1

namespace HIF{

template <typename Scalar>
void GraphPart(const SparseMatrix<Scalar>& A, 
	vector<int>& p1, vector<int>& p2, 
	vector<int>& sep1, vector<int>& sep2);

template <typename Scalar>
void MetisPart(const SparseMatrix<Scalar>& A, 
	vector<int>& p1, vector<int>& p2, vector<int>& sep);

template <typename Scalar>
void MetisGraph(const SparseMatrix<Scalar>& A, graph_t& g);

void Accumarray(const vector<int>& vec, vector<int>& count);

void Cumsum(const vector<int>& vec, vector<int>& a);

} // namespace HIF.

#endif //METISMEX_HPP
