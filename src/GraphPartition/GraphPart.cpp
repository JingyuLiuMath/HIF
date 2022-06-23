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
    // p1, p2, sep1 is sorted.
    // p1 = p1 + sep1, p2 = p2, sep2 need to be assigned.
    TIMER_HIF(TimerStart(TIMER_SETSEP2))
    /*p1.insert(p1.end(), sep1.begin(), sep1.end());
    sort(p1.begin(), p1.end());
    sort(sep1.begin(), sep1.end());
    sort(p2.begin(), p2.end());*/
    for (int t = 0; t < sep1.size(); t++)
    {
        AddElement_Sort(p1, sep1[t]);
    }
    vector<int> tmpsep2(p2.size(), 0);
    for (int i = 0; i < sep1.size(); i++)
    {
        int sep1i = sep1[i];
        vector<int> sep2i_tmp(A.Width());
        int actualsize_sep2i_tmp = 0;
        const int* targetA = A.LockedTargetBuffer();
        const int* offsetA = A.LockedOffsetBuffer();
        for (int k = offsetA[sep1i]; k < offsetA[sep1i + 1]; k++)
        {
            sep2i_tmp[actualsize_sep2i_tmp] = targetA[k];
            actualsize_sep2i_tmp++;
        }
        sep2i_tmp.erase(sep2i_tmp.begin() + actualsize_sep2i_tmp, sep2i_tmp.end());
        vector<int> index_sep2i;
        Intersect_Sort(sep2i_tmp, p2, index_sep2i, 2);
        for (int t = 0; t < index_sep2i.size(); t++)
        {
            tmpsep2[index_sep2i[t]] += 1;
        }
    }
    sep2.resize(p2.size());
    int actualsize_sep2 = 0;
    for (int t = 0; t < tmpsep2.size(); t++)
    {
        if (tmpsep2[t] > 0)
        {
            sep2[actualsize_sep2] = p2[t];
            actualsize_sep2++;
        }
    }
    sep2.erase(sep2.begin() + actualsize_sep2, sep2.end());
    TIMER_HIF(TimerStop(TIMER_SETSEP2))
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
    const int* offsetA = A.LockedOffsetBuffer();
    vector<int> degree(A.Height(), 0);
    for (int t = 1; t <= A.Height(); t++)
    {
        degree[t - 1] += (offsetA[t] - 1);
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
    
    TIMER_HIF(TimerStart(TIMER_GETSUBMATRIX))
    SparseMatrix<Scalar> A1 = A(idx, idx);
    TIMER_HIF(TimerStop(TIMER_GETSUBMATRIX))

    vector<int> lidx, ridx, sepidx;
    MetisSepPart(A1, lidx, ridx, sepidx);  // lidx, ridx, sepidx are sorted.
    lidx.sort();
    ridx.sort();
    sepidx.sort();
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
    // sep.insert(sep.end(), singleidx.begin(), singleidx.end());
    for (int t = 0; t < singleidx.size(); t++)
    {
        AddElement_Sort(sep, singleidx[t]);
    }
}

// Metis separator partition.
template <typename Scalar>
void MetisSepPart(const SparseMatrix<Scalar>& A, 
    vector<int>& p1, vector<int>& p2, vector<int>& sep)
{
    DEBUG_HIF(CallStackEntry cse("MetisSepPart"))

    TIMER_HIF(TimerStart(TIMER_METIS))
    // nvtxs.
    idx_t nvtxs = A.Height();
    // xadj.
    const int* sourceA = A.LockedSourceBuffer();
    const int* targetA = A.LockedTargetBuffer();
    const int* offsetA = A.LockedOffsetBuffer();
    int nnzA = A.NumEntries();
    idx_t* xadj = new idx_t[nvtxs + 1];
    xadj[0] = 0;
    for (int t = 1; t <= nvtxs; t++)
    {
        xadj[t] = offsetA[t] - t;
    }
    // adjncy.
    idx_t* adjncy = new idx_t[nnzA - nvtxs];
    int actualsize_adjncy = 0;
    for (int t = 0; t < nnzA; t++)
    {
        if (sourceA[t] != targetA[t])
        {
            adjncy[actualsize_adjncy] = targetA[t];
            actualsize_adjncy++;
        }
    }
    // others.
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
    TIMER_HIF(TimerStop(TIMER_METIS))
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
