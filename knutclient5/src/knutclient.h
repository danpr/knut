/***************************************************************************
 *   Copyright (C) 2009 by Daniel Prynych,,,                               *
 *   Daniel@prynych.cz                                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef KNUTCLIENT_H
#define KNUTCLIENT_H


#include "knutvardata.h"
#include "knutupsdata.h"
#include "knutnet.h"
#include "knutdock.h"
#include "knutprefdlg.h"
#include "knutmainwindow.h"
#include "knutprintupsvar.h"
#include "knutrwvar.h"
#include "knutinstcomms.h"


#include <QSessionManager>
#include <QTimer>
#include <QWidget>
#include <QStringList>
#include <QMutex>



  const int countNextConnDelay = 10;
  const int nextConnDelay[] = {
  5, 5, 20, 30, 60, 60, 120, 300, 600, 900 };

/**
 * This class have to been QWidget because sysTray parent is not QWidget class
 * and exec function from KPageDialog/KDialog/QDialog ends application when is main widget.
 * In Qt sysTray parent was QWidget
 *
 *
 **/

/**class KNutClient : public QWidget, public QSessionManager  { **/
class KNutClient : public QWidget  {
    Q_OBJECT
public:
    KNutClient(bool noDock = false, QWidget* parent=0);
    virtual ~KNutClient();



private Q_SLOTS:

/**
 *
 * Calls function for repainting of icon in dock.
 * Vola funkci pro prekresleni icony v doku.
 *
 * @since  0.2
 **/
    void slotTryFirstConnection (int );


/**
 * This function is called when program repeats other connection;
 * Calls function for repainting of icon in dock.
 * Tata funkce je volana, kdyz program opakuje pokus o pripojeni.
 * Vola funkci pro prekresleni icony v doku.
 *
 * @since  0.2
 **/
    void slotTryReepatFirstConnection (int number );

/**
 * Nastavi hodnoty po prvem pripojeni na UPS, zavola prekresleni docku a  nastartuje timer
 * Sets values after the first conection to UPS ,calls to repaint of dock and starts timer
 *
 * @since  0.2
 **/
    void slotFirstConnection (void);

/**
 * Zpracuje chybu spojeni
 * Executes error of connection
 *
 * @since  0.1
 **/
    void slotErrorConnection (int error);

/**
 * Zpracuje ukonceni spojeni, pokud spojeni bylo ukonceno druhou stranou.
 * Executes end of connection, when connection was closed by the second side.
 *
 * @since  0.1
 **/
    void slotConnectionClosedByServer(void);

    void slotTimeout (void);


/**
 * Makes new connection on ups server (upsd).
 **/
    void slotReconnectTimeout (void);

/**
 * Makes referesh connection.
 * Intervals between connection is 5 , 5, 20, 30, 60, 60, 120, 300, 600, 900 sec.
 *
 * @since  0.1
 **/
    void slotRefreshConnection(void);

    void slotSetNewUPS (const QString name);

    void slotPreferenceUps (void);
    void slotSetConfig (unsigned int changeSetting);

    void slotPrintUPSVars(void);
    void slotInstComms(void);
    void slotRWVars(void);


    void slotCloseMainWindow (void);
    void slotMainWindowDestroyed (void);

    void slotActiveMainWindow (void);

    void slotExitApplication(void);

    void slotSaveConf (void);

/**
 * Send messages to status line of mainWidow and to KDE messages viewer.
 *
 * @since  0.1
 **/
    void slotSendMessage (const QString &message, const QString &event);

private:

/**
 * Prepares and show main window.
 * Pripravi a zobrazi hlavni okno.
 *
 * @since  0.1
 **/
    void makeMainWindow (void);


    virtual bool commitData (QSessionManager&);
    virtual bool saveState (QSessionManager&);


/**
 * Sets all variables to initial values.
 * Nastavi vsechny vsechny promenne na pocatecni hodnoty.
 *
 * @since  0.1
 **/
    void initVars (void);

    void loadConfiguration ( void );

/**
 * Runs saveCommonConfiguration and savePreferencesConfiguration.
 * Saves all configurations informations and call syncConfiguration.
 *
 * @since  0.1
 **/
    void saveAllConfiguration ( void);



/**
 * Saves common configuration positions of main window, name of active ups 
 *
 * @since  0.1
 **/
    void saveCommonConfiguration ( void );

/**
 * Saves configuration about proferences dialog .. fonts, colors, info about upses ...
 *
 * @since  0.1
 **/
    void savePreferencesConfiguration ( void );

    void syncConfiguration ( void );

    void getUpsData (void);

/**
 * Sets selected ups vars like active, when main window is existed, procedure sets
 * ups variables ares showed in main window like active. Other ups variables will be
 * setted like unactive
 *
 * @since  0.1
 **/
    void setActiveUpsVars (bool setVarsForMainWindowAlways = false );

    void setActiveXFerVars (bool activate);

    void upsDeactivate (void);

    void setActiveUpsInfo (const QString &name);

    void addEventString (const QString text, const QString UPSAddress = QString::null);

    void showStatusEvent(void);


    KNutVarData::upsInfoDef m_mainUpsInfo;

    // v teto strukture jsou ulozeny promene ktere nastavuji konfiguraci
    // in this structure are saveing values, which setting configuration
    KNutVarData::SettingDataDef settingData;


    KNutDock *m_myDock;
    KNutMainWindow *mainWindow;


    KNutUpsData *upsRecords;
    KNutVarData *analogRecords;

    QTimer *m_upsTimer;
    QTimer *m_reconnectTimer;

    QStringList *m_listKNutEvent;

    bool m_panelsAreZeroing; //   indikuje zda je panel nulovany // use if analog panels is setting to zero

    //  kod posledni chyby spojeni zjistene v timeoutu
    //  nebere ohled zda byla chyba pri inicializaci
    int m_lastError; //code off last error from timeout

    // mutex for reconnect, only one reconnect at a time
    QMutex m_reconnectMutex;
    QMutex m_upsDeactivateMutex;


};

#endif // _knutclient_H_
