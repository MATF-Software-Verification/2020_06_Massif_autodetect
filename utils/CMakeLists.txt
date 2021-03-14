set(UTILS_SOURCE
   ${UTILS_SOURCE}
   ${CMAKE_CURRENT_SOURCE_DIR}/src/options_desc.cpp
   PARENT_SCOPE
)
target_include_directories(parser PUBLIC include)
