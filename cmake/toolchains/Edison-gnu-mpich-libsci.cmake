# NOTE: You will need to have the GNU environment loaded, e.g., via the command
#       
# module swap PrgEnv-intel PrgEnv-gnu
#

# The Cray wrappers
set(COMPILER_DIR $ENV{CRAYPE_DIR}/bin)
set(CMAKE_C_COMPILER       ${COMPILER_DIR}/cc)
set(CMAKE_CXX_COMPILER     ${COMPILER_DIR}/CC)
set(CMAKE_Fortran_COMPILER ${COMPILER_DIR}/ftn)

# This is just a hack, as this machine always uses the above wrappers
set(MPI_C_COMPILER ${CMAKE_C_COMPILER})
set(MPI_CXX_COMPILER ${CMAKE_CXX_COMPILER})
set(MPI_Fortran_COMPILER ${CMAKE_Fortran_COMPILER})

if(CMAKE_BUILD_TYPE MATCHES Debug)
  set(CMAKE_CXX_FLAGS "-std=gnu++11 -O3 -dynamic -Wl,-rpath,/global/homes/r/ryanlee/install/Elemental/lib64:/global/homes/r/ryanlee/install/Elemental/lib")
else()
  set(CMAKE_CXX_FLAGS "-std=gnu++11 -O2 -dynamic -Wl,-rpath,/global/homes/r/ryanlee/install/Elemental/lib64:/global/homes/r/ryanlee/install/Elemental/lib")
endif()

set(EL_INCLUDE_DIR /global/homes/r/ryanlee/install/Elemental/include)
set(EL_LIB_DIR /global/homes/r/ryanlee/install/Elemental/lib64)
set(METIS_LIB_DIR /global/homes/r/ryanlee/install/Elemental/lib)

set(Boost_INCLUDE_DIR /usr/common/software/boost/1.55/gnu/include)
set(Boost_LIBRARY_DIR /usr/common/software/boost/1.55/gnu/lib)

set(CMAKE_FIND_LIBRARY_SUFFIXES .so ${CMAKE_FIND_LIBRARY_SUFFIXES})
