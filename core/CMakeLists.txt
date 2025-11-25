setup_lib(core Core)
target_link_libraries(
	MDECore
	PRIVATE
	Mw
)
if(NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
	target_link_libraries(
		MDECore
		PRIVATE
		pthread
	)
endif()
