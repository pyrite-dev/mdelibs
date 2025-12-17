include(MDEBase)
include(MDEPkgConfig)

mde_setup_library(MDESound ${CMAKE_INSTALL_LIBDIR})

mde_pkg_config_add(MDESound vorbisfile)
