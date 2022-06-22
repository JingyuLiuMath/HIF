# What I Do

- /include/El/blas_like/level1/GetSubmatrix.hpp
write
```
template<typename T>
void GetSubmatrix
( const SparseMatrix<T>& A,
  const vector<Int>& I,
  const vector<Int>& J,
        SparseMatrix<T>& ASub )
```

- /include/El/core/SparseMatrix/decl.hpp, impl.hpp
write
```
Int Size()
```
