cmake_minimum_required(VERSION 3.25)
project(
	mdelibs
)

include(GNUInstallDirs)
macro(setup_lib dir name)
	project(
		mde${dir}
	)

	file(
		GLOB
		MDE${name}_SRC
		*.c
	)

	add_library(
		MDE${name}
		SHARED
		${MDE${name}_SRC}
	)

	target_include_directories(
		MDE${name}
		PUBLIC
		../include
	)

	install(
		TARGETS MDE${name}
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	)
endmacro()

add_subdirectory(core)
add_subdirectory(audio)
add_subdirectory(sound)

install(
	DIRECTORY include/
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	FILES_MATCHING PATTERN "*.h"
)
