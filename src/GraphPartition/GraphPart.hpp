#include "HIF.hpp"
#include <matrix.h>
#include <metislib.h>
#include <metis.h>

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
    // TODO: set nvtxs, xadj, adjncy.
    // nvtxs.
    idx_t nvtxs = A.Height();
    // xadj.
    idx_t* xadj;
    vector<int> rowindex, colindex;
    FindAllNonzeroIndexMat(A, rowindex, colindex);
    vector<int> ijindex;
    FindEqualIndex(roindex, colindex, ijindex);
    for (int k = 0; k < ijindex.size(); k++)
    {
        rowindex.erase(ijindex[k]);
        colindex.erase(ijindex[k]);
    }
    if (colindex.size() == 0)
    {
        xadj.resize(0);
    }
    else
    {
        vector<int> accumj;
        Accumarray(colindex, accumj);
        vector<int> cumsumj;
        Cumsum(accumj, cumsumj);
        g.xadj->resize(1);
        (g.xadj)[0] = 0;
        (g.xadj).insert((g.xadj).end(), cumsumj.start(), cumsumj.end());
    }
    // adjncy.
    idx_t* adjncy = i;
    // idx_t* adjncy;
    idx_t* vwgt;
    idx_t options[METIS_NOPTIONS];
    
    // Metis main function
    idx_t i, nnvtxs = 0;
    idx_t ptlgraph, ptrgraph, ptsep;
    graph_t* graph = NULL;
    ctrl_t* ctrl;
    idx_t* piperm;
    idx_t snvtxs[3];
    idx_t* where;

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

// Create graph.
template <typename Scalar>
void MetisGraph(const SparseMatrix<Scalar>& A, graph_t& g)
{
    g.nvtxs = A.Height();
    vector<int> i, j;
    FindAllNonzeroIndexMat(A, i, j);
    vector<int> ijindex;
    FindEqualIndex(i, j, ijindex);
    for (int k = 0; k < ijindex.size(); k++)
    {
        i.erase(ijindex[k]);
        j.erase(ijindex[k]);
    }
    if (j.size() == 0)
    {
        (g.xadj).resize(0);
    }
    else
    {
        // g.xadj = [1;cumsum(accumarray(j, 1))+1]
        vector<int> accumj;
        Accumarray(j, accumj);
        vector<int> cumsumj;
        Cumsum(accumj, cumsumj);
        g.xadj->resize(1);
        (g.xadj)[0] = 0;
        (g.xadj).insert((g.xadj).end(), cumsumj.start(), cumsumj.end());
    }
    g.adjncy = i - 1;
}

// count = accumarray(vec, 1).
void Accumarray(const vector<int>& vec, vector<int>& count)
{
    auto maxPosition = max_element(a.begin(), a.end());
    count = vector(*maxPosition, 0);
    for (int i = 0; i < vec; i++)
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

} // namespace HIF.