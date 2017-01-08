project(winca2plugin)



set(winca2plugin_PLATFORM_DEPS
	w32-pthreads)

set(winca2plugin_SOURCES
	win32.c
	obs-slideshow.c)

		
add_library(winca2plugin MODULE
	${winca2plugin_SOURCES})

target_link_libraries(winca2plugin
	libobs
	${winca2plugin_PLATFORM_DEPS})

install_obs_plugin_with_data(winca2plugin data)



