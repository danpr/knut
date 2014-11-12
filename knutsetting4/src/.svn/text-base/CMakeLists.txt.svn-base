add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
#add_definitions (-DQT3_SUPPORT -DQT3_SUPPORT_WARNINGS)
#ADD_DEFINITIONS( -DQT_NO_DEBUG_OUTPUT )

include (KDE4Defaults ${QT_USE_QT3SUPPORT})
include_directories( ${KDE4_INCLUDES} ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${QT_QT3SUPPORT_INCLUDE_DIR})

set(kcm_knutsetting_SRCS
   knsstring.cpp
   knscommondata.cpp
   knsintlineedit.cpp
   knsintaddress.cpp
   knsacladdress.cpp
   knsacl.cpp
   knsintaddress.cpp
   knsvalueedit.cpp
   knsmoddrivval.cpp
   knsaddparam.cpp
   knsmodusrval.cpp
   knsmodstdval.cpp
   knsmodulesedit.cpp 
   knsupssaxhandler.cpp
   knsaddups.cpp
   knsmodules.cpp 
   knsdoublelistbox.cpp
   knsuseredit.cpp
   knsusers.cpp
   knsmonvalues.cpp
   knsmonnotify.cpp
   knsmonsystems.cpp
   knsmonitoring.cpp
   knsnet.cpp
   knsprocesinfo.cpp
   knssaxhandler.cpp
   knutsetting.cpp
#   main.cpp
 )


	

kde4_add_plugin(kcm_knutsetting ${kcm_knutsetting_SRCS})

#######
kde4_add_executable(kcmknutsettinghelper knshelper.cpp ${knshelper_mocs})
target_link_libraries(kcmknutsettinghelper ${KDE4_KDECORE_LIBS})
install(TARGETS kcmknutsettinghelper DESTINATION ${LIBEXEC_INSTALL_DIR})
kde4_install_auth_helper_files(kcmknutsettinghelper org.kde.kcontrol.knutsetting root)
#######


###########

target_link_libraries(kcm_knutsetting
                      ${KDE4_KDEUI_LIBS}
                      ${KDE4_KDECORE_LIBS}
                      ${KDE4_KIO_LIBS}
                      ${QT_QT3SUPPORT_LIBRARY}
)

install(TARGETS kcm_knutsetting DESTINATION ${PLUGIN_INSTALL_DIR} )


########### install files ###############

#install( FILES knutsettingui.rc  DESTINATION  ${DATA_INSTALL_DIR}/knutsetting )
#install(TARGETS kcmknutsettinghelper DESTINATION ${LIBEXEC_INSTALL_DIR})
