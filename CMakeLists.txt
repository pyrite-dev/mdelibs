cmake_minimum_required(VERSION 3.25)
project(
	mdelibs
)

include(GNUInstallDirs)

add_subdirectory(core)
add_subdirectory(audio)
add_subdirectory(sound)

install(
	DIRECTORY include/
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	FILES_MATCHING PATTERN "*.h"
)
