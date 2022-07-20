#pragma once
#ifndef HIFGRAPH_HPP
#define HIFGRAPH_HPP 1

#include "HIF.hpp"

namespace HIF {

template <typename Scalar>
class HIFGraph
{
public:
    typedef Matrix<Scalar> MatrixS;
    typedef SparseMatrix<Scalar> SparseMatrixS;

    // Initialization.
    HIFGraph(const SparseMatrixS& A, int minvtx);
    HIFGraph(int level, int seqnum,
        const vector<int>& vtx, const vector<int>& sep, const vector<int>& nb);
    ~HIFGraph();

    // Factorization.
    void Factorization();

    // Application.
    void Apply(MatrixS& b);

private:

    struct SkelInfo
    {
        int skip;

        MatrixS Th1c1;
        vector<int> myindex_p11;
        vector<int> myindex_p12;
        vector<int> nodekindex_p11;
        vector<int> nodekindex_p12;

        MatrixS Th2c2;
        vector<int> myindex_p21;
        vector<int> myindex_p22;
        vector<int> nodekindex_p21;
        vector<int> nodekindex_p22;

        MatrixS Ac1c1inv;
        MatrixS Ac1c1invAc1h1;
        MatrixS Ac1c1invAc1c2;
        MatrixS Ac1c1invAc1h2;

        MatrixS Ac2c2inv;
        MatrixS Ac2c2invAc2h1;
        MatrixS Ac2c2invAc2h2;
    }; // Skeletonization information.

    struct IndexInfo
    {
        vector<int> myindex_intr;
        vector<int> cindex_intr;
        vector<int> myindex_sep;
        vector<int> cindex_sep;
    }; // Index information, used for merge, applymerge, applysplit. `c` denotes `child`.

    struct ChildNode
    {
        vector<HIFGraph*> nodevec;

        ChildNode()
        {
            nodevec.resize(2);
        }
        ~ChildNode()
        {
            for (int t = 0; t < nodevec.size(); t++)
            {
                delete nodevec[t];
            }
            nodevec.resize(0);
        }

        HIFGraph& Child(int t)
        {
            return *nodevec[t];
        }
        const HIFGraph& Child(int t) const
        {
            return *nodevec[t];
        }

        HIFGraph* PtChild(int t)
        {
            return nodevec[t];
        }
        const HIFGraph* PtChild(int t) const
        {
            return nodevec[t];
        }

    }; // children node.

    // Graph data.
    vector<int> vtx_; // Vertices.
    vector<int> intr_; // Interior vertices.
    vector<int> sep_; // Separator vertices.
    vector<int> nb_; // Neighbor vertices.
    vector<int> sk_; // Skeleton sep. We also use hat (h) to represent it.
    vector<int> re_; // Redundant sep. We also use check (c) to represent it.
    vector<int> nbsk_; // Skeleton nb.
    vector<int> nbre_; // Redundant nb.
    vector<vector<int>> singlesep_; // Sep which only interact with one node.
    vector<int> complexsep_; // Sep which interact with more than one node.

    // Tree data.
    int numlevels_ = 0; // The highest level in current subtree.
    int level_; // Current level, start from 0.
    int seqnum_; // A node's order in its level.
    int endflag_ = 0; // Whether the partition ends.
    ChildNode childrennode_; // Children nodes.
    // vector<HIFGraph*> children_; // Children nodes.
    vector<HIFGraph*> nbnode_; // Neighbor nodes.
    vector<int> nbnodeseqnum_; // Neighbor nodes' seqnum.
    vector<int> nbnodelevel_; // Neighbor nodes' level.
    vector<SkelInfo> nbinfo_; // Information between a node and its nbnode after skeletonization.
    vector<IndexInfo> childreninfo_; // Index information of a node and its children.
    
    // Matrices data.
    // For the following matrices, the fist index is row, and the second index is col.
    MatrixS AII_; // Interaction between int and int.
    MatrixS ASI_; // Interaction between sep and int.
    MatrixS ASS_; // Interaction between sep and sep.
    MatrixS ANS_; // Interaction between nb and sep.
    MatrixS AIIinv_; // AII^{-1} which is obtained by the LDL factorization of AII.
    MatrixS AIIinvAIS_; // AIIinvAIS = AII^ { -1 } *ASI^ { T }.

    // Vectors data.
    // We deal a matrix by its columns.
    MatrixS xI_; // The intr part of a vector x.
    MatrixS xS_; // The sep part of a vector x.

    // Initialization functions.
    void BuildTree(const SparseMatrixS& A, int minvtx);
    void PassSeparatorNeighbor(const SparseMatrixS& A);
    void SetNeighborNode();  
    void FillTree(const SparseMatrixS& A);

    void SetSeparatorType();
    
    // Factorization functions.
    void RecursiveSparseElim(int whatlevel);
    void SparseElim();

    void RecursiveSkel(int whatlevel);
    void Skel();
    void NoSkel();

    void RecursiveMerge(int whatlevel);
    void Merge();
    void RecursiveSetSeparatorType(int whatlevel);
    void FactorClear();

    void RootFactorization();

    // Apply functions.
    void FillVecTree(const MatrixS& b, const vector<int>& xcol);

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

    void GetSolution(MatrixS& b, const vector<int>& xcol);

    // Debug functions.
    void ShowPartition();
};

} // namespace HIF.

#endif // ifndef HIFGRAPH_HPP
