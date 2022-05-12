# The serial GNU compilers
set(COMPILER_DIR /opt/rh/devtoolset-7/root/usr/bin)
set(CMAKE_C_COMPILER       ${COMPILER_DIR}/gcc)
set(CMAKE_CXX_COMPILER     ${COMPILER_DIR}/g++)
set(CMAKE_Fortran_COMPILER ${COMPILER_DIR}/gfortran)

# The MPI wrappers for the C and C++ compilers
set(MPI_COMPILER_DIR /opt/mpich/4.0/bin)
set(MPI_C_COMPILER       ${MPI_COMPILER_DIR}/mpicc)
set(MPI_CXX_COMPILER     ${MPI_COMPILER_DIR}/mpicxx)
set(MPI_Fortran_COMPILER ${MPI_COMPILER_DIR}/mpif90)

set(EL_INCLUDE_DIR /opt/home-admin/jyliu/packages/elemental/master/include)
set(EL_LIB_DIR /opt/home-admin/jyliu/packages/elemental/master/lib64)
