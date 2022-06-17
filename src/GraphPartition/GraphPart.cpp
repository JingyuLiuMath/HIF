#include "HIF.hpp"

namespace HIF {

// Graph partition.
template <typename Scalar>
void GraphPart(const SparseMatrix<Scalar>& A, 
    vector<int>& p1, vector<int>& p2, 
    vector<int>& sep1, vector<int>& sep2)
{
    DEBUG_HIF(CallStackEntry cse("GraphPart"))

    MetisPart(A, p1, p2, sep1);
    // p1 = p1 + sep1, p2 = p2, sep2 need to be assigned.
    p1.insert(p1.end(), sep1.begin(), sep1.end());
    sort(p1.begin(), p1.end());
    sort(sep1.begin(), sep1.end());
    sort(p2.begin(), p2.end());
    for (int i = 0; i < sep1.size(); i++)
    {
        int sep1i = sep1[i];
        vector<int> sep2i_tmp(A.Width());
        int actualsize_sep2i_tmp = 0;
        // TODO: can be reduced...
        for (int col = 0; col < A.Width(); col++)
        {
            if (A.Get(sep1i, col) != Scalar(0))
            {
                sep2i_tmp[actualsize_sep2i_tmp] = col;
                actualsize_sep2i_tmp++;
            }
        }
        sep2i_tmp.erase(sep2i_tmp.begin() + actualsize_sep2i_tmp, sep2i_tmp.end());
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
    DEBUG_HIF(CallStackEntry cse("MetisPart"))

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
    vector<int> degree(B.Width(), 0);
    for (int t = 0; t < nnzA; t++)
    {
        if (B.Get(sourceA[t], targetA[t]) != 0)
        {
            degree[targetA[t]] += 1;
        }
    }
    vector<int> singleidx(degree.size());
    vector<int> idx(degree.size());
    int actualsize_singleidx = 0;
    int actualsize_idx = 0;
    for (int i = 0; i < degree.size(); i++)
    {
        if (degree[i] == 0)
        {
            singleidx[actualsize_singleidx] = i;
            actualsize_singleidx++;
        }
        else
        {
            idx[actualsize_idx] = i;
            actualsize_idx++;
        }
    }
    singleidx.erase(singleidx.begin() + actualsize_singleidx, singleidx.end());
    idx.erase(idx.begin() + actualsize_idx, idx.end());
    
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
    DEBUG_HIF(CallStackEntry cse("MetisSepPart"))

    // nvtxs.
    idx_t nvtxs = A.Height();
    // xadj.
    const int* sourceA = A.LockedSourceBuffer();
    const int* targetA = A.LockedTargetBuffer();
    const int* offsetA = A.LockedOffsetBuffer();
    int nnzA = A.NumEntries();
    /*idx_t* xadj = new idx_t[A.Height() + 1];
    std::cout << "xadj" << std::endl;
    for (int t = 0; t <= A.Height(); t++)
    {
        xadj[t] = offsetA[t];
        std::cout << xadj[t] << std::endl;
    }*/
    /*std::cout << "source" << std::endl;
    for (int t = 0; t < nnzA; t++)
    {
        std::cout << sourceA[t] << std::endl;
    }*/
    /*std::cout << "offset" << std::endl;
    for (int t = 0; t <= A.Height(); t++)
    {
        std::cout << offsetA[t] << std::endl;
    }*/
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
    int actualsize = 0;
    for (int k = 0; k < rowindex_copy.size(); k++)
    {
        if (rowindex_copy[k] != colindex_copy[k])
        {
            rowindex[actualsize] = rowindex_copy[k];
            colindex[actualsize] = colindex_copy[k];
            actualsize++;
        }
    }
    rowindex.erase(rowindex.begin() + actualsize, rowindex.end());
    colindex.erase(colindex.begin() + actualsize, colindex.end());
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
        /*std::cout << "xadj" << std::endl;
        std::cout << 0 << std::endl;*/
        xadj[0] = 0;
        for (int i = 0; i < cumsum_accumj.size(); i++)
        {
            xadj[i + 1] = cumsum_accumj[i];
            // std::cout << xadj[i+1] << std::endl;
        }
    }
    // adjncy.
    /*idx_t* adjncy = new idx_t[nnzA];
    std::cout << "adjncy" << std::endl;
    for (int t = 0; t < nnzA; t++)
    {
        adjncy[t] = targetA[t];
        std::cout << adjncy[t] << std::endl;
    }*/
    /*std::cout << "target" << std::endl;
    for (int t = 0; t < nnzA; t++)
    {
        std::cout << targetA[t] << std::endl;
    }*/
    if (rowindex.size() == 0)
    {
        RangeVec(0, nvtxs, sep);
        return;
    }
    idx_t* adjncy = new idx_t[rowindex.size()];
    // std::cout << "adjncy" << std::endl;
    for (int t = 0; t < rowindex.size(); t++)
    {
        adjncy[t] = rowindex[t];
        // std::cout << adjncy[t] << std::endl;
    }

    idx_t* vwgt = NULL;
    idx_t options[METIS_NOPTIONS];
    METIS_SetDefaultOptions(options);

    // Metis main function.
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
            std::cerr << "Metis input error" << std::endl;
            break;
        case METIS_ERROR_MEMORY:
            std::cerr << "Metis memory error" << std::endl;
            break;
        default:
            std::cerr << "Unknown Metis error" << std::endl;
            break;
        }
    }

    // Set up the run time parameters.
    ctrl = SetupCtrl(METIS_OP_OMETIS, options, 1, 3, NULL, NULL);

    // Prune the dense columns.
    if (ctrl->pfactor > 0.0)
    {
        piperm = imalloc(nvtxs, "OMETIS: piperm");

        graph = PruneGraph(ctrl, nvtxs, xadj, adjncy, vwgt,
            piperm, ctrl->pfactor);
        if (graph == NULL)
        {
            // If there was no prunning, cleanup the pfactor.
            gk_free((void**)&piperm, LTERM);
            ctrl->pfactor = 0.0;
        }
        else
        {
            nnvtxs = graph->nvtxs;
            // Disable compression if prunning took place.
            ctrl->compress = 0;
        }
    }

    // Compress the graph.
    if (ctrl->compress)
    {
        ctrl->compress = 0;
    }

    // If no prunning and no compression, setup the graph in the normal way.
    if (ctrl->pfactor == 0.0 && ctrl->compress == 0)
    {
        graph = SetupGraph(ctrl, nvtxs, 1, xadj, adjncy, vwgt, NULL, NULL);
    }
    ASSERT(CheckGraph(graph, ctrl->numflag, 1));

    
    // Allocate workspace memory.
    AllocateWorkSpace(ctrl, graph);

    MlevelNodeBisectionMultiple(ctrl, graph);

    snvtxs[0] = 0;
    snvtxs[1] = 0;
    snvtxs[2] = 0;

    if (ctrl->pfactor > 0.0)
    {
        snvtxs[2] += nvtxs - nnvtxs;
    }

    where = graph->where;
    for (i = 0; i < graph->nvtxs; i++)
    {
        snvtxs[where[i]]++;
    }

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

    // Clean up
    FreeCtrl(&ctrl);
}

// count = accumarray(vec, 1).
void Accumarray(const vector<int>& vec, vector<int>& count)
{
    DEBUG_HIF(CallStackEntry cse("Accumarray"))

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
    DEBUG_HIF(CallStackEntry cse("Cumsum"))

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
