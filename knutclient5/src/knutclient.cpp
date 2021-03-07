/***************************************************************************
 *   Copyright (C) 2009 by Daniel Prynych,,,                               *
 *   Daniel at prynych.cz                                                  *
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


#include "knutclient.h"
#include "knutnet.h"
#include "knutupsdata.h"
#include "knutanalog.h"
#include "knutfrontpanel.h"
#include "knutnewupsdlg.h"
#include "knutdock.h"


#include <KMessageBox>
#include <KWindowSystem>
#include <KNotification>
#include <KLocalizedString>
#include <KSharedConfig>

#include <QApplication>
#include <QDropEvent>
#include <QPainter>
#include <QColor>
#include <QFont>



#include <unistd.h>  // for function sleep and other ...

//  const char *nameUpsVars1[] = {
//  "NONE", "UTILITY", "BATTPCT", "UPSTEMP", "ACFREQ", "LOADPCT", "AMBTEMP", "AMBHUMID","BATTVOLT","OUTVOLT",CURRENT};
//  const char *nameUpsVars2[] = {
//    "none", "input.voltage", "battery.charge", "ups.temperature", "input.frequency", "ups.load", "ambient.temperature",
//"ambient.humidity","battery.voltage","output.voltage","output.current","battery.temperature","battery.current"};


KNutClient::KNutClient(bool noDock, QWidget* parent) : QWidget(parent) {
  
  qDebug ("KnutClient::Constructor");

  qApp->setQuitOnLastWindowClosed(false);
  upsRecords = new KNutUpsData();
  analogRecords = new KNutVarData();

  m_upsTimer = new QTimer (this);
  m_upsTimer->setSingleShot( true );
  connect( m_upsTimer, SIGNAL(timeout()), SLOT(slotTimeout()) );

  m_reconnectTimer = new QTimer(this);
  connect( m_reconnectTimer, SIGNAL(timeout()), SLOT(slotReconnectTimeout()) );

  //  data for combo
  m_listKNutEvent = new QStringList();

  // init and load of configuration
  initVars();

  loadConfiguration();

  if (!noDock) {

     m_myDock = new KNutDock(&m_mainUpsInfo,&settingData,upsRecords);

     
     m_myDock->setStatus(KStatusNotifierItem::Active);

     connect(m_myDock, SIGNAL(endApplication()), this, SLOT(slotExitApplication()));
     connect(m_myDock, SIGNAL(activeMainWindow()), this, SLOT(slotActiveMainWindow()));
     connect(m_myDock, SIGNAL(minimize()), this, SLOT(slotCloseMainWindow()));
     connect(m_myDock, SIGNAL(UPSChanged(QString)), this, SLOT(slotSetNewUPS (QString)));

     connect(m_myDock, SIGNAL(preferences()),this, SLOT(slotPreferenceUps()));
     connect(m_myDock, SIGNAL(printUPSVars()),this, SLOT(slotPrintUPSVars()));
     connect(m_myDock, SIGNAL(instComms()),this, SLOT(slotInstComms()));
     connect(m_myDock, SIGNAL(RWVars()),this, SLOT(slotRWVars()));
     connect(m_myDock, SIGNAL(makeRefreshConnection()),this, SLOT(slotRefreshConnection()));
     }

  addEventString(i18n("Ready."));


  // nastavime data pro vybranou polozku
  // sets data for selected item
  setActiveUpsInfo(m_mainUpsInfo.record.name);

  // sets mainUpsInfo.oldState to const value KNutNet::initStatus
  m_mainUpsInfo.oldState=KNutNet::initState;

 //makes connect to UPS, connection is non blocking operation
  //we have to use signal firstConnection and connectionError from KNutNet class
  //
  m_mainUpsInfo.upsNet = new KNutNet( m_mainUpsInfo.record.upsAddress, m_mainUpsInfo.record.upsName, m_mainUpsInfo.record.port,5,200);

  connect(m_mainUpsInfo.upsNet, SIGNAL(firstConnection(void)),this, SLOT(slotFirstConnection(void)));
  connect(m_mainUpsInfo.upsNet, SIGNAL(connectionError(int)),this, SLOT(slotErrorConnection(int)));
  connect(m_mainUpsInfo.upsNet, SIGNAL(connectionClosedByServer(void)),this, SLOT(slotConnectionClosedByServer(void)));
  connect(m_mainUpsInfo.upsNet, SIGNAL(tryFirstConnection(int)),this, SLOT(slotTryFirstConnection(int)));
  connect(m_mainUpsInfo.upsNet, SIGNAL(tryRepeatFirstConnection(int)),this, SLOT(slotTryReepatFirstConnection(int)));


 // repaints the dock
  if (m_myDock) m_myDock->clearDockValues();

  if (settingData.useMainWindow == knc::mw_yes || noDock ||
    (settingData.useMainWindow == knc::mw_auto && settingData.activatedMainWindow )) makeMainWindow ();

  //tries connecting to server
  m_mainUpsInfo.upsDriverState=knc::connecting;
  m_mainUpsInfo.upsNet->open();
}

KNutClient::~KNutClient() {

  // melo by uz byt zruseno - jistota
  if (m_myDock) { delete m_myDock; m_myDock=NULL;}

  if (m_mainUpsInfo.upsNet) {
  // melo by uz byt zruseno - jistota
    delete m_mainUpsInfo.upsNet;
    m_mainUpsInfo.upsNet = 0;
    }
  m_listKNutEvent->clear();
  delete m_listKNutEvent;

  // melo by uz byt zruseno - jistota
  if (mainWindow) delete mainWindow;

  delete upsRecords;
  delete analogRecords;
}

void KNutClient::slotErrorConnection (int error) {
  qDebug ("KNutClient::slotErrorConnection -------------- %i",error);

  if (mainWindow) mainWindow->hideConnectPB();

  m_mainUpsInfo.netError = error;
  m_mainUpsInfo.errorUpsData=true;
  if (m_mainUpsInfo.netError == KNutNet::DriverNotConnected)
    m_mainUpsInfo.upsDriverState=knc::notConnected;
  else
    m_mainUpsInfo.upsDriverState=knc::error;
  if (m_myDock) m_myDock->repaintDock(true);

  if (m_mainUpsInfo.reconnecting) {
    m_mainUpsInfo.reconnectCount++;
    if (m_mainUpsInfo.reconnectCount > countNextConnDelay) m_mainUpsInfo.reconnectCount = countNextConnDelay;

    addEventString(i18n("The connection failed. The next connection will make after %1 sec.").arg(nextConnDelay[m_mainUpsInfo.reconnectCount-1]),m_mainUpsInfo.record.name);
    m_reconnectTimer->setSingleShot( true );
    m_reconnectTimer->start( nextConnDelay[m_mainUpsInfo.reconnectCount-1]*1000); // starts reconnect timer again
    }
  else {
    slotSendMessage (KNutVarData::errorToText(m_mainUpsInfo.netError),"errorConn_event");
    }
  }



//-------------------- Procedures ----------------//

void KNutClient::initVars (void) {
// Nasledujici data jsou nastavena pri natazeni konfigurace
//Next data is setted when konfiguration is loaded
  settingData.panelFlags = KNutFrontPanel::DefaultPanelFlags; // informace v hlavnim panelu // information in main panel
  settingData.toolTipDockFlags = KNutDock::DefaultToolTipFlags; // informace v hlavnim panelu // information in main panel

  m_mainUpsInfo.record.name="";
  m_mainUpsInfo.upsNet=0;
  m_mainUpsInfo.netError=0; // no error zadna chyba
  m_mainUpsInfo.name="";
  m_mainUpsInfo.password="";
  m_mainUpsInfo.errorUpsData=false;
  m_mainUpsInfo.nutVariables=KNutNet::versionVarUnknown;
  m_mainUpsInfo.runtimeVar="";
  m_mainUpsInfo.lowxferVar="";
  m_mainUpsInfo.highxferVar="";
  m_mainUpsInfo.statusVar="";
  m_mainUpsInfo.upsLoadVar="";
  m_mainUpsInfo.upsDriverState=knc::notConnected;
  m_mainUpsInfo.reconnectCount = 1;
  m_mainUpsInfo.reconnecting = false;

  m_panelsAreZeroing=false;
  m_lastError=0; // no error


  mainWindow=0;
  m_myDock=0;
  settingData.activatedMainWindow=false;

//  settingData.x=0;
//  settingData.y=0;
//  settingData.width=300;
//  settingData.height=200;
  }



void KNutClient::loadConfiguration ( void ) {

  qDebug ("KNutClient::loadConfiguration ( void )");
  QString groupName;
  KNutUpsData::upsRecordDef upsData; // structure includes informations about UPS // struktura obsahuje udaje o UPS
  QFont appFont = qApp->font(); // systemovy font pri startu

//  KConfigGroup config = KGlobal::config()->group("");
  KConfigGroup config( KSharedConfig::openConfig(), "" );

  settingData.areYouSure=config.readEntry("AreYouSure",false);
  settingData.useMainWindow=(knc::mainWindowStartVariant)config.readEntry("UseMainWindow",int(knc::mw_auto));
  settingData.useMessageWindow=config.readEntry("UseMessageWindow",false);
  settingData.activatedMainWindow=config.readEntry("PreviousMainWindow",false);
  settingData.useDescription=config.readEntry("UseDescription",false);
  settingData.countCols=config.readEntry("NumberOfCols",int(knc::DefaultCountCols));
  settingData.panelFlags=config.readEntry("PanelFlags",(unsigned int)(KNutFrontPanel::DefaultPanelFlags));
  settingData.inputFrequency=config.readEntry("InputFrequency",int(knc::DefaultInputFrequency));
  settingData.inputVoltage=config.readEntry("InputVoltage",int(knc::DefaultInputVoltage));
  settingData.lowHighXfer=config.readEntry("LowHighXfer",false);
  settingData.customFont=config.readEntry("CustomFont",false);
  settingData.aPanelFont=config.readEntry("AnalogPanelFont",QFont(appFont));
  settingData.mPanelFont=config.readEntry("MainPanelFont",QFont(appFont));
  int upsCount=config.readEntry("NumberOfUpsRecords",int(0));
  settingData.customBColor=config.readEntry("CustomBColor",false);
  settingData.mainBackgroundColor=config.readEntry ("MainBackGroundColor",QColor( knc::defaultBGColor));
  settingData.customBPanelColor=config.readEntry("CustomBPanelColor",false);
  settingData.customBAnalogColor=config.readEntry("CustomBAnalogColor",false);
  settingData.customOAnalogColor=config.readEntry("CustomOAnalogColor",false);
  settingData.mPanelBackgroundColor=config.readEntry ("MPanelBackGroundColor",QColor(knc::defaultBGColor));
  settingData.aPanelBackgroundColor=config.readEntry ("APanelBackGroundColor",QColor(knc::defaultBGColor));
  settingData.analogFingerColor=config.readEntry ("AnalogFingerColor",QColor(knc::defaultFingerColor));
  settingData.analogOKColor=config.readEntry ("AnalogOKColor",QColor(knc::defaultOkColor));
  settingData.analogWarnningColor=config.readEntry ("AnalogWarnningColor",QColor(knc::defaultWarnningColor));
  settingData.analogErrorColor=config.readEntry ("AnalogErrorColor",QColor(knc::defaultErrorColor));
  settingData.analogScaleColor=config.readEntry ("AnalogScaleColor",QColor(knc::defaultScaleColor));
  settingData.analogFontColor=config.readEntry ("AnalogFontColor",QColor(knc::defaultFontColor));
  settingData.toolTipDockFlags=config.readEntry("ToolTipFlags",(unsigned int)(KNutDock::DefaultToolTipFlags));
  settingData.customKIconColor=config.readEntry("CustomKIconColor",false);
  settingData.kIconBackgroundColor=config.readEntry ("KIconBackGroundColor",QColor(knc::defaultKIconColor));
  settingData.typeOfKIcon=config.readEntry("TypeOfKIcon", int(KNutDock::pictureKI));
  settingData.typeDigitalProcessing=config.readEntry("DigitalProcessingOfPointers", int(2));

  m_mainUpsInfo.record.name=config.readEntry("ActiveUps","");


  //config = KGlobal::config()->group("MainWindow");
  //KConfigGroup configMainWindoes ( KSharedConfig::openConfig(), "MainWindow" );
  config =  KConfigGroup ( KSharedConfig::openConfig(), "MainWindow" );

  for (int i = 0; i < upsCount; i++) {

    groupName.sprintf ("UPS %d",i);
//    config = KGlobal::config()->group(groupName);
    config =  KConfigGroup ( KSharedConfig::openConfig(), groupName );

  if (!((upsData.name = config.readEntry ("Name","")).isEmpty())) {
      // record exist, we will go to next part

      //if ActiveUps is empty, we use name of first ups from ups list
      if ((m_mainUpsInfo.record.name.isEmpty()) && (!(upsData.name.isEmpty())))  m_mainUpsInfo.record.name = upsData.name;



      upsData.upsName = config.readEntry ("UpsName","");
      upsData.upsAddress = config.readEntry ("UpsAddress","");

      if ((!( upsData.upsAddress.isEmpty()))
        || (( upsData.upsAddress.isEmpty()) && (!(upsData.upsName.isEmpty())))) {

        // only for backwards compatibility with older release then 0.7
        if (upsData.upsAddress.isEmpty()) {
          int charPos;
          if ((charPos=upsData.upsName.indexOf('@')) == -1) {
            upsData.upsAddress=upsData.upsName;
            upsData.upsName="";
            }
          else {
            upsData.upsAddress=upsData.upsName.mid(charPos+1);
            upsData.upsName=upsData.upsName.left(charPos);
            }
          }
        // zaznam existuje pokracujeme dale a nacteme zbytek hodnotet
       // record exists, we go next and will read rest of values
        upsData.delay = config.readEntry("Delay",int(knc::DefaultDelay));
        upsData.port = (quint16)config.readEntry("Port",uint(knc::DefaultPort));
        upsData.userName=config.readEntry("UserName","");
        upsData.password=config.readEntry("Password","");
        upsData.savePassword=config.readEntry("SavePassword",false);
        for (int j=0; j < knc::NumberOfVars; j++) {
          groupName.sprintf ("Var %d",j);
          upsData.upsVarCode[j]=config.readEntry (groupName,int(0));
          if ((upsData.upsVarCode[j] < 0) || (upsData.upsVarCode[j] > knc::NumberOfTypes))
             upsData.upsVarCode[j] = 0;
          }
        upsRecords->add (upsData);
        }
      }
    }

  }


void KNutClient::saveAllConfiguration ( void ) {

  saveCommonConfiguration (); 
  savePreferencesConfiguration();
  syncConfiguration();
  }


void KNutClient::syncConfiguration ( void ) {

//  KConfigGroup config = KGlobal::config()->group("");
  KConfigGroup config( KSharedConfig::openConfig(), "" );
  config.sync();
  }

void KNutClient::saveCommonConfiguration ( void ) {


  KConfigGroup config( KSharedConfig::openConfig(), "" );
  config.writeEntry ("PreviousMainWindow",settingData.activatedMainWindow);
  KNutUpsData::upsRecordDef upsData; // struktura obsahuje udaje o UPS // structure includes information about UPS

// active UPS
  //if name of active ups isn't knowly, will use name of the firt ups from ups list
  if ((m_mainUpsInfo.record.name.isEmpty()) &&  (upsRecords->getCount() > 0)) {
	      upsRecords->get(0,upsData);
      config.writeEntry ("ActiveUps",upsData.name);
    }
  else config.writeEntry ("ActiveUps",m_mainUpsInfo.record.name); //
}



void KNutClient::savePreferencesConfiguration ( void ) {
  QString groupName;
  QString codeName;
  KNutUpsData::upsRecordDef upsData; // struktura obsahuje udaje o UPS // structure includes information about UPS
  int upsCount = upsRecords->getCount();

//  KConfigGroup config = KGlobal::config()->group("");
  KConfigGroup config( KSharedConfig::openConfig(), "" );

  config.writeEntry ("DigitalProcessingOfPointers",int(settingData.typeDigitalProcessing));
  config.writeEntry ("TypeOfKIcon",int(settingData.typeOfKIcon));
  config.writeEntry ("KIconBackGroundColor",QColor(settingData.kIconBackgroundColor));
  config.writeEntry ("CustomKIconColor",settingData.customKIconColor);
  config.writeEntry ("ToolTipFlags",(unsigned int)(settingData.toolTipDockFlags));
  config.writeEntry ("AnalogFontColor",QColor(settingData.analogFontColor));
  config.writeEntry ("AnalogScaleColor",QColor(settingData.analogScaleColor));
  config.writeEntry ("AnalogErrorColor",QColor(settingData.analogErrorColor));
  config.writeEntry ("AnalogWarnningColor",QColor(settingData.analogWarnningColor));
  config.writeEntry ("AnalogOKColor",QColor(settingData.analogOKColor));
  config.writeEntry ("AnalogFingerColor",QColor(settingData.analogFingerColor));
  config.writeEntry ("APanelBackGroundColor",QColor(settingData.aPanelBackgroundColor));
  config.writeEntry ("MPanelBackGroundColor",QColor(settingData.mPanelBackgroundColor));
  config.writeEntry ("MainBackGroundColor",QColor(settingData.mainBackgroundColor));
  config.writeEntry ("NumberOfUpsRecords",upsCount);
  config.writeEntry ("CustomFont",settingData.customFont);             //bool
  config.writeEntry ("AnalogPanelFont",QFont(settingData.aPanelFont)); //QFont
  config.writeEntry ("MainPanelFont",QFont(settingData.mPanelFont));   //QFont
  config.writeEntry ("CustomBColor",settingData.customBColor);
  config.writeEntry ("CustomBPanelColor",settingData.customBPanelColor);
  config.writeEntry ("CustomBAnalogColor",settingData.customBAnalogColor);
  config.writeEntry ("CustomOAnalogColor",settingData.customOAnalogColor);
  config.writeEntry ("LowHighXfer",settingData.lowHighXfer);
  config.writeEntry ("InputVoltage",settingData.inputVoltage);
  config.writeEntry ("InputFrequency",settingData.inputFrequency);
  config.writeEntry ("PanelFlags",uint(settingData.panelFlags | 3));
  config.writeEntry ("NumberOfCols",settingData.countCols);
  config.writeEntry ("UseDescription",settingData.useDescription);
  config.writeEntry ("AreYouSure",settingData.areYouSure);
  config.writeEntry ("UseMainWindow",int(settingData.useMainWindow));  // cast enum type to int type
  config.writeEntry ("UseMessageWindow",settingData.useMessageWindow);

  for (int i=0; i<upsCount; i++) {
    groupName.sprintf ("UPS %d",i);
    config =  KConfigGroup ( KSharedConfig::openConfig(), groupName );

    upsRecords->get(i,upsData);
    config.writeEntry ("Port",uint(upsData.port));
    config.writeEntry ("Delay",upsData.delay);
    config.writeEntry ("UpsAddress",upsData.upsAddress);
    config.writeEntry ("UpsName",upsData.upsName);
    config.writeEntry ("Name",upsData.name);
    if (upsData.savePassword) {
      config.writeEntry ("UserName",upsData.userName);
      config.writeEntry ("Password",upsData.password);
      }
    config.writeEntry ("SavePassword",upsData.savePassword);
    for (int j=0; j < knc::NumberOfVars; j++) {
      groupName.sprintf ("Var %d",j);
      config.writeEntry (groupName,upsData.upsVarCode[j]);
      }
    config =  KConfigGroup ( KSharedConfig::openConfig(), "" );
    }
  }


bool KNutClient::commitData (QSessionManager&) {

  saveCommonConfiguration ();
  syncConfiguration();
  return true;
  }



void KNutClient::upsDeactivate (void) {

  qDebug("KNutClient::upsDeactivate");

  // this function isn't interuptable function
  QMutexLocker upsDeactivateLocker( &m_upsDeactivateMutex );
  m_upsTimer->stop();
  m_mainUpsInfo.upsNet->close(); // close connection
  // state of connection to usbd, this isn't state of knutnet
  m_mainUpsInfo.upsDriverState=knc::notConnected;
  m_mainUpsInfo.netError=0; // no error
  m_mainUpsInfo.nutVariables=KNutNet::versionVarUnknown;
  }

void KNutClient::setActiveUpsVars ( bool setVarsForMainWindowAlways ) {
  qDebug("KNutClient::setActiveUpsVars");
  // sets when same variables are existed
  if ( m_mainUpsInfo.upsNet->readNumberVars() > 0) {
    // deaktivujeme veskere promene
    // unactivates all variables
    m_mainUpsInfo.upsNet->setUnactiveAllUpsVars();

    if (( mainWindow) || (setVarsForMainWindowAlways) ) {
      for (int j=0; j < knc::NumberOfVars; j++) {
        if (m_mainUpsInfo.record.upsVarCode[j] != 0 ) {
          m_mainUpsInfo.upsNet->setActiveUpsVar(KNutVarData::nameUpsVar(m_mainUpsInfo.record.upsVarCode[j],m_mainUpsInfo.nutVariables));
          }
        }
      }

    if (m_myDock) m_mainUpsInfo.upsNet->setActiveUpsVar(m_mainUpsInfo.upsLoadVar);

    //runtime je vzdy pridano v merenym polozkam
    m_mainUpsInfo.upsNet->setActiveUpsVar(m_mainUpsInfo.runtimeVar);
    // status se nastavi vzdy
    m_mainUpsInfo.upsNet->setActiveUpsVar(m_mainUpsInfo.statusVar);
    // xfer se nastavi vzdy pokud existuje
    // potrebujeme ho znat jeste pred startem a pak behen behu programu
    setActiveXFerVars(true);
    }
  }


bool KNutClient::saveState (QSessionManager&) {

  saveCommonConfiguration ();
  syncConfiguration();
  return true;
  }


void KNutClient::getUpsData (void) {
  int runNetError;

  qDebug("KNutClient::getUpsData");
  runNetError=m_mainUpsInfo.upsNet->getUpsValues(false);

  if (!(runNetError) && (!(m_mainUpsInfo.errorUpsData))) {
    m_mainUpsInfo.netError=runNetError;
    qDebug("Data OK -> Data OK");
    return;
    }

  if (!(runNetError) && (m_mainUpsInfo.errorUpsData)) {
    qDebug("Data Error -> Data OK");

    m_mainUpsInfo.netError=runNetError;
    m_mainUpsInfo.errorUpsData=false;
    slotSendMessage (i18n("Data OK"),"ok_event");
    return;
    }

  if ((runNetError) && (!m_mainUpsInfo.errorUpsData)) {
    qDebug("Data OK -> Data Error");

    m_mainUpsInfo.netError=runNetError;
    slotSendMessage (KNutVarData::errorToText(runNetError),"error_event");
    m_lastError=runNetError;
    m_mainUpsInfo.errorUpsData=true;
    return;
    }

  if ((runNetError) && (m_mainUpsInfo.errorUpsData)) {
    qDebug("Data Error -> Data Error");

    m_mainUpsInfo.netError=runNetError;
    if (m_lastError != runNetError) { // change status from error to other error
       slotSendMessage (KNutVarData::errorToText(runNetError),"error_event");
      m_lastError=runNetError;
      }
    return;
    }
  }


void KNutClient::setActiveXFerVars (bool activate) {
  if (activate) {
    // podkud neexistuje jedna z promenych neaktivujeme ani jednu
    if (!(m_mainUpsInfo.upsNet->setActiveUpsVar(m_mainUpsInfo.lowxferVar)))
      if (m_mainUpsInfo.upsNet->setActiveUpsVar(m_mainUpsInfo.highxferVar))
        m_mainUpsInfo.upsNet->setActiveUpsVar(m_mainUpsInfo.lowxferVar,false);
    }
  else {
    m_mainUpsInfo.upsNet->setActiveUpsVar(m_mainUpsInfo.lowxferVar,false);
    m_mainUpsInfo.upsNet->setActiveUpsVar(m_mainUpsInfo.highxferVar,false);
    }
  }


void KNutClient::setActiveUpsInfo (const QString &name) {
  qDebug ("KnutClient : setActiveUpsInfo"); 
  //presuneme jmena  z databaze ups-ek - je nahrana z config. souboru
  KNutUpsData::upsRecordDef* upsRecordPointer;

  if ((!name.isNull()) && (!name.isEmpty()) && ((upsRecordPointer = upsRecords->findName(name)) != NULL)) {
    m_mainUpsInfo.record.upsName = upsRecordPointer->upsName;
    m_mainUpsInfo.record.upsAddress = upsRecordPointer->upsAddress;
    m_mainUpsInfo.record.delay = upsRecordPointer->delay;
    m_mainUpsInfo.record.port = upsRecordPointer->port;
    m_mainUpsInfo.record.userName = upsRecordPointer->userName;
    m_mainUpsInfo.record.password = upsRecordPointer->password;
    for (int i =0; i < knc::NumberOfVars; i++) m_mainUpsInfo.record.upsVarCode[i] = 0; // vynulujeme polozky
    int ii = 0;
    for (int i =0; i < knc::NumberOfVars; i++)
      if (upsRecordPointer->upsVarCode[i]>0) m_mainUpsInfo.record.upsVarCode[ii++] = upsRecordPointer->upsVarCode[i];
    }
  }


void KNutClient::addEventString (const QString text, const QString UPSAddress) {
  qDebug ("KNutClient::addEventString");

  QString eventText(QDate::currentDate().toString());
  eventText +=" " + QTime::currentTime().toString();

  if (UPSAddress.isEmpty()){
     eventText +=" "+text;
     }
  else {
     eventText +=" "+UPSAddress+" : "+text;
     }

  if (mainWindow) {
    mainWindow->addComboStatusItem (eventText);
    }

  m_listKNutEvent->append(eventText);

  if (m_listKNutEvent->count() > knc::maxCountKNutEvent) {
    m_listKNutEvent->takeFirst(); //uses takeFirst() because one is fastly next remove(..begin)
    }
  }


void KNutClient::showStatusEvent(void) {


  qDebug ("KNutClient::showStatusEvent");
  int myState = m_mainUpsInfo.upsNet->readStatus();
  if (myState != m_mainUpsInfo.oldState) {

    if ((myState & KNutNet::OFF) != (m_mainUpsInfo.oldState & KNutNet::OFF)) {
      if (myState & KNutNet::OFF) {
        slotSendMessage (i18n("UPS is off."),"upsOff_event");
        }
      else {
        slotSendMessage (i18n("UPS is back on."),"upsBack_event");
        }
      }
    if ((myState & KNutNet::OL) != (m_mainUpsInfo.oldState & KNutNet::OL)){
       if (myState & KNutNet::OL)  {
         slotSendMessage (i18n("Power is back online."),"powerBack_event");
         }
       }
    if ((myState & KNutNet::OB) != (m_mainUpsInfo.oldState & KNutNet::OB)) {
      if (myState & KNutNet::OB)  {
        slotSendMessage (i18n("UPS is on battery."),"upsOnBatt_event");
        }
      }
    if ((myState & KNutNet::LB) != (m_mainUpsInfo.oldState & KNutNet::LB)) {
      if (myState & KNutNet::LB) {
        slotSendMessage (i18n("UPS battery is low."),"battLow_event");
        }
      else {
        slotSendMessage (i18n("UPS battery is OK."),"battOK_event");
        }
      }
    if ((myState & KNutNet::RB) != (m_mainUpsInfo.oldState & KNutNet::RB)) {
      if (myState & KNutNet::RB) {
        slotSendMessage (i18n("UPS battery is bad and needs be replaced."),"battRep_event");
        }
      }
    if ((myState & KNutNet::OVER) != (m_mainUpsInfo.oldState & KNutNet::OVER)) {
      if (myState & KNutNet::OVER) {
        slotSendMessage (i18n("UPS is overloaded."),"overOn_event");
        }
      else {
        slotSendMessage (i18n("UPS isn't overloaded."),"overOff_event");
        }
      }
    if ((myState & KNutNet::CAL) != (m_mainUpsInfo.oldState & KNutNet::CAL)) {
      if (myState & KNutNet::CAL) {
        slotSendMessage (i18n("UPS is performing calibration."),"calOn_event");
        }
      else {
        slotSendMessage(i18n("Calibration of UPS is ended."),"calOff_event");
        }
      }
    if ((myState & KNutNet::BOOST) != (m_mainUpsInfo.oldState & KNutNet::BOOST)) {
      if (myState & KNutNet::BOOST) {
        slotSendMessage(i18n("UPS is boosting incoming voltage."),"boostOn_event");
        }
      else {
        slotSendMessage(i18n("Boosting of UPS is ended."),"boostOff_event");
        }
      }
    if ((myState & KNutNet::TRIM) != (m_mainUpsInfo.oldState & KNutNet::TRIM)) {
      if (myState & KNutNet::TRIM) {
        slotSendMessage(i18n("UPS is trimming incoming voltage."),"trimOff_event");
        }
      else {
        slotSendMessage (i18n("Trimming of UPS is ended."),"trimOff_event");
        }
      }
    m_mainUpsInfo.oldState=myState;
    }
  }


void KNutClient::makeMainWindow (void) {
  qDebug ("KNutClient::makeMainWindow");

  mainWindow = new KNutMainWindow(m_listKNutEvent ,&m_mainUpsInfo, &settingData, upsRecords, analogRecords);
  connect(mainWindow, SIGNAL(UPSChanged (QString)), this, SLOT(slotSetNewUPS (QString)));
  connect(mainWindow, SIGNAL(preferenceUps()),this, SLOT(slotPreferenceUps()));
  connect(mainWindow, SIGNAL(showUPSVars()),this, SLOT(slotPrintUPSVars()));
  connect(mainWindow, SIGNAL(makeInstComms()),this, SLOT(slotInstComms()));
  connect(mainWindow, SIGNAL(makeRWVars()),this, SLOT(slotRWVars()));
  connect(mainWindow, SIGNAL(makeRefreshConnection()),this, SLOT(slotRefreshConnection()));
  connect(mainWindow, SIGNAL(saveConf()),this, SLOT(slotSaveConf()));
  connect(mainWindow, SIGNAL(endApplication()), this, SLOT(slotExitApplication()));
  connect(mainWindow, SIGNAL(destroyed()), this, SLOT(slotMainWindowDestroyed()));
  }




/*********************************************************************************************/
/*                                                                                           */
/*                             PRIVATE SLOTS                                                 */
/*                                                                                           */
/*********************************************************************************************/


