#include "HIF.hpp"

namespace HIF {

// Graph partition.
template <typename Scalar>
void GraphPart(const SparseMatrix<Scalar>& A, 
    vector<int>& p1, vector<int>& p2, 
    vector<int>& sep1, vector<int>& sep2)
{
    MetisPart(A, p1, p2, sep1);
    // p1 = p1 + sep, p2 = p2, sep1 = sep, sep2 need to be assigned.
    p1.insert(p1.end(), sep1.begin(), sep1.end());
    sort(p1.begin(), p1.end());
    sort(sep1.begin(), sep1.end());
    sort(p2.begin(), p2.end());
    for (int i = 0; i < sep1.size(); i++)
    {
        int sep1i = sep1[i];
        vector<int> sep2i_tmp;
        for (int col = 0; col < A.Width(); col++)
        {
            if (A.Get(sep1i, col) != 0)
            {
                sep2i_tmp.push_back(col);
            }
        }
        if (sep2i_tmp.size() > 0)
        {
            vector<int> sep2i;
            Intersect_Sort(sep2i_tmp, p2, sep2i);
            sep2.insert(sep2.end(), sep2i.begin(), sep2i.end());
            sort(sep2.begin(), sep2.end());
            Unique_Sort(sep2);
        }
    }
}

// Metis partition.
template <typename Scalar>
void MetisPart(const SparseMatrix<Scalar>& A,
    vector<int>& p1, vector<int>& p2, vector<int>& sep)
{
    // degree = sum((spones(nvtxs) - speye(size(nvtxs))) > 0);
    // singleidx = find(degree == 0);
    // idx = find(degree > 0);
    SparseMatrix<int> B;
    B.Resize(A.Height(), A.Width());
    const int* sourceA = A.LockedSourceBuffer();
    const int* targetA = A.LockedTargetBuffer();
    int nnzA = A.NumEntries();
    for (int t = 0; t < nnzA; t++)
    {
        B.QueueUpdate(sourceA[t], targetA[t], int(1));
    }
    B.ProcessQueues();
    for (int t = 0; t < B.Height(); t++)
    {
        B.QueueUpdate(t, t, -1);
    }
    B.ProcessQueues();
    for (int t = 0; t < nnzA; t++)
    {
        if (B.Get(sourceA[t], targetA[t]) != 0)
        {
            B.Set(sourceA[t], targetA[t], int(1));
        }
    }
    vector<int> degree(B.Width(), 0);
    for (int t = 0; t < nnzA; t++)
    {
        if (B.Get(sourceA[t], targetA[t]) != 0)
        {
            degree[targetA[t]] += 1;
        }
    }
    vector<int> singleidx;
    vector<int> idx;
    for (int i = 0; i < degree.size(); i++)
    {
        if (degree[i] == 0)
        {
            singleidx.push_back(i);
        }
        else
        {
            idx.push_back(i);
        }
    }
    SparseMatrix<Scalar> A1 = A(idx, idx);
    vector<int> lidx, ridx, sepidx;
    MetisSepPart(A1, lidx, ridx, sepidx);
    p1.resize(lidx.size());
    p2.resize(ridx.size());
    sep.resize(sepidx.size());
    for (int i = 0; i < p1.size(); i++)
    {
        p1[i] = idx[lidx[i]];
    }
    for (int i = 0; i < p2.size(); i++)
    {
        p2[i] = idx[ridx[i]];
    }
    for (int i = 0; i < sep.size(); i++)
    {
        sep[i] = idx[sepidx[i]];
    }
    sep.insert(sep.end(), singleidx.begin(), singleidx.end());
}

// Metis separator partition.
template <typename Scalar>
void MetisSepPart(const SparseMatrix<Scalar>& A, 
    vector<int>& p1, vector<int>& p2, vector<int>& sep)
{
    // nvtxs.
    idx_t nvtxs = A.Height();
    // xadj.
    const int* sourceA = A.LockedSourceBuffer();
    const int* targetA = A.LockedTargetBuffer();
    int nnzA = A.NumEntries();
    idx_t* xadj;
    vector<int> rowindex(nnzA, 0);
    vector<int> colindex(nnzA, 0);
    for (int t = 0; t < nnzA; t++)
    {
        rowindex[t] = targetA[t];
        colindex[t] = sourceA[t];
    }
    vector<int> rowindex_copy(rowindex);
    vector<int> colindex_copy(colindex);
    rowindex.clear();
    colindex.clear();
    for (int k = 0; k < rowindex_copy.size(); k++)
    {
        if (rowindex_copy[k] != colindex_copy[k])
        {
            rowindex.push_back(rowindex_copy[k]);
            colindex.push_back(colindex_copy[k]);
        }
    }
    if (colindex.size() == 0)
    {
        xadj = NULL;
    }
    else
    {
        vector<int> cumsum_accumj;
        Accumarray(colindex, cumsum_accumj);
        Cumsum(cumsum_accumj);
        xadj = new idx_t[cumsum_accumj.size() + 1];
        xadj[0] = 0;
        for (int i = 0; i < cumsum_accumj.size(); i++)
        {
            xadj[i + 1] = cumsum_accumj[i];
        }
    }
    // adjncy.
    if (rowindex.size() == 0)
    {
        RangeVec(0, nvtxs, sep);
        return;
    }
    idx_t* adjncy = new idx_t[rowindex.size()];
    for (int i = 0; i < rowindex.size(); i++)
    {
        adjncy[i] = rowindex[i];
    }

    idx_t* vwgt = NULL;
    idx_t options[METIS_NOPTIONS];
    METIS_SetDefaultOptions(options);
    
    // Metis main function
    idx_t i, nnvtxs = 0;
    idx_t ptlgraph, ptrgraph, ptsep;
    graph_t* graph = NULL;
    ctrl_t* ctrl;
    idx_t* piperm;
    idx_t snvtxs[3];
    idx_t* where;

    if (!gk_malloc_init())
    {
        switch (METIS_ERROR_MEMORY)
        {
        case METIS_OK:
            return;
        case METIS_ERROR_INPUT:
            std::cout << "Metis input error" << std::endl;
            break;
        case METIS_ERROR_MEMORY:
            std::cout << "Metis memory error" << std::endl;
            break;
        default:
            std::cout << "Unknown Metis error" << std::endl;
            break;
        }
    }

    // set up the run time parameters
    ctrl = SetupCtrl(METIS_OP_OMETIS, options, 1, 3, NULL, NULL);

    // prune the dense columns
    if (ctrl->pfactor > 0.0)
    {
        piperm = imalloc(nvtxs, "OMETIS: piperm");

        graph = PruneGraph(ctrl, nvtxs, xadj, adjncy, vwgt,
            piperm, ctrl->pfactor);
        if (graph == NULL)
        {
            // if there was no prunning, cleanup the pfactor
            gk_free((void**)&piperm, LTERM);
            ctrl->pfactor = 0.0;
        }
        else
        {
            nnvtxs = graph->nvtxs;
            // disable compression if prunning took place
            ctrl->compress = 0;
        }
    }

    // compress the graph
    if (ctrl->compress)
        ctrl->compress = 0;

    // if no prunning and no compression, setup the graph in the normal way.
    if (ctrl->pfactor == 0.0 && ctrl->compress == 0)
    {
        graph = SetupGraph(ctrl, nvtxs, 1, xadj, adjncy, vwgt, NULL, NULL);
    }
    ASSERT(CheckGraph(graph, ctrl->numflag, 1));

    
    // allocate workspace memory.
    AllocateWorkSpace(ctrl, graph);

    MlevelNodeBisectionMultiple(ctrl, graph);


    snvtxs[0] = 0;
    snvtxs[1] = 0;
    snvtxs[2] = 0;

    if (ctrl->pfactor > 0.0)
        snvtxs[2] += nvtxs - nnvtxs;

    where = graph->where;
    for (i = 0; i < graph->nvtxs; i++)
        snvtxs[where[i]]++;

    p1.resize(snvtxs[0]);
    p2.resize(snvtxs[1]);
    sep.resize(snvtxs[2]);

    ptlgraph = 0;
    ptrgraph = 0;
    ptsep = 0;

    if (ctrl->pfactor > 0.0)
    {
        for (i = 0; i < graph->nvtxs; i++)
            if (where[i] == 0)
                p1[ptlgraph++] = piperm[i];
            else if (where[i] == 1)
                p2[ptrgraph++] = piperm[i];
            else
                sep[ptsep++] = piperm[i];

        for (i = nnvtxs; i < nvtxs; i++)
            sep[ptsep++] = piperm[i];

        gk_free((void**)&piperm, LTERM);
    }
    else
    {
        for (i = 0; i < graph->nvtxs; i++)
            if (where[i] == 0)
                p1[ptlgraph++] = i;
            else if (where[i] == 1)
                p2[ptrgraph++] = i;
            else
                sep[ptsep++] = i;
    }

    /* clean up */
    FreeCtrl(&ctrl);
}

// count = accumarray(vec, 1).
void Accumarray(const vector<int>& vec, vector<int>& count)
{
    int maxvec = *(std::max_element(vec.begin(), vec.end()));
    count = vector<int> (maxvec + 1, 0);
    for (int i = 0; i < vec.size(); i++)
    {
        count[vec[i]] += 1;
    }
}

// a = cumsum(vec)
void Cumsum(vector<int>& vec)
{
    for (int i = 1; i < vec.size(); i++)
    {
        vec[i] += vec[i - 1];
    }
}

#define PROTOTYPE_PART(Scalar) \
template void GraphPart(const SparseMatrix<Scalar>& A, \
    vector<int>& p1, vector<int>& p2, \
    vector<int>& sep1, vector<int>& sep2); \
template void MetisPart(const SparseMatrix<Scalar>& A, \
    vector<int>& p1, vector<int>& p2, vector<int>& sep); \
template void MetisSepPart(const SparseMatrix<Scalar>& A, \ 
    vector<int>& p1, vector<int>& p2, vector<int>& sep);

PROTOTYPE_PART(float)
PROTOTYPE_PART(double)

} // namespace HIF.