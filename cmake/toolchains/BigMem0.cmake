# The serial GNU compilers
set(COMPILER_DIR /opt/rh/devtoolset-7/root/usr/bin)
set(CMAKE_C_COMPILER       ${COMPILER_DIR}/gcc)
set(CMAKE_CXX_COMPILER     ${COMPILER_DIR}/g++)
set(CMAKE_Fortran_COMPILER ${COMPILER_DIR}/gfortran)

# Elemental
set(EL_INCLUDE_DIR /home/jyliu/packages/elemental/master/include)
set(EL_LIB_DIR /home/jyliu/packages/elemental/master/lib64)

# Metis
set(METIS_INCLUDE_DIR /home/jyliu/metis-5.1.0/include)
set(METIS_LIBMETIS_DIR /home/jyliu/metis-5.1.0/libmetis)
set(METIS_GKLIB_DIR /home/jyliu/metis-5.1.0/GKlib)
set(METIS_BUILD_LIBMETIS_DIR /home/jyliu/metis-5.1.0/build/Linux-x86_64/libmetis)
set(METIS_BUILD_INCLUDE_DIR /home/jyliu/metis-5.1.0/build/Linux-x86_64/include)

set (CMAKE_EXE_LINKER_FLAGS "-Wl,--copy-dt-needed-entries")
