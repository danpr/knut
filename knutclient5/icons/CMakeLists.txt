##4###kde4_install_icons(${ICON_INSTALL_DIR} ) 


include(ECMInstallIcons)
ecm_install_icons (ICONS
	16-apps-knutclient.png
	22-apps-knutclient.png
	32-apps-knutclient.png
	48-apps-knutclient.png
#	lo16-app-knutclient.png
#	lo32-app-knutclient.png
       	DESTINATION ${KDE_INSTALL_ICONDIR} THEME hicolor)

