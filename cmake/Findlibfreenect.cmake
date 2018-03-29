#nothing yet; in case we need it, edit it for cmake to find libfreenect
unset(libfreenect_FOUND)

set(libfreenect_inc_DIR /usr/local/include/libfreenect)

find_path(libfreenect_INCLUDE_DIRS NAMES libfreenect.hpp
    HINTS ${libfreenect_inc_DIR})

message("libfreenct_include_dirs:${libfreenect_INCLUDE_DIRS}")

find_library(libfreenect_LIBRARIES NAMES freenect
    HINTS /usr/local/lib)

message("libfreenect_lib_dirs:${libfreenect_LIBRARIES}")

if(libfreenect_LIBRARIES AND libfreenect_INCLUDE_DIRS)
    set(libfreenect_FOUND 1)
endif()