void KNutClient::slotTryFirstConnection (int maxCount) {
qDebug ("KNutClient::slotTryFirstConnection");

  //KNutNet begin the first test of connection (connectToHost will be made after emiting signal tryFirstConnection )
  // repaints icon
  if (m_myDock) m_myDock->repaintDock(true);
  if (mainWindow) mainWindow->startConnectPB(maxCount);
  }

void KNutClient::slotTryReepatFirstConnection (int number ) {

qDebug ("KNutClient::slotTryReepatFirstConnection");
//KNutNet begin nexts test /the second ..../ of connection

  if (mainWindow) mainWindow->setProgressConnectPB (number);
  }



void KNutClient::slotFirstConnection (void) {
qDebug ("KNutClient::slotFirstConnection");
//TCP - UDP connection to server was made

  bool reconnecting = m_mainUpsInfo.reconnecting;

  m_mainUpsInfo.reconnecting =false;
  m_reconnectTimer->stop();

  m_mainUpsInfo.nutVariables = m_mainUpsInfo.upsNet->getNutVariables();
  m_mainUpsInfo.nutProtocol = m_mainUpsInfo.upsNet->getNutProtocol();

  if (mainWindow) mainWindow->hideConnectPB();


  // reads UPS's variables

    int countRevision = 2; // pocet max opakovani po kterych muzeme zjistovat hodnoty
    do {
      if (!(m_mainUpsInfo.netError = m_mainUpsInfo.upsNet->getUpsVars())) break;
      if (!(m_mainUpsInfo.netError)) break;  // no error
      if (m_mainUpsInfo.netError == KNutNet::UnknownUps) break;
      if (m_mainUpsInfo.netError == KNutNet::DriverNotConnected) break;
      countRevision--;
      if (countRevision) sleep (2);
      }
    while (countRevision);
  if (m_mainUpsInfo.netError) {
    m_mainUpsInfo.errorUpsData=true;
    // mainUpsInfo.upsDriverState is state of connection to usbd, this isn't state of knutnet
    if (m_mainUpsInfo.netError == KNutNet::DriverNotConnected)
      m_mainUpsInfo.upsDriverState=knc::notConnected;
    else
      m_mainUpsInfo.upsDriverState=knc::error;

    if (m_myDock) m_myDock->repaintDock(true);
    slotSendMessage(KNutVarData::errorToText(m_mainUpsInfo.netError),"errorConn_event");
    }
  else {
    if (reconnecting) {
      slotSendMessage(i18n("The connection was renewed."),"connRenewed_event");
    }
  m_mainUpsInfo.upsDriverState=knc::connected;
    m_mainUpsInfo.errorUpsData=false;

    switch (m_mainUpsInfo.nutVariables) { // type of variables, old or new (with dot)
        case KNutNet::versionVarOne :
          if (!(m_mainUpsInfo.upsNet->existName("BATT_RUNTIME"))) m_mainUpsInfo.runtimeVar="BATT_RUNTIME";
          else m_mainUpsInfo.runtimeVar="RUNTIME";
          m_mainUpsInfo.upsLoadVar="LOADPCT";
          m_mainUpsInfo.lowxferVar="LOWXFER";
          m_mainUpsInfo.highxferVar="HIGHXFER";
          m_mainUpsInfo.statusVar="STATUS";
        break;
        case KNutNet::versionVarTwo:
          m_mainUpsInfo.runtimeVar="battery.runtime";
          m_mainUpsInfo.upsLoadVar="ups.load";
          m_mainUpsInfo.lowxferVar="input.transfer.low";
          m_mainUpsInfo.highxferVar="input.transfer.high";
          m_mainUpsInfo.statusVar="ups.status";
        break;
        case KNutNet::versionVarUnknown:
          m_mainUpsInfo.runtimeVar="";
          m_mainUpsInfo.upsLoadVar="";
          m_mainUpsInfo.lowxferVar="";
          m_mainUpsInfo.highxferVar="";
          m_mainUpsInfo.statusVar="";
        break;
       }

    if ((settingData.useDescription) && (!m_mainUpsInfo.upsNet->isDescription())) m_mainUpsInfo.upsNet->getDescription();
      // test presunut do setActiveUpsVars
    setActiveUpsVars();  //sets all variables like active variables // ones will be read

    // repaints main window and dock immediately
    if (mainWindow) mainWindow->showUpsData(); //sets value of front panel
    if (m_myDock) m_myDock->repaintDock(true); // dock is repainting always

    m_panelsAreZeroing=false; // analog panels isn't setting to zero
    }
  if ((m_mainUpsInfo.upsNet->getState() == KNutNet::Connected) && (!m_mainUpsInfo.errorUpsData)) {
    showStatusEvent();
    // getUpsVars loads informations about variables, but their values too
    //starts timer immediately we needs new data
    m_upsTimer->setSingleShot( true );
    m_upsTimer->start( 1); // nastartujeme casovac - starts timer
    }
  }

