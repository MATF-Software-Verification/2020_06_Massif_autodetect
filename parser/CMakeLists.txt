add_library(parser STATIC 
            src/parser.cpp 
            src/snapshot.cpp)
target_include_directories(parser PUBLIC include)
