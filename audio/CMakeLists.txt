setup_lib(audio Audio)

find_package(PkgConfig)

if(NOT BACKEND_FOUND)
	pkg_check_modules(AO ao)
	if(AO_FOUND)
		target_include_directories(
			MDEAudio
			PRIVATE
			${AO_INCLUDE_DIRS}
		)
		target_link_directories(
			MDEAudio
			PRIVATE
			${AO_LIBRARY_DIRS}
		)
		target_link_libraries(
			MDEAudio
			PRIVATE
			${AO_LIBRARIES} pthread
		)
		target_compile_definitions(
			MDEAudio
			PRIVATE
			USE_AO
		)
		set(BACKEND_FOUND 1)
	endif()
endif()

if(NOT BACKEND_FOUND)
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
		target_compile_definitions(
			MDEAudio
			PRIVATE
			USE_ALSA
		)
		set(BACKEND_FOUND 1)
	endif()
endif()
