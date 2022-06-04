# The serial GNU compilers
set(COMPILER_DIR /opt/rh/devtoolset-7/root/usr/bin)
set(CMAKE_C_COMPILER       ${COMPILER_DIR}/gcc)
set(CMAKE_CXX_COMPILER     ${COMPILER_DIR}/g++)
set(CMAKE_Fortran_COMPILER ${COMPILER_DIR}/gfortran)

# The MPI wrappers for the C and C++ compilers
set(MPI_COMPILER_DIR /opt/MPICH/4.0.2/bin)
set(MPI_C_COMPILER       ${MPI_COMPILER_DIR}/mpicc)
set(MPI_CXX_COMPILER     ${MPI_COMPILER_DIR}/mpicxx)
set(MPI_Fortran_COMPILER ${MPI_COMPILER_DIR}/mpif90)

set(EL_INCLUDE_DIR /home/jyliu/packages/elemental/master/include)
set(EL_LIB_DIR /home/jyliu/packages/elemental/master/lib64)

# set(METIS_INCLUDE_DIR /opt/local/metis/5.1.0/include)
# set(METIS_LIB_DIR /opt/local/metis/5.1.0/lib)