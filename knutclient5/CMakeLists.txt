project(knutclient)
cmake_minimum_required(VERSION 3.0)

####### ECM = Extra CMake Modules ==========
find_package(ECM 0.0.11 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
    set(DEPRECATION_FLAG "-wr1478")
else()
    set(DEPRECATION_FLAG "-Wno-deprecated-declarations")
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${DEPRECATION_FLAG}")

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)
include(FeatureSummary)

find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets Network )
###find_package(KF5 REQUIRED COMPONENTS CoreAddons I18n DocTools WidgetsAddons Completion XmlGui Notifications WindowSystem IconThemes Config ConfigWidgets)
find_package(KF5 REQUIRED COMPONENTS CoreAddons KDELibs4Support I18n DocTools WidgetsAddons Completion XmlGui Notifications WindowSystem IconThemes Config ConfigWidgets)

add_subdirectory(src)
add_subdirectory(pics)
add_subdirectory(icons)
add_subdirectory(po)
add_subdirectory(doc)
########### install files ###############

###install( FILES knutclient.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES knutclient.desktop DESTINATION ${XDG_APPS_INSTALL_DIR} ) 
install( FILES knutclient.notifyrc DESTINATION ${DATA_INSTALL_DIR}/knutclient)
