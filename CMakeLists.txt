cmake_minimum_required(VERSION 3.10)
project(interpolate VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(INTERPOLATE_USE_SYSTEM_EIGEN "Prefer system/vcpkg Eigen if available" ON)
option(INTERPOLATE_BUILD_TESTS "Build the interpolate tests" ON)

set(EIGEN_FOUND_LOCAL FALSE)
if (INTERPOLATE_USE_SYSTEM_EIGEN)
	find_package(Eigen3 QUIET CONFIG)
	if (NOT Eigen3_FOUND)
		find_package(Eigen3 QUIET MODULE)
	endif()
endif()

if (NOT Eigen3_FOUND)
	message(STATUS "Eigen3 not found system-wide; using vendored Eigen submodule")
	set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
	add_library(Eigen3::Eigen INTERFACE IMPORTED)
	set_target_properties(Eigen3::Eigen PROPERTIES
		INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/vendor/eigen"
	)
	set(EIGEN_FOUND_LOCAL TRUE)
else()
	message(STATUS "Using system/vcpkg Eigen: ${Eigen3_DIR}")
endif()

add_compile_options(
	# MSVC and clang-cl
	"$<$<AND:$<COMPILE_LANGUAGE:CXX>,$<OR:$<CXX_COMPILER_ID:MSVC>,$<CXX_COMPILER_FRONTEND_VARIANT:MSVC>>>:/utf-8>"
	# GCC
	"$<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CXX_COMPILER_ID:GNU>>:-finput-charset=UTF-8>"
	"$<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CXX_COMPILER_ID:GNU>>:-fexec-charset=UTF-8>"
)

file(GLOB_RECURSE INTERPOLATE_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

file(GLOB_RECURSE INTERPOLATE_HEADERS
	"${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
)

add_library(interpolate
	${INTERPOLATE_SOURCES}
	${INTERPOLATE_HEADERS}
)

target_include_directories(interpolate
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
)

target_link_libraries(interpolate
	PUBLIC
		Eigen3::Eigen
)

source_group(
	TREE "${CMAKE_CURRENT_SOURCE_DIR}/include"
	PREFIX "Header Files"
	FILES ${INTERPOLATE_HEADERS}
)
source_group(
	TREE "${CMAKE_CURRENT_SOURCE_DIR}/src"
	PREFIX "Source Files"
	FILES ${INTERPOLATE_SOURCES}
)

install(TARGETS interpolate
	EXPORT interpolateTargets
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
	RUNTIME DESTINATION bin
)

install(DIRECTORY include/
    DESTINATION include
)

if (INTERPOLATE_BUILD_TESTS)
	add_subdirectory(test)
endif()
