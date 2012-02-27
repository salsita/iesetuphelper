
# this script is used to include the library in a project

set(iesetuphelper_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/iesetuphelper)
set(iesetuphelper_DLL "iesetuphelper.dll")
set(iesetuphelper_DLL_DIR_DEBUG "${CMAKE_CURRENT_LIST_DIR}/_bin/${outdir_cfg_debug}")
set(iesetuphelper_DLL_DIR_RELEASE "${CMAKE_CURRENT_LIST_DIR}/_bin/${outdir_cfg_release}")

add_library(iesetuphelper SHARED IMPORTED)

set_property(TARGET iesetuphelper PROPERTY IMPORTED_LOCATION_DEBUG "${iesetuphelper_DLL_DIR_DEBUG}/${iesetuphelper_DLL}")
set_property(TARGET iesetuphelper PROPERTY IMPORTED_LOCATION_RELEASE "${iesetuphelper_DLL_DIR_RELEASE}/${iesetuphelper_DLL}")

set(dependecy_list_debug ${dependecy_list_debug} "${iesetuphelper_DLL_DIR_DEBUG}/${iesetuphelper_DLL}" "${CMAKE_CURRENT_LIST_DIR}/iesetuphelper/helpercode.isi")
set(dependecy_list_release ${dependecy_list_release} "${iesetuphelper_DLL_DIR_RELEASE}/${iesetuphelper_DLL}" "${CMAKE_CURRENT_LIST_DIR}/iesetuphelper/helpercode.isi")
