cmake_minimum_required(VERSION 3.16)

project(panon VERSION 6.0.1)

find_package(ECM 1.4.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

find_package(Plasma REQUIRED VERSION 6)
find_package(KF6 REQUIRED COMPONENTS I18n)

add_subdirectory(shaders)
add_subdirectory(translations)

install(DIRECTORY plasmoid DESTINATION .)

set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_GENERATOR "ZIP")
set(CPACK_PACKAGE_FILE_NAME ${CMAKE_PROJECT_NAME}-${PROJECT_VERSION})
include(CPack)
