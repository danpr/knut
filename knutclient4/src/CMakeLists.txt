add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
ADD_DEFINITIONS( -DQT_NO_DEBUG_OUTPUT )

include (KDE4Defaults)
include_directories( ${KDE4_INCLUDES} ${KDE4_INCLUDE_DIR} ${QT_INCLUDES})

set(knutclient_SRCS
   knutclient.cpp
   main.cpp
   knutupsdata.cpp
   knutvardata.cpp 
   knutnet.cpp
   knutfrontpanel.cpp
   knutpanelmeter.cpp
   knutanalog.cpp
   knutnewupsdlg.cpp
   knutprefdlg.cpp
   knutdock.cpp
   knutmainwindow.cpp 
   knutprintupsvar.cpp
   knutrwvar.cpp
   knutinstcomms.cpp
 )

#kde4_automoc(${knutclient_SRCS})

#kde4_add_kcfg_files(knutclient_SRCS settings.kcfgc )

kde4_add_executable(knutclient ${knutclient_SRCS})

target_link_libraries(knutclient
                      ${KDE4_KDEUI_LIBS}
                      ${KDE4_KDECORE_LIBS}
                      ${QT_QTNETWORK_LIBRARIES}
)

install(TARGETS knutclient DESTINATION ${BIN_INSTALL_DIR} )


########### install files ###############

#install( FILES knutclient.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES knutclientui.rc  DESTINATION  ${DATA_INSTALL_DIR}/knutclient )

