#include "HIF.hpp"

namespace HIF {

// Graph partition.
template <typename Scalar>
void GraphPart(const SparseMatrix<Scalar>& A, 
    vector<int>& p1, vector<int>& p2, 
    vector<int>& sep1, vector<int>& sep2)
{
    std::cout << "GraphPart" << std::endl;
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
    std::cout << "MetisPart" << std::endl;
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
    std::cout << "MetisSepPart" << std::endl;
    // nvtxs.
    idx_t nvtxs = A.Height();
    const int* sourceA = A.LockedSourceBuffer();
    const int* targetA = A.LockedTargetBuffer();
    int nnzA = A.NumEntries();
    // xadj.
    idx_t* xadj;
    vector<int> rowindex(nnzA, 0);
    vector<int> colindex(nnzA, 0);
    for (int t = 0; t < nnzA; t++)
    {
        rowindex[t] = sourceA[t];
        colindex[t] = targetA[t];
    }
    vector<int> ijindex;
    FindEqualIndex(rowindex, colindex, ijindex);
    /*for (int k = 0; k < ijindex.size(); k++)
    {
        rowindex.erase(ijindex[k]);
        colindex.erase(ijindex[k]);
    }*/
    for (int k = ijindex.size() - 1; k >= 0; k--)
    {
        rowindex.erase(rowindex.begin() + ijindex[k]);
        colindex.erase(colindex.begin() + ijindex[k]);
    }
    if (colindex.size() == 0)
    {
        // nothing to do.
    }
    else
    {
        vector<int> accumj;
        Accumarray(colindex, accumj);
        vector<int> cumsumj;
        Cumsum(accumj, cumsumj);
        xadj = new idx_t[cumsumj.size() + 1];
        xadj[0] = 0;
        for (int i = 1; i < cumsumj.size() + 1; i++)
        {
            xadj[i] = cumsumj[i];
        }
    }
    // adjncy.
    idx_t* adjncy;
    adjncy = new idx_t[rowindex.size()];
    for (int i = 0; i < rowindex.size(); i++)
    {
        adjncy[i] = rowindex[i];
    }
    idx_t* vwgt;
    idx_t options = METIS_NOPTIONS;
    
    // Metis main function
    idx_t i, nnvtxs = 0;
    idx_t ptlgraph, ptrgraph, ptsep;
    graph_t* graph = NULL;
    ctrl_t* ctrl;
    idx_t* piperm;
    idx_t snvtxs[3];
    idx_t* where;

    // set up the run time parameters
    std::cout << "Jyliu 1" << std::endl;

    ctrl = SetupCtrl(METIS_OP_OMETIS, options, 1, 3, NULL, NULL);

    std::cout << "Jyliu 2" << std::endl;
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
        graph = SetupGraph(ctrl, nvtxs, 1, xadj, adjncy, vwgt, NULL, NULL);

    ASSERT(CheckGraph(graph, ctrl->numflag, 1));

    /* allocate workspace memory */
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
    auto maxPosition = std::max_element(vec.begin(), vec.end());
    count = vector<int> (*maxPosition, 0);
    for (int i = 0; i < vec.size(); i++)
    {
        count[vec[i]] += 1;
    }
}

// a = cumsum(vec)
void Cumsum(const vector<int>& vec, vector<int>& a)
{
    if (vec.size() == 0)
    {
        return;
    }
    a.resize(vec.size());
    a[0] = vec[0];
    for (int i = 1; i < a.size(); i++)
    {
        a[i] = a[i - 1] + vec[i];
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