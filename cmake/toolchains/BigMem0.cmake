# The serial GNU compilers
set(COMPILER_DIR /opt/rh/devtoolset-7/root/usr/bin)
set(CMAKE_C_COMPILER       ${COMPILER_DIR}/gcc)
set(CMAKE_CXX_COMPILER     ${COMPILER_DIR}/g++)
set(CMAKE_Fortran_COMPILER ${COMPILER_DIR}/gfortran)

# The MPI wrappers for the C and C++ compilers
# set(MPI_COMPILER_DIR /opt/MPICH/4.0.2/bin)
# set(MPI_C_COMPILER       ${MPI_COMPILER_DIR}/mpicc)
# set(MPI_CXX_COMPILER     ${MPI_COMPILER_DIR}/mpicxx)
# set(MPI_Fortran_COMPILER ${MPI_COMPILER_DIR}/mpif90)

# Elemental
set(EL_INCLUDE_DIR /home/jyliu/HIFCpp/HIF/packages/elemental/include)
set(EL_LIB_DIR /home/jyliu/HIFCpp/HIF/packages/elemental/lib)
set(EL_LIB64_DIR /home/jyliu/HIFCpp/HIF/packages/elemental/lib64)

# Metis
set(METIS_INCLUDE_DIR /home/jyliu/HIFCpp/HIF/extern/metis/include)
set(METIS_LIBMETIS_DIR /home/jyliu/HIFCpp/HIF/extern/metis/libmetis)
set(METIS_GKLIB_DIR /home/jyliu/HIFCpp/HIF/extern/metis/GKlib)
set(METIS_BUILD_LIBMETIS_DIR /home/jyliu/HIFCpp/HIF/packages/metis/libmetis)
set(METIS_BUILD_INCLUDE_DIR /home/jyliu/HIFCpp/HIF/packages/metis/include)

set (CMAKE_EXE_LINKER_FLAGS "-Wl,--copy-dt-needed-entries")
