include_directories (${Z85_SOURCE_DIR}/src)
add_executable (Test test.cpp)
target_link_libraries (Test Z85cpp)

