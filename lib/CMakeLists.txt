include(FetchContent)
 
message(STATUS "Downloading stb_image_write.h...")
 
FetchContent_Declare(
	stb_image_write
	URL https://raw.githubusercontent.com/nothings/stb/31c1ad37456438565541f4919958214b6e762fb4/stb_image_write.h
	DOWNLOAD_NO_EXTRACT TRUE
)
 
FetchContent_MakeAvailable(stb_image_write)
 
add_library(stb INTERFACE)
target_include_directories(stb SYSTEM INTERFACE ${stb_image_write_SOURCE_DIR})
 
message(STATUS "stb_image_write.h downloaded.")


# List sources.

file(GLOB_RECURSE IMAGEDRAW_SOURCES CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE IMAGEDRAW_HEADERS CONFIGURE_DEPENDS "include/*.hpp")

add_library(${IMAGEDRAW_LIB_NAME} ${IMAGEDRAW_SOURCES} ${IMAGEDRAW_HEADERS})
add_library(ImageDraw::ImageDraw-lib ALIAS ${IMAGEDRAW_LIB_NAME})

target_compile_features(${IMAGEDRAW_LIB_NAME} PUBLIC cxx_std_17)
target_include_directories(${IMAGEDRAW_LIB_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_link_libraries(${IMAGEDRAW_LIB_NAME} PRIVATE stb)

if(MSVC)
	target_compile_options(${IMAGEDRAW_LIB_NAME} PRIVATE /W4 /permissive-)
else()
	target_compile_options(${IMAGEDRAW_LIB_NAME} PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Wshadow)
endif()

if(WIN32)
	target_compile_definitions(${IMAGEDRAW_LIB_NAME} PRIVATE NOMINMAX WIN32_LEAN_AND_MEAN)
endif()

install(
	TARGETS ${IMAGEDRAW_LIB_NAME}
	EXPORT ImageDrawTargets
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
	DIRECTORY
		${CMAKE_CURRENT_SOURCE_DIR}/include/
	DESTINATION
		${CMAKE_INSTALL_INCLUDEDIR}
)