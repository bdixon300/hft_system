# GoogleTest (gtest)
# A unit testing library for C/C++
# Creates a libgtest target packaged with the required include driectories
find_package(Threads REQUIRED)
include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.17.0  # or whatever tag you want
)

FetchContent_MakeAvailable(googletest)

# add_library(libgtest IMPORTED STATIC GLOBAL)
# add_dependencies(libgtest gtest)

# add_library(libgtest, gtest)
#target_link_libraries(libgtest gtest)

# Setup the build tree and package the target
# file(MAKE_DIRECTORY ${SOURCE_DIR}/include)
# set_target_properties(libgtest PROPERTIES
#     "IMPORTED_LOCATION" "${BINARY_DIR}/libgtest.a"
#     "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
#     "INTERFACE_INCLUDE_DIRECTORIES" "${SOURCE_DIR}/include"
# )
