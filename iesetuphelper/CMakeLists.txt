file(GLOB_RECURSE source_files *.cpp *.h *.def *.rc generate-iss.js)

add_library(iesetuphelper SHARED ${source_files})

add_definitions("-D_CRT_SECURE_NO_WARNINGS -D_UNICODE -DPSAPI_VERSION=1")

set_target_properties(iesetuphelper PROPERTIES LINK_FLAGS "Shlwapi.lib Psapi.lib")

set_property(TARGET iesetuphelper PROPERTY RUNTIME_OUTPUT_DIRECTORY_RELEASE ${outdir_bin_release})
set_property(TARGET iesetuphelper PROPERTY RUNTIME_OUTPUT_DIRECTORY_DEBUG ${outdir_bin_debug})
set_property(TARGET iesetuphelper PROPERTY LIBRARY_OUTPUT_DIRECTORY_RELEASE ${outdir_bin_release})
set_property(TARGET iesetuphelper PROPERTY LIBRARY_OUTPUT_DIRECTORY_DEBUG ${outdir_bin_debug})
set_property(TARGET iesetuphelper PROPERTY ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${outdir_lib_release})
set_property(TARGET iesetuphelper PROPERTY ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${outdir_lib_debug})

add_custom_command(
	TARGET iesetuphelper
	POST_BUILD
	COMMAND cscript generate-iss.js
	WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
	)