void KNutClient::slotConnectionClosedByServer(void) {
qDebug("KNutClient::slotConnectionClosedByServer ");

  m_upsTimer->stop();

  // state of connection to usbd, this isn't state of knutnet
  m_mainUpsInfo.upsDriverState=knc::notConnected;
  m_mainUpsInfo.nutVariables=KNutNet::versionVarUnknown;
  m_mainUpsInfo.netError = KNutNet::ConnetionClosedByServer;

  if (mainWindow) {
    mainWindow->hideConnectPB();
    mainWindow->cleanPanels();
    }
  if (m_myDock) {
    // clears values of dock and set standard icon of dock
   m_myDock->clearDockValues();
   }
  slotSendMessage(i18n("The connection was closed by the second side (upsd)."),"connClosedSecondSide_event");
  m_mainUpsInfo.reconnecting = true;
  m_mainUpsInfo.reconnectCount = 1;
  m_reconnectTimer->setSingleShot( true );
  m_reconnectTimer->start( nextConnDelay[0]*1000 ); // start timer for reconnect delay
  }


void KNutClient::slotTimeout (void) {

qDebug("KNutClient::slotTimeout");
  getUpsData();
  if (!m_mainUpsInfo.errorUpsData) { // no error
    if (mainWindow) {
      mainWindow->repaintPanel();
      mainWindow->repaintAnalogs();
      }
    m_panelsAreZeroing=false;
    showStatusEvent();
    if (m_myDock) m_myDock->repaintDock();
    }
  else { // error
    if ((mainWindow) && (!m_panelsAreZeroing)) mainWindow->zeroingPanels();
    m_panelsAreZeroing=true;
    if (m_myDock) m_myDock->clearDockValues();
    if (m_myDock) m_myDock->repaintDock(true);
    }
//  if (m_myDock) m_myDock->repaintDock();
  m_upsTimer->setSingleShot( true );
  m_upsTimer->start( m_mainUpsInfo.record.delay ); // nastartujem znovu
  }



