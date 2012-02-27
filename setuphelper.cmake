
# this script is used to include the library in a project

set(setuphelper_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/setuphelper)
set(setuphelper_DLL "setuphelper.dll")
set(setuphelper_DLL_DIR_DEBUG "${CMAKE_CURRENT_LIST_DIR}/_bin/${outdir_cfg_debug}")
set(setuphelper_DLL_DIR_RELEASE "${CMAKE_CURRENT_LIST_DIR}/_bin/${outdir_cfg_release}")

add_library(setuphelper SHARED IMPORTED)

set_property(TARGET setuphelper PROPERTY IMPORTED_LOCATION_DEBUG "${setuphelper_DLL_DIR_DEBUG}/${setuphelper_DLL}")
set_property(TARGET setuphelper PROPERTY IMPORTED_LOCATION_RELEASE "${setuphelper_DLL_DIR_RELEASE}/${setuphelper_DLL}")

set(dependecy_list_debug ${dependecy_list_debug} "${setuphelper_DLL_DIR_DEBUG}/${setuphelper_DLL}" "${CMAKE_CURRENT_LIST_DIR}/setuphelper/helpercode.isi")
set(dependecy_list_release ${dependecy_list_release} "${setuphelper_DLL_DIR_RELEASE}/${setuphelper_DLL}" "${CMAKE_CURRENT_LIST_DIR}/setuphelper/helpercode.isi")
