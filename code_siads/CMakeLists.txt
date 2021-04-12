# The name of our project is "PROVA". CMakeLists files in this project can
# refer to the root source directory of the project as ${PROVA_SOURCE_DIR} and
# to the root binary directory of the project as ${PROVA_BINARY_DIR}. 

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(CMAKE_BUILD_TYPE debug)
set(CMAKE_CXX_FLAGS_DEBUG "-g03 -Wall")
set(CMAKE_C_FLAGS_DEBUG "-g03 -Wall")


cmake_minimum_required (VERSION 2.6)
project (PROVA)

#find_package(MPFR REQUIRED)
set(PETSC_DIR $ENV{PETSC_DIR})
find_package(PETSc 3.2 REQUIRED)
find_package(MPFR REQUIRED)

set(iRRAM $ENV{IRRAM_DIR})
set(iRRAM_INCLUDE_DIRS "${iRRAM}/include")
find_library(iRRAM_LIBRARIES iRRAM "${iRRAM}/lib")

include_directories (${PROVA_SOURCE_DIR})
include_directories(${PETSC_INCLUDES})
include_directories(${MPFR_INCLUDES})
include_directories(${iRRAM_INCLUDE_DIRS})

set(LIBS ${LIBS} ${PETSC_LIBRARIES})
set(LIBS ${LIBS} ${MPFR_LIBRARIES})

message(STATUS ${iRRAM_INCLUDE_DIRS})

add_subdirectory (ulam)
add_subdirectory (lipschitz)

add_executable(markovize markovize.cxx)
add_executable(eigenerr-L1 eigenerr-L1.cxx)
add_executable(eigenerr-lipschitz eigenerr-lipschitz.cxx)

add_executable(genmatrix-ulam genmatrix-ulam.cxx)
add_executable(genmatrix-lipschitz genmatrix-lipschitz.cxx)

target_link_libraries(markovize ${LIBS})
target_link_libraries (eigenerr-L1 eigenrig-L1 ${LIBS})
target_link_libraries (eigenerr-lipschitz eigenrig-lipschitz ${LIBS})
target_link_libraries (genmatrix-ulam actual-genmatrix-ulam rpartition rinterval dynamic rapfloat ${LIBS})
target_link_libraries (genmatrix-lipschitz actualgen-lipschitz dynamic-lipschitz ${iRRAM_LIBRARIES} ${LIBS})