void KNutClient::slotReconnectTimeout (void) {
qDebug("KNutClient::slotReconnectTimeout");

  m_mainUpsInfo.netError=0; //clears netError
  m_mainUpsInfo.upsDriverState=knc::connecting;
  m_mainUpsInfo.upsNet->open();
  }


void KNutClient::slotPreferenceUps (void) {

qDebug("KNutClient::slotPreferenceUps");

  KNutPrefDlg* prefDlg = new KNutPrefDlg ( upsRecords, analogRecords, &settingData, m_mainUpsInfo.record.name ); // aktivujeme dialog pro preference
  connect (prefDlg,SIGNAL(signalChangeConfig(unsigned int)),this,SLOT(slotSetConfig(unsigned int)));
  prefDlg->exec();
  delete prefDlg;

  }



void KNutClient::slotSetConfig (unsigned int changeSetting) {

  if (changeSetting) {
    // cely blok konfiguraci ukladame jen zde // all block of configurations is saved only here
    saveAllConfiguration();
    if (changeSetting & KNutPrefDlg::ChangeXFer) {
      if (settingData.lowHighXfer)
        setActiveXFerVars(false);
      else
        setActiveXFerVars(true);
      }

    if (changeSetting & KNutPrefDlg::ChangeUpsSetting) {
    //we have changed active UPS
//********** same code like code into slotSetNewUPS -- stejny kod jako v slotSetNewUPS
      upsDeactivate();

      if (m_mainUpsInfo.record.name == "") {
        // the first record was added, because the first UPS is active UPS always
        QString name = upsRecords->getName(0);
        addEventString(i18n("Switched"),name);
        m_mainUpsInfo.record.name=name;
        m_mainUpsInfo.errorUpsData=false;
        saveCommonConfiguration (); // saves information of new/first active UPS
        }
      else {
        if (upsRecords->getCount() == 0) {
        // the last record was deleted, because the last UPS is active UPS
          addEventString(i18n("Switched"),"");
          m_mainUpsInfo.record.name="";
          m_mainUpsInfo.errorUpsData=false;
          m_mainUpsInfo.record.upsName = "";
          m_mainUpsInfo.record.upsAddress = "";
          saveCommonConfiguration (); // saves information of no UPS, list of UPS is empty
          }
        }
      setActiveUpsInfo(m_mainUpsInfo.record.name);
      // zde je nutno hned provest prekresleni
      // showUpsData je nutno udelat take proto ze se mohl zmenit vyber meraku
//      if (mainWindow) mainWindow->showUpsData();
      if (mainWindow) mainWindow->cleanPanels();
//      if (myDock) myDock->repaintDock(mainUpsInfo.netError);
      if (m_myDock) m_myDock->clearDockValues();


      //newUPS close connection  and sets paremetrs of new UPS
      m_mainUpsInfo.upsNet->newUPS(m_mainUpsInfo.record.upsAddress,m_mainUpsInfo.record.upsName, m_mainUpsInfo.record.port);
     // state of connection to usbd, this isn't state of knutnet
      m_mainUpsInfo.upsDriverState=knc::connecting;
      m_mainUpsInfo.upsNet->open();  // opens connections

      }
    if (changeSetting & KNutPrefDlg::ChangeUpsCount) {
      if (m_myDock) m_myDock->changeUps();
      }

    if (changeSetting & KNutPrefDlg::ChangeTypeOfKIcon) {
      if (m_myDock) m_myDock->changeKIcon();
      }

    if (changeSetting & KNutPrefDlg::ChangeCustomKIconColor) {
      if (m_myDock) m_myDock->changeKIBColor();
      }

    if (changeSetting & KNutPrefDlg::ChangeKIconBColor) {
      if (m_myDock) m_myDock->changeKIBColor();
      }

    if (changeSetting & KNutPrefDlg::ChangeToolTip) {
      if (m_myDock) m_myDock->changeToolTip();
      }

    //for other settings will call functions from mainWindow class
   if (mainWindow) mainWindow->setConfig (changeSetting);
    }
  }



