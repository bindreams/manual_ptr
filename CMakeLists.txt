cmake_minimum_required(VERSION 3.13)
project(manual_ptr)

# Set up Conan package manager =========================================================================================
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# Compiler settings ====================================================================================================
set(CMAKE_CXX_STANDARD 17)
include_directories("src")

# Glob sources =========================================================================================================
file(GLOB sources_test "test/*.cpp")

# Targets ==============================================================================================================
add_library   (${PROJECT_NAME} INTERFACE)
add_executable(${PROJECT_NAME}_test ${sources_test})

# Linking ==============================================================================================================
target_link_libraries(${PROJECT_NAME}_test ${CONAN_LIBS})