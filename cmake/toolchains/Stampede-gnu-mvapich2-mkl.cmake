# The serial GNU compilers
set(COMPILER_DIR /opt/apps/gcc/4.9.1/bin)
set(CMAKE_C_COMPILER       ${COMPILER_DIR}/gcc)
set(CMAKE_CXX_COMPILER     ${COMPILER_DIR}/g++)
set(CMAKE_Fortran_COMPILER ${COMPILER_DIR}/gfortran)

# The MPI wrappers for the C and C++ compilers
set(MPI_COMPILER_DIR /opt/apps/gcc4_9/mvapich2/2.1/bin)
set(MPI_C_COMPILER       ${MPI_COMPILER_DIR}/mpicc)
set(MPI_CXX_COMPILER     ${MPI_COMPILER_DIR}/mpicxx)
set(MPI_Fortran_COMPILER ${MPI_COMPILER_DIR}/mpif90)

if(CMAKE_BUILD_TYPE MATCHES Debug)
  set(CXX_FLAGS "-g")
else()
  set(CXX_FLAGS "-O3")
endif()

set(EL_INCLUDE_DIR /work/02539/lyzh588/install/Elemental/include)
set(EL_LIB_DIR /work/02539/lyzh588/install/Elemental/lib)

set(Boost_INCLUDE_DIR /opt/apps/gcc4_9/boost/1.55.0/x86_64/include)
set(Boost_LIBRARY_DIR /opt/apps/gcc4_9/boost/1.55.0/x86_64/lib)
