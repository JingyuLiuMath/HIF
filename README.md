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
    // REMARK(Jingyu Liu): The function is correct only when I and J are sorted.
    ASub.Resize(I.size(), J.size());
    const Int* sourceA = A.LockedSourceBuffer();
    const Int* targetA = A.LockedTargetBuffer();
    const Int* offsetA = A.LockedOffsetBuffer();
    const T* valueA = A.LockedValueBuffer();
    Int Irow = -1;
    Int nnzIrow = -1;
    Int startIrow = -1;
    Int endIrow = -1;
    Int i = -1;
    Int j = -1;
    for (Int row = 0; row < I.size(); row++)
    {
        Irow = I[row];
        startIrow = offsetA[Irow];
        endIrow = offsetA[Irow + 1];
        i = startIrow;
        j = 0;
        // J and Target(startIrow:endIrow-1) is sorted.
        while ((i < endIrow) && (j < J.size()))
        {
            if (targetA[i] < J[j])
            {
                i++;
            }
            else if (targetA[i] > J[j])
            {
                j++;
            }
            else
            {
                ASub.QueueUpdate(row, j, valueA[i]);
                i++;
                j++;
            }
        }
    }
    ASub.ProcessQueues();
}
```
