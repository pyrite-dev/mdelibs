cmake_minimum_required(VERSION 3.25)
project(
	mdelibs
)

include(GNUInstallDirs)
macro(setup_lib dir name)
	file(
		GLOB
		MDE${name}_SRC
		src/${dir}/*.c
	)

	add_library(
		MDE${name}
		SHARED
		${MDE${name}_SRC}
	)

	target_include_directories(
		MDE${name}
		PUBLIC
		include
	)

	install(
		TARGETS MDE${name}
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	)
endmacro()

setup_lib(core Core)
setup_lib(audio Audio)

find_package(PkgConfig)
pkg_check_modules(ALSA alsa)
if(ALSA_FOUND)
	target_include_directories(
		MDEAudio
		PRIVATE
		${ALSA_INCLUDE_DIRS}
	)
	target_link_directories(
		MDEAudio
		PRIVATE
		${ALSA_LIBRARY_DIRS}
	)
	target_link_libraries(
		MDEAudio
		PRIVATE
		${ALSA_LIBRARIES} pthread
	)
	target_sources(
		MDEAudio
		PRIVATE
		src/audio/driver/alsa.c
	)
endif()

install(
	DIRECTORY include/
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	FILES_MATCHING PATTERN "*.h"
)
