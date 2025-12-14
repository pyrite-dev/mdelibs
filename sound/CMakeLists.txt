include(MDEBase)
include(MDEPkgConfig)

setup_library(MDESound ${CMAKE_INSTALL_LIBDIR})

pkg_config_add(MDESound vorbisfile)
