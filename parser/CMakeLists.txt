add_library(parser STATIC 
            src/parser.cpp 
            src/snapshot.cpp
            src/node.cpp)
target_include_directories(parser PUBLIC include)
