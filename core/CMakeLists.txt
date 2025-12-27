include(MDEBase)
include(MDEPkgConfig)

mde_setup_library(MDECore ${CMAKE_INSTALL_LIBDIR})
if(NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
	target_link_libraries(
		MDECore
		PRIVATE
		pthread
	)
endif()
