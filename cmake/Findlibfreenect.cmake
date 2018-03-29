#nothing yet; in case we need it, edit it for cmake to find libfreenect
unset(libfreenect_FOUND)

set(libfreenect_DIR ~/dep/libfreenect)

find_path(libfreenect_INCLUDE_DIRS NAMES libfreenect.hpp kinect.hpp
    HINTS ${libfreenect_DIR}/include)

message("libfreenct_include_dirs:${libfreenect_INCLUDE_DIRS}")

find_library(libfreenect_LIBRARIES NAMES libfreenect.dylib
    HINTS ${libfreenect_DIR}/build/lib)

message("libfreenect_lib_dirs:${libfreenect_LIBRARIES}")

if(libfreenect_LIBRARIES AND libfreenect_INCLUDE_DIRS)
    set(libfreenect_FOUND 1)
endif()
