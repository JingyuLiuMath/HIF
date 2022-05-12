# The serial GNU compilers
set(COMPILER_DIR /opt/rh/devtoolset-3/root/usr/bin)
set(CMAKE_C_COMPILER       ${COMPILER_DIR}/gcc)
set(CMAKE_CXX_COMPILER     ${COMPILER_DIR}/g++)
set(CMAKE_Fortran_COMPILER ${COMPILER_DIR}/gfortran)

# The MPI wrappers for the C and C++ compilers
set(MPI_COMPILER_DIR /share/sw/free/mpich/3.1.4/gcc/4.4/bin)
set(MPI_C_COMPILER       ${MPI_COMPILER_DIR}/mpicc)
set(MPI_CXX_COMPILER     ${MPI_COMPILER_DIR}/mpicxx)
set(MPI_Fortran_COMPILER ${MPI_COMPILER_DIR}/mpif90)

if(CMAKE_BUILD_TYPE MATCHES Debug)
  set(CXX_FLAGS "-g")
else()
  set(CXX_FLAGS "-O3")
endif()

set(EL_INCLUDE_DIR /share/PI/lexing/install/Elemental/include)
set(EL_LIB_DIR /share/PI/lexing/install/Elemental/lib)

set(Boost_INCLUDE_DIR /share/sw/free/boost/1.60.0/include)
set(Boost_LIBRARY_DIR /share/sw/free/boost/1.60.0/lib)
