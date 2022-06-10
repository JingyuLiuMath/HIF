#pragma once
#ifndef HIFGRAPH_HPP
#define HIFGRAPH_HPP 1

#include "HIF.hpp"

namespace HIF {

template <typename Scalar>
struct SkelInfo
{
    int skip;

    Matrix<Scalar> Th1c1;
    vector<int> myindex_p11;
    vector<int> myindex_p12;
    vector<int> nodekindex_p11;
    vector<int> nodekindex_p12;

    Matrix<Scalar> Th2c2;
    vector<int> myindex_p21;
    vector<int> myindex_p22;
    vector<int> nodekindex_p21;
    vector<int> nodekindex_p22;

    Matrix<Scalar> Ac1c1inv;
    Matrix<Scalar> Ac1c1invAc1h1;
    Matrix<Scalar> Ac1c1invAc1c2;
    Matrix<Scalar> Ac1c1invAc1h2;

    Matrix<Scalar> Ac2c2inv;
    Matrix<Scalar> Ac2c2invAc2h1;
    Matrix<Scalar> Ac2c2invAc2h2;
};

struct IndexInfo
{
    vector<int> myindex_intr;
    vector<int> cindex_intr;
    vector<int> myindex_sep;
    vector<int> cindex_sep;
};

template <typename Scalar>
class HIFGraph
{
public:

    typedef Matrix<Scalar> MatrixS;
    typedef SparseMatrix<Scalar> SparseMatrixS;

    // Initialization.
    HIFGraph(const SparseMatrix<Scalar>& A, int minvtx);
    HIFGraph(int level, int seqnum,
        vector<int>& vtx, vector<int>& sep, vector<int>& nb);
    ~HIFGraph();

    // Factorization.
    void Factorization(double tol = 1e-3);

    // Application.
    void Apply(Matrix<Scalar>& b);

private:    

    // Graph data.
    vector<int> vtx_; // Vertices.
    vector<int> intr_; // Interior vertices.
    vector<int> sep_; // Separator vertices.
    vector<int> nb_; // Neighbor vertices.
    vector<int> re_; // Redundant sep. We also use check (c) to represent it.
    vector<int> sk_; // Skeleton sep. We also use hat (h) to represent it.
    vector<int> nbre_; // Redundant nb.
    vector<vector<int>> singlesep_; // Sep which only interact with one node.
    vector<int> complexsep_; // Sep which interact with more than one nodes.

    // Tree data.
    int numlevels_ = 0; // Total number of levels.
    int level_; // Current level, start from 0.
    int seqnum_; // A node's order in its level.
    int endflag_ = 0; // Whether the partition ends.
    vector<HIFGraph*> children_; // Children nodes.
    vector<HIFGraph*> nbnode_; // Neighbor nodes.
    vector<int> nbnodeseqnum_; // Neighbor nodes' seqnum.
    vector<int> nbnodelevel_; // Neighbor nodes' level.
    vector<SkelInfo<Scalar>> nbinfo_; // Information between a node and its nbNode when skeletonization.
    vector<IndexInfo> childreninfo_; // Index information when merge and split.
    
    // Matrices data.
    // For the following matrices, the fist index is row, and the second index is col.
    MatrixS AII_; // Interaction between int and int.
    MatrixS ASI_; // Interaction between sepand int.
    MatrixS ASS_; // Interaction between sepand sep.
    MatrixS ANS_; // Interaction between nband sep.
    MatrixS AIIinv_; // The LDL factorization about AII.
    MatrixS AIIinvAIS_; // AIIinvAIS = AII^ { -1 } *ASI^ { T }.

    // Vectors data.
    // We deal a matrix by its columns.
    MatrixS xI_; // The intr part of a vector x.
    MatrixS xS_; // The sep part of a vector x.

    // Initialization functions.
    void BuildTree(const SparseMatrix<Scalar>& A, int minvtx);
    void PassSeparatorNeighbor(const SparseMatrix<Scalar>& A);
    void SetNeighborNode();  
    void FillTree(const SparseMatrix<Scalar>& A);

    void SetSeparatorType();
    
    // Factorization functions.
    void RecursiveSparseElim(int whatlevel);
    void SparseElim();

    void RecursiveSkel(int whatlevel, double tol);
    void Skel(double tol);
    void NoSkel();

    void RecursiveMerge(int whatlevel);
    void Merge();
    void FactorClear();

    void RootFactorization();

    // Apply functions.
    void FillVecTree(const Matrix<Scalar>& b, const vector<int>& xcol);

    void RecursiveApplySparseElimUp(int whatlevel);
    void ApplySparseElimUp();

    void RecursiveApplySkelUp(int whatlevel, const vector<int>& xcol);
    void ApplySkelUp(const vector<int>& xcol);

    void RecursiveApplyMerge(int whatlevel, const vector<int>& xcol);
    void ApplyMerge(const vector<int>& xcol);

    void RootApply();

    void RecursiveApplySplit(int whatlevel, const vector<int>& xcol);
    void ApplySplit(const vector<int>& xcol);

    void RecursiveApplySkelDown(int whatlevel, const vector<int>& xcol);
    void ApplySkelDown(const vector<int>& xcol);

    void RecursiveApplySparseElimDown(int whatlevel);
    void ApplySparseElimDown();

    void GetSolution(Matrix<Scalar>& b, const vector<int>& xcol);

    // Debug functions.
    void ShowPartition();
    void ShowMatrix(const MatrixS& A);
    void ShowVector(const vector<int>& v);
    void GiveMeA(MatrixS& myA);
};

} // namespace HIF.

#endif // ifndef HIF_GRAPH_HPP
