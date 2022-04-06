cmake_minimum_required(VERSION 3.0.0)
project(tsp)
set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/includes)

find_package(OpenMP REQUIRED)
file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/src/*.c*")
add_library(tsp STATIC ${SOURCES})
target_link_libraries(tsp OpenMP::OpenMP_CXX)

add_executable(matrix-test "${CMAKE_SOURCE_DIR}/tests/matrix-tests.cpp")
target_link_libraries(matrix-test tsp)

add_executable(random-sampling-test "${CMAKE_SOURCE_DIR}/tests/random-sampling.cpp")
target_link_libraries(random-sampling-test tsp)

add_executable(backtracking-test "${CMAKE_SOURCE_DIR}/tests/backtracking.cpp")
target_link_libraries(backtracking-test tsp)

add_executable(greedy-test "${CMAKE_SOURCE_DIR}/tests/greedy.cpp")
target_link_libraries(greedy-test tsp)

add_executable(dijkstra-test "${CMAKE_SOURCE_DIR}/tests/dijkstra.cpp")
target_link_libraries(dijkstra-test tsp)

add_executable(big-boy-first-thirty-heuristics-test "${CMAKE_SOURCE_DIR}/tests/big-boy-first-thirty-heuristics.cpp")
target_link_libraries(big-boy-first-thirty-heuristics-test tsp)
