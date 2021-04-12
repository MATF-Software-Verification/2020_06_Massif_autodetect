cmake_minimum_required(VERSION 3.9)
project(fixif VERSION 1.0)


find_package(Boost 1.50 COMPONENTS program_options system filesystem  REQUIRED)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(parser)
add_subdirectory(utils)
add_subdirectory(analyzer)

add_definitions( -DFIXIF_VERSION="${CMAKE_PROJECT_VERSION}" )
add_executable(fixif 
main.cpp
${UTILS_SOURCE})
target_link_libraries(fixif parser analyzer
                ${Boost_LIBRARIES}
                ${Boost_FILESYSTEM_LIBRARY}
                ${Boost_SYSTEM_LIBRARY}
                )
