add_library (Z85 z85.c z85.h)
add_library (Z85cpp z85_impl.cpp z85.hpp)
target_link_libraries (Z85cpp Z85)

install (TARGETS Z85 DESTINATION lib)
install (TARGETS Z85cpp DESTINATION lib)

