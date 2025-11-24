setup_lib(audio Audio)

find_package(PkgConfig)

macro(check_backend uppername name)
	if(NOT BACKEND_FOUND)
		pkg_check_modules(${uppername} ${name})
		if(${uppername}_FOUND)
			target_include_directories(
				MDEAudio
				PRIVATE
				${${uppername}_INCLUDE_DIRS}
			)
			target_compile_options(
				MDEAudio
				PRIVATE
				${${uppername}_CFLAGS_OTHER}
			)
			target_link_directories(
				MDEAudio
				PRIVATE
				${${uppername}_LIBRARY_DIRS}
			)
			target_link_options(
				MDEAudio
				PRIVATE
				${${uppername}_LDFLAGS_OTHER}
			)
			target_link_libraries(
				MDEAudio
				PRIVATE
				${${uppername}_LIBRARIES} pthread
			)
			target_compile_definitions(
				MDEAudio
				PRIVATE
				USE_${uppername}
			)
			set(BACKEND_FOUND 1)
		endif()
	endif()
endmacro()

check_backend(ALSA alsa)
check_backend(AO ao)
