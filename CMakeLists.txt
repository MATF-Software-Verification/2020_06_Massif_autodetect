cmake_minimum_required(VERSION 3.9)
project(fixif VERSION 1.0)


find_package(Boost 1.50 COMPONENTS program_options system filesystem  REQUIRED)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CLANG_LIBS clang)


include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})
add_subdirectory(parser)
add_subdirectory(utils)

llvm_map_components_to_libnames(llvm_libs support core irreader)

add_definitions( -DFIXIF_VERSION="${CMAKE_PROJECT_VERSION}" )
add_executable(fixif 
main.cpp
${UTILS_SOURCE})
target_link_libraries(fixif parser 
                ${Boost_LIBRARIES}
                ${Boost_FILESYSTEM_LIBRARY}
                ${Boost_SYSTEM_LIBRARY}
                ${llvm_libs}
                ${CLANG_LIBS}
                )

