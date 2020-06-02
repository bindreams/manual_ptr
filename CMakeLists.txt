cmake_minimum_required(VERSION 3.13)
project(manual_ptr LANGUAGES CXX VERSION 0.1.0)

include("${CMAKE_CURRENT_SOURCE_DIR}/tools/cmake/install.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/tools/cmake/static_analysis.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/tools/cmake/warnings.cmake")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

# Tooling
#enable_clang_tidy()
#enable_iwyu()
set_warnings(TREAT_AS_ERRORS)

# Project ==============================================================================================================
set(target_name ${PROJECT_NAME})
set(namespace zh)

option(${PROJECT_NAME}_BUILD_TESTS "Build project tests." OFF)
option(${PROJECT_NAME}_BUILD_EXAMPLES "Build project examples." OFF)

# Sources
file(GLOB_RECURSE sources RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/src" CONFIGURE_DEPENDS
	"src/*.cpp"
)

# Target
add_library(${target_name} INTERFACE)
add_library(${namespace}::${target_name} ALIAS ${target_name})

foreach(source ${sources})
	target_sources(${target_name} INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/${source}>)
	target_sources(${target_name} INTERFACE $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include/${source}>)
endforeach()
target_compile_features(${target_name} INTERFACE cxx_std_17)
target_include_directories(${target_name}
	INTERFACE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
		$<INSTALL_INTERFACE:include>
)

# Installation
install_targets(${target_name})
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src/"
	DESTINATION include
	COMPONENT Devel
)

# Generate configuration files and install the package
configure_and_install(${CMAKE_SCRIPTS_ROOT}/simple-config.cmake.in ${namespace} SameMajorVersion)

# Tests ================================================================================================================
if(${PROJECT_NAME}_BUILD_TESTS)
	message(STATUS "Building ${PROJECT_NAME} tests")

	find_package(doctest REQUIRED)

	file(GLOB_RECURSE sources CONFIGURE_DEPENDS
		"tests/*.cpp"
	)

	add_executable(${target_name}_tests)
	target_sources(${target_name}_tests PRIVATE ${sources})

	target_link_libraries(${target_name}_tests
		PRIVATE
			${namespace}::${target_name}
			doctest::doctest
	)
endif()

# Examples =============================================================================================================
#[[if(${PROJECT_NAME}_BUILD_EXAMPLES)
	message(STATUS "Building ${PROJECT_NAME} examples")

	add_executable(${target_name}_example)
	target_sources(${target_name}_example PRIVATE "examples/example.cpp")
	target_link_libraries(${target_name}_example PRIVATE ${namespace}::${target_name})
endif()]]