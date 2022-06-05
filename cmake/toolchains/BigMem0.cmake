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
# set(METIS_LIB_DIR /home/jyliu/packages/elemental/master/lib)

set(METIS_INCLUDE_DIR /home/jyliu/metis-5.1.0/include)
set(METIS_LIBMETIS_DIR /home/jyliu/metis-5.1.0/libmetis)
set(METIS_GKLIB_DIR /home/jyliu/metis-5.1.0/GKlib)
set(METIS_BUILD_LIBMETIS_DIR /home/jyliu/metis-5.1.0/build/Linux-x86_64/libmetis)
set(METIS_BUILD_INCLUDE_DIR /home/jyliu/metis-5.1.0/build/Linux-x86_64/include)

set (CMAKE_EXE_LINKER_FLAGS "--copy-dt-needed-entries")