void KNutClient::slotPrintUPSVars(void) {

if (m_mainUpsInfo.upsDriverState == knc::connected) {
  if (!m_mainUpsInfo.errorUpsData) {
    if ((m_mainUpsInfo.upsNet->readNumberVars (KNutNet::AllVars)) || (m_mainUpsInfo.upsNet->readNumberComms ())) {
      KNutPrintUpsVar *windowUpsVars = new KNutPrintUpsVar(m_mainUpsInfo.upsNet,settingData.useDescription,(m_myDock) && (settingData.useMessageWindow));
      windowUpsVars->exec();
      delete windowUpsVars;
      }
    }
   else {
     slotSendMessage(KNutVarData::errorToText(m_mainUpsInfo.netError),"errorConn_event");
     }
   }
 else {
   slotSendMessage(KNutVarData::errorToText(KNutNet::NotConnection),"notConnection_event");
   }
}




void KNutClient::slotInstComms(void) {

  if (m_mainUpsInfo.upsDriverState == knc::connected) {
    if (!m_mainUpsInfo.errorUpsData) {
      if (m_mainUpsInfo.upsNet->readNumberComms ()) {
        KNutInstComms *windowInstComms = new KNutInstComms(&m_mainUpsInfo.name,&m_mainUpsInfo.password,m_mainUpsInfo.record.userName,m_mainUpsInfo.record.password,m_mainUpsInfo.upsNet);
        connect(windowInstComms,SIGNAL(signalShowMessage(QString, QString)),this,SLOT(slotSendMessage(QString,QString)));
        if (windowInstComms->isUpsOk()) windowInstComms->exec();
        disconnect(windowInstComms,SIGNAL(signalShowMessage(QString, QString)),this,SLOT(slotSendMessage(QString,QString)));
        delete windowInstComms;
        }
      }
    else {
      slotSendMessage(KNutVarData::errorToText(m_mainUpsInfo.netError),"errorConn_event");
      }
    }
  else {
    slotSendMessage(KNutVarData::errorToText(KNutNet::NotConnection),"notConnection_event");
    }
  }




