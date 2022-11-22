cmake_minimum_required(VERSION 3.22)

project(
	CLibUtil
	VERSION 1.2.2
	LANGUAGES CXX
)

# ---- Include guards ----

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
	message(
		FATAL_ERROR
			"In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
)
endif()

# ---- Add source files ----

include(cmake/headerlist.cmake)

source_group(
	TREE
		${CMAKE_CURRENT_SOURCE_DIR}
	FILES
		${headers}
)

# ---- Create library ----

include(GNUInstallDirs)

add_library(
	${PROJECT_NAME}
	INTERFACE
	${headers}
	.clang-format
)

add_library(
	${PROJECT_NAME}::${PROJECT_NAME} 
	ALIAS 
	${PROJECT_NAME}
)

target_include_directories(
	${PROJECT_NAME}
	INTERFACE
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
	"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_compile_features(
	${PROJECT_NAME}
	INTERFACE
		cxx_std_23
)

# ---- Create an installable target ----

install(
	TARGETS ${PROJECT_NAME}
	EXPORT ${PROJECT_NAME}-targets
)

install(
	DIRECTORY include
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/include
)

configure_file(
	cmake/config.cmake.in
	${PROJECT_NAME}Config.cmake
	@ONLY
)

install(
	FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

install(
	EXPORT ${PROJECT_NAME}-targets
	NAMESPACE ${PROJECT_NAME}::
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)