#
# Fudan University
#
cmake_minimum_required(VERSION 2.8.12) # for the new FindMPI module
project(HIF)

set(HIF_VERSION_MAJOR 1)
set(HIF_VERSION_MINOR 0)

option(TIMER "Instrument HIF with timers." OFF)
option(HIF_INFO "Print HIF detailed information in the log file of master node." OFF)
option(SBATCH "Auto-generate SBATCH files." OFF)

if(HIF_INFO)
    set(TIMER ON)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  if(NOT CMAKE_CXX_FLAGS)
    set(CMAKE_CXX_FLAGS "-std=gnu++11 -O3")
  endif()
  option(RELEASE "Release mode flag" ON)
else()
  if(NOT CMAKE_CXX_FLAGS)
    set(CMAKE_CXX_FLAGS "-std=gnu++11 -g -O2  -Wall")
  endif()
  option(RELEASE "Release mode flag" OFF)
endif()

# Store all of our system-specific definitions
configure_file(${PROJECT_SOURCE_DIR}/cmake/config.h.cmake
               ${PROJECT_BINARY_DIR}/include/HIF/config.h)
install(FILES ${PROJECT_BINARY_DIR}/include/HIF/config.h
        DESTINATION include/HIF)

# Grab all of the .c, .cpp, .h, and .hpp HIF files
file(GLOB_RECURSE HIF_CPP RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
     "src/*.c" "src/*.cpp" "src/*.hpp")
file(GLOB_RECURSE HIF_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
     "include/*.h" "include/*.hpp")
set(HIF_SRC "${HIF_CPP};${HIF_HEADERS}")

find_package(MPI REQUIRED)
include_directories(${MPI_C_INCLUDE_PATH})
include_directories(${MPI_CXX_INCLUDE_PATH})

if(NOT EL_LIB_DIR)
    find_package(El REQUIRED)
endif()
link_directories(${EL_LIB_DIR})
include_directories(${EL_INCLUDE_DIR})

if(NOT METIS_LIB_DIR)
    find_package(METIS REQUIRED)
endif()
link_directories(${METIS_LIB_DIR})
include_directories(${METIS_INCLUDE_DIR})
include_directories(${METIS_LIBMETIS_DIR})
include_directories(${METIS_GKLIB_DIR})

# Build the HIF source
add_library(HIF STATIC ${HIF_SRC})
install(TARGETS HIF DESTINATION lib)

# Copy the headers into the build directory
set(PREPARED_HEADERS)
foreach(HEADER ${HIF_HEADERS})
  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${HEADER}
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${HEADER}
            ${CMAKE_CURRENT_BINARY_DIR}/${HEADER}
    DEPENDS "${PROJECT_SOURCE_DIR}/${HEADER}")
  list(APPEND PREPARED_HEADERS ${PROJECT_BINARY_DIR}/${HEADER})

  get_filename_component(HEADER_PATH ${HEADER} PATH)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${HEADER} DESTINATION ${HEADER_PATH})
endforeach()
add_custom_target(prepare_HIF_headers DEPENDS ${PREPARED_HEADERS})
add_dependencies(HIF prepare_HIF_headers)

# Make sure the HIF headers can be found
include_directories("${PROJECT_BINARY_DIR}/include")

set(TEST_DIR ${PROJECT_SOURCE_DIR}/test)
set(TEST_TYPES HIFGraph)
set(HIFGraph_TESTS Factorization)
foreach(TYPE ${TEST_TYPES})
  set(OUTPUT_DIR "${PROJECT_BINARY_DIR}/bin/${TYPE}")
  foreach(TEST ${${TYPE}_TESTS})
    add_executable(${TYPE}-${TEST} ${TEST_DIR}/${TYPE}/${TEST}.cpp)
    target_link_libraries(${TYPE}-${TEST} El METIS HIF ${MPI_LIBRARIES} stdc++fs)
    # linking stdc++fs for filesystem on gnu 7 & 8
    set_target_properties(${TYPE}-${TEST}
                          PROPERTIES
                          OUTPUT_NAME ${TEST}
                          RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})
    install(TARGETS ${TYPE}-${TEST} DESTINATION bin/${TYPE})
  endforeach()
endforeach()
