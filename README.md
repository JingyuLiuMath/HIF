# HIF

Hierarchical Interpolative Factorization (HIF) is a fast solver for symmetric linear systems $Ax=b$. Our code based on [Elemental](https://github.com/elemental/Elemental) and [Metis](http://glaros.dtc.umn.edu/gkhome/metis/metis/overview).

## Authors

Jingyu Liu, Fudan University, 381258337@qq.com

## How to run

### Preliminaries

``` bash
source scl_source enable devtoolset-7
module load LAPACK
module load MPICH
module load CMake
module load OpenBLAS
```

### Compile Elemental

``` bash
cd extern/elemental/
mkdir -p ../../packages/elemental/
mkdir build/
cd build/
cmake -D CMAKE_INSTALL_PREFIX=/home/jyliu/HIFCpp/HIF/packages/elemental/ \
-D CMAKE_BUILD_TYPE=Release \
-D GFORTRAN_LIB=/opt/rh/devtoolset-7/root/usr/lib/gcc/x86_64-redhat-linux/7/  \
-D INSTALL_PYTHON_PACKAGE=OFF ..
make -j
make install
```

### Compile METIS

``` bash
cd extern/metis/
make config
make -j
```

### Compile HIF

Before compile HIF, please write a `.cmake` file. You can find an example in `cmake/toolchains/BigMem0.cmake`.

``` bash
mkdir build/
cd build/
cmake .. --toolchain ../cmake/toolchains/BigMem0.cmake
make -j64
```

### Run tests

``` bash
cd bin/HIFSolve/
./HIFSolve --input_A "path/to/A" --input_b "path/to/b" --minvtx 64 --HIFbutton true --tol 1e-3 --logApp true
```

And see `log.log` in the dir `log` for more inforamtion.

## Aknowledgement

Many thanks to [Yingzhou Li](https://www.yingzhouli.com/). Without his guidance and help, the code can't be finished.

July 20, 2022
