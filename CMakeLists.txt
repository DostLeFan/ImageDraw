cmake_minimum_required(VERSION 3.25)

project(ImageDraw VERSION 0.0.1 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(IMAGEDRAW_LIB_NAME ImageDraw-lib)
set(IMAGEDRAW_TESTS_NAME ImageDraw_tests)

include(GNUInstallDirs)

option(IMAGEDRAW_BUILD_TEST "Build unit tests" OFF)

add_subdirectory(lib)

if(IMAGEDRAW_BUILD_TEST)
	enable_testing()
	add_subdirectory(tests)
endif()

# CPack/CMakePackageConfigHelpers
include(CMakePackageConfigHelpers)

write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/ImageDrawConfigVersion.cmake"
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake/ImageDrawConfig.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/ImageDrawConfig.cmake"
	INSTALL_DESTINATION
		${CMAKE_INSTALL_LIBDIR}/cmake/ImageDraw-lib
)

install(
	FILES
		"${CMAKE_CURRENT_BINARY_DIR}/ImageDrawConfig.cmake"
		"${CMAKE_CURRENT_BINARY_DIR}/ImageDrawConfigVersion.cmake"
	DESTINATION
		${CMAKE_INSTALL_LIBDIR}/cmake/ImageDraw-lib
)