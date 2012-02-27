file(GLOB_RECURSE source_files *.cpp *.h *.def *.rc)

add_library(setuphelper SHARED ${source_files})

add_definitions("-D_CRT_SECURE_NO_WARNINGS -D_UNICODE")

set_target_properties(setuphelper PROPERTIES LINK_FLAGS "Shlwapi.lib")

set_property(TARGET setuphelper PROPERTY RUNTIME_OUTPUT_DIRECTORY_RELEASE ${outdir_bin_release})
set_property(TARGET setuphelper PROPERTY RUNTIME_OUTPUT_DIRECTORY_DEBUG ${outdir_bin_debug})
set_property(TARGET setuphelper PROPERTY LIBRARY_OUTPUT_DIRECTORY_RELEASE ${outdir_bin_release})
set_property(TARGET setuphelper PROPERTY LIBRARY_OUTPUT_DIRECTORY_DEBUG ${outdir_bin_debug})
set_property(TARGET setuphelper PROPERTY ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${outdir_lib_release})
set_property(TARGET setuphelper PROPERTY ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${outdir_lib_debug})