void KNutClient::slotRWVars(void) {

  qDebug("KNutClient::slotRWVars");
  if (m_mainUpsInfo.upsDriverState == knc::connected) {
    if (!m_mainUpsInfo.errorUpsData) {
      if (m_mainUpsInfo.upsNet->readNumberVars (KNutNet::RWVars)) {
        KNutRWVar *windowRWVars = new KNutRWVar(&m_mainUpsInfo.name,&m_mainUpsInfo.password,m_mainUpsInfo.record.userName,m_mainUpsInfo.record.password,m_mainUpsInfo.upsNet);
        connect(windowRWVars,SIGNAL(signalShowMessage(QString, QString)),this,SLOT(slotSendMessage(QString,QString)));
        if (windowRWVars->isUpsOk())  windowRWVars->exec();
        disconnect(windowRWVars,SIGNAL(signalShowMessage(QString, QString)),this,SLOT(slotSendMessage(QString,QString)));
        delete windowRWVars;
       }
     }
   else {
     slotSendMessage (KNutVarData::errorToText(m_mainUpsInfo.netError),"errorConn_event");
      }
    }
  else {
   slotSendMessage (KNutVarData::errorToText(KNutNet::NotConnection),"notConnection_event");
    }
  }




void KNutClient::slotSendMessage (const QString &message, const QString &event) {

  if ((m_myDock) && (settingData.useMessageWindow)) {
    KNotification::event (event,message);
    }
  addEventString(message,m_mainUpsInfo.record.name);
  }  




