project(knutclient)
cmake_minimum_required(VERSION 2.6)

find_package(KDE4 REQUIRED)
find_package(Qt4 REQUIRED )


add_subdirectory(src)
add_subdirectory(pics)
add_subdirectory(icons)
add_subdirectory(po)
add_subdirectory(doc)
########### install files ###############

###install( FILES knutclient.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES knutclient.desktop DESTINATION ${XDG_APPS_INSTALL_DIR} ) 
install( FILES knutclient.notifyrc DESTINATION ${DATA_INSTALL_DIR}/knutclient)
