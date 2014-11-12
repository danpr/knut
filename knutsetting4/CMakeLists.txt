project(knutsetting)
cmake_minimum_required(VERSION 2.6)


find_package(KDE4 REQUIRED)
find_package(Qt4 REQUIRED )

kde4_install_auth_actions(org.kde.kcontrol.knutsetting knutsetting.actions)

add_subdirectory(src)
add_subdirectory(pics)
add_subdirectory(icons)
add_subdirectory(xmlconf)
add_subdirectory(po)
#-#add_subdirectory(doc)
########### install files ###############

install( FILES knutsetting.desktop DESTINATION ${SERVICES_INSTALL_DIR} ) 
install( FILES dist.conf DESTINATION ${DATA_INSTALL_DIR}/knutsetting/ ) 
