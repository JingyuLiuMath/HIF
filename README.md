# HIF

Hierarchical Interpolative Factorization (HIF) is a fast solver for symmetric linear systems $Ax=b$. Our code based on [Elemental](https://github.com/elemental/Elemental) and [Metis](http://glaros.dtc.umn.edu/gkhome/metis/metis/overview).

## Authors

Jingyu Liu, Fudan University, 381258337@qq.com

## How to run

### Compile (Local) Elemental

Remark: Since [Elemental](https://github.com/elemental/Elemental) is not complete, we write some functions and that's why we call it local Elemental.

``` bash
cd extern/Elemental/
mkdir build
cd build/
mkdir /home/jyliu/packages/elemental
cmake -D CMAKE_INSTALL_PREFIX=/home/jyliu/packages/elemental/master/ \
-D CMAKE_BUILD_TYPE=Release \
-D GFORTRAN_LIB=/opt/rh/devtoolset-7/root/usr/lib/gcc/x86_64-redhat-linux/7/  \
-D INSTALL_PYTHON_PACKAGE=OFF ..
make -j64
make install
```

### Compile Metis

Just follow the guide of [Metis](http://glaros.dtc.umn.edu/gkhome/metis/metis/overview).

### Compile HIF

Before compile HIF, please write a `.cmake` file. You can find an example in `cmake/toolchains/BigMem0.cmake`.

``` bash
mkdir build/
cd build/
cmake .. --toolchain <path/to/your/cmakefile>
make -j64
```

### Run tests

``` bash
cd bin/HIFGraph/
./HIF --input_A "path/to/A" --input_b "path/to/b" --minvtx 64 --HIFbutton true --tol 1e-3 --logApp true
```

And see `log.log` in the dir log for more inforamtion.

## Aknowledgement

Many thanks to [Yingzhou Li](https://www.yingzhouli.com/). Without his guidance and help, the code can't be finished.

June 22, 2022