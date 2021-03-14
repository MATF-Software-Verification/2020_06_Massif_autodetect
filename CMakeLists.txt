cmake_minimum_required(VERSION 3.9)
project(fixif VERSION 1.0)


find_package(Boost 1.40 COMPONENTS program_options REQUIRED)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(parser)
add_subdirectory(utils)

add_definitions( -DFIXIF_VERSION="${CMAKE_PROJECT_VERSION}" )
add_executable(fixif 
main.cpp
${UTILS_SOURCE})
target_link_libraries(fixif parser ${Boost_LIBRARIES})