void KNutClient::slotSetNewUPS (const QString name) {
qDebug(" KNutClient::slotSetNewUPS");

//stops reconnecting 
  m_mainUpsInfo.reconnecting =false;
  m_reconnectTimer->stop();

  if (name  != m_mainUpsInfo.record.name) {
    upsDeactivate();

    addEventString(i18n("Switched"),name);

    m_mainUpsInfo.record.name=name;
    m_mainUpsInfo.errorUpsData=false;
    setActiveUpsInfo(m_mainUpsInfo.record.name);

    //new connection name and password are cleaned
    m_mainUpsInfo.name = "";
    m_mainUpsInfo.password = "";

    if (mainWindow) {
      mainWindow->setCombo(name);
      mainWindow->cleanPanels();
      }

   if (m_myDock) {
      // clears values of dock and set standard icon of dock
     m_myDock->clearDockValues();
     m_myDock->setNewUPS(); // sets miniIcon in list of UPSes for selected UPS
     }

    m_panelsAreZeroing=false;
    m_lastError=0; // zadna chyba
    m_mainUpsInfo.oldState=KNutNet::initState; // init variable of last UPS state

    m_mainUpsInfo.upsNet->newUPS(m_mainUpsInfo.record.upsAddress,m_mainUpsInfo.record.upsName, m_mainUpsInfo.record.port);
     // state of connection to usbd, this isn't state of knutnet
    m_mainUpsInfo.upsDriverState=knc::connecting;

    m_mainUpsInfo.upsNet->open();
    }
  }



