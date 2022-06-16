# HIF

Hierarchical Interpolative Factorization (HIF) is a fast solver for symmetric linear systems $Ax=b$. Our code based on [Elemental](https://github.com/elemental/Elemental) and [Metis](http://glaros.dtc.umn.edu/gkhome/metis/metis/overview).

## Authors

Jingyu Liu, Fudan University, 381258337@qq.com

## Aknowledgement

Many thanks to [Yingzhou Li](https://www.yingzhouli.com/). Without his guidance and help, the code can't be finished.

June 15, 2022

## Remarks

Since [Elemental](https://github.com/elemental/Elemental) is not complete, you need to write the following codes in order to run my code successfully.

- /include/El/blas_like/level1/GetSubmatrix.hpp

``` C++
template<typename T>
void GetSubmatrix
( const SparseMatrix<T>& A,
  const vector<Int>& I,
  const vector<Int>& J,
        SparseMatrix<T>& ASub )
{
    EL_DEBUG_CSE
    // TODO(poulson): Decide how to handle unsorted I and J with duplicates
    // LogicError("This routine is not yet written");
    ASub.Resize(I.size(), J.size());
    for (Int row = 0; row < I.size(); row++)
    {
        for (Int col = 0; col < J.size(); col++)
        {
            if (A.Get(I[row], J[col]) != T(0))
            {
                ASub.QueueUpdate(row, col, A.Get(I[row], J[col]));
            }
        }
    }
    ASub.ProcessQueues();
}
```
