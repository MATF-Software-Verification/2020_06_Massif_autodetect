add_library(analyzer STATIC 
            src/analyzer.cpp)
target_include_directories(analyzer PUBLIC include)
target_link_libraries(analyzer parser)
