ADD_DEFINITIONS( -DQT_NO_DEBUG_OUTPUT )

add_definitions(-DTRANSLATION_DOMAIN=\"knutclient\")

cmake_policy(SET CMP0063 NEW)


include_directories( ${QT_INCLUDES})

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



#kf5_add_kdeinit_executable(knutclient ${knutclient_SRCS})
add_executable(knutclient ${knutclient_SRCS})


target_link_libraries(knutclient
    ##    Qt5::Quick
    ##    Qt5::Qt5Gui
    Qt5::Widgets
    KF5::CoreAddons
    KF5::I18n
    KF5::DBusAddons
    KF5::Crash
    KF5::KDELibs4Support
    KF5::WidgetsAddons
    KF5::Completion
    KF5::XmlGui
    KF5::Notifications
    KF5::WindowSystem
    KF5::IconThemes   
    KF5::ConfigCore
    KF5::ConfigGui
    KF5::I18n
    KF5::ConfigWidgets
    ## KF5::QuickAddons
    ##    PW::KWorkspace
    ##    KF5::Declarative
)


install(TARGETS knutclient DESTINATION ${BIN_INSTALL_DIR} )


########### install files ###############

#install( FILES knutclientui.rc  DESTINATION  ${DATA_INSTALL_DIR}/knutclient )
install( FILES knutclientui.rc  DESTINATION  ${KXMLGUI_INSTALL_DIR}/knutclient )