void KNutClient::slotRefreshConnection(void) {
qDebug("KNutClient::slotRefreshConnection");

  m_reconnectMutex.lock();

  m_mainUpsInfo.reconnecting =false;
  m_reconnectTimer->stop();
  m_upsTimer->stop();
  upsDeactivate(); // closes connections

    // nastavime na docku iconu
    // sets icon for dock
  if (m_myDock) {
    m_myDock->clearDockValues();
 //     myDock->initImage();
 //     myDock->repaint();
    }
  if (mainWindow) {
    mainWindow->cleanPanels();
    }
  addEventString(i18n("Reconnect"),m_mainUpsInfo.record.name);
//    mainUpsInfo.record.name=name;
  m_mainUpsInfo.errorUpsData=false;
  setActiveUpsInfo(m_mainUpsInfo.record.name);

  //new connection name and password are cleaned
  m_mainUpsInfo.name = "";
  m_mainUpsInfo.password = "";

  m_panelsAreZeroing=false;
  m_lastError=0; // zadna chyba
  m_mainUpsInfo.oldState=KNutNet::initState;

  m_mainUpsInfo.upsNet->newUPS(m_mainUpsInfo.record.upsAddress,m_mainUpsInfo.record.upsName, m_mainUpsInfo.record.port);
  // state of connection to usbd, this isn't state of knutnet
  m_mainUpsInfo.upsDriverState=knc::connecting;
  m_mainUpsInfo.upsNet->open();
  m_reconnectMutex.unlock();
  }





void KNutClient::slotActiveMainWindow (void) {
qDebug("KNutClient::slotActiveMainWindow (void)");

  if (!mainWindow) {
    //  pred activaci nastaveme promenne znovu precteme data
    // state of connection to usbd, this isn't state of knutnet
    if (m_mainUpsInfo.upsDriverState==knc::connected) {
      m_upsTimer->stop();
      setActiveUpsVars(true);  // true activate ups variable for main window too
      QTimer::singleShot(0, this, SLOT(slotTimeout()));
//      getUpsData();
      makeMainWindow();
//      if (m_myDock) m_myDock->repaintDock(m_mainUpsInfo.netError);
//      QTimer::singleShot(0, this, SLOT(slotTimeout()));
      }
    else {
      setActiveUpsVars(true);
      //getUpsData();  zrusene protoze kdyz nebylo spojeni navazeno vratil by prikaz
      // getValues error=0 a doslo by vypsani data ok.
      makeMainWindow();
      if (!m_mainUpsInfo.errorUpsData) {
        if (m_myDock) m_myDock->repaintDock(m_mainUpsInfo.netError);
        }
      }
    }
  else {   // end if mainWindow
    //activates main window
    mainWindow->show();
    mainWindow->raise();
    KWindowSystem::forceActiveWindow( mainWindow->winId());
    }
  }

void KNutClient::slotSaveConf (void) {
  saveAllConfiguration(); // saves all configuration
                          // main window required
}


void KNutClient::slotMainWindowDestroyed (void) {

qDebug("KNutClient::slotMainWindowDestroyed");
  mainWindow=NULL;
  if (m_mainUpsInfo.upsNet) setActiveUpsVars();
  // Pokud neni aktivovan dock musime ukoncit aplikaci..
  // When dock isn't activated we must close application.
  if (m_myDock == 0) {
    saveCommonConfiguration ();
    syncConfiguration();
    if (m_mainUpsInfo.upsNet) {
      delete m_mainUpsInfo.upsNet;
      m_mainUpsInfo.upsNet = NULL;
      }
    qApp->quit();
    }
  }


void KNutClient::slotCloseMainWindow (void) {

qDebug ("KNutClient::slotCloseMainWindow");
  if (mainWindow) {
    if (m_myDock) m_myDock->repaintDock(m_mainUpsInfo.netError);
    m_upsTimer->stop();
    delete mainWindow;
    mainWindow=NULL;
    QTimer::singleShot(0, this, SLOT(slotTimeout()));
    }
  }


void KNutClient::slotExitApplication(void) {
  qDebug () << "slotExitApp";

  if ((!(settingData.areYouSure)) ||
     ((settingData.areYouSure) &&  (KMessageBox::questionYesNo (0,i18n("Are you sure ?"),"") == KMessageBox::Yes))) {

    m_upsTimer->stop();
    if (mainWindow) {
      // odpojime protoze slotMainWindowDestroyed by byl vykonan az po zkonceni procedury;
      disconnect(mainWindow, SIGNAL(destroyed()), this, SLOT(slotMainWindowDestroyed()));
      delete mainWindow;
      mainWindow = NULL;
      }
    saveCommonConfiguration ();
    syncConfiguration();
    if (m_mainUpsInfo.upsNet) {
      delete m_mainUpsInfo.upsNet;
      m_mainUpsInfo.upsNet = 0;
      }
    if (m_myDock) { delete m_myDock; m_myDock = 0;}
    qApp->quit();
    }
  }

#include "moc_knutclient.cpp"
