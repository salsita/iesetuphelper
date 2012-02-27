
cmake_minimum_required(VERSION 2.8.7)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

project("setuphelper")

if(CMAKE_CL_64)
	set(outdir_cfg_release Release64)
	set(outdir_cfg_debug Debug64)
else(CMAKE_CL_64)
	set(outdir_cfg_release Release32)
	set(outdir_cfg_debug Debug32)
endif(CMAKE_CL_64)

set(outdir_bin_release ${CMAKE_CURRENT_SOURCE_DIR}/_bin/${outdir_cfg_release}/)
set(outdir_bin_debug ${CMAKE_CURRENT_SOURCE_DIR}/_bin/${outdir_cfg_debug}/)
set(outdir_lib_release ${CMAKE_CURRENT_SOURCE_DIR}/_lib/${outdir_cfg_release}/)
set(outdir_lib_debug ${CMAKE_CURRENT_SOURCE_DIR}/_lib/${outdir_cfg_debug}/)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

if($ENV{VERSION_BUILD})
	add_definitions("-DVER_NUMBER_BUILD=$ENV{VERSION_BUILD}")
endif($ENV{VERSION_BUILD})

add_subdirectory(setuphelper)
