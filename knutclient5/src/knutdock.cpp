/**************************************************************************
                          knutdock.cpp  -  description
                             -------------------
    begin                : Út zari 23 2003
    copyright            : (C) 2003 by Daniel Prynych
    email                : Daniel@Prynych.cz
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "knutdock.h"
#include "knutnet.h"


#include <kapplication.h>
#include <KIconLoader>
#include <KAboutApplicationDialog>
#include <KAboutData>
#include <KColorScheme>
#include <KLocalizedString>
//#include <KStatusNotifierItem>


#include <QMenu>
#include <QPalette>
#include <QPainter>
#include <QToolTip>
#include <QPixmap>
#include <QImage>
#include <QBrush>
#include <QString>
#include <QAction>
#include <QActionGroup>
#include <QPixmap>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

//#include <iostream>

KNutDock::KNutDock(KNutVarData::upsInfoDef* activeUpsInfo, KNutVarData::SettingDataDef* settingData, KNutUpsData* upsRecords, QWidget* ) 
	
//  : KSystemTrayIcon (NULL) 
  : KStatusNotifierItem()

  {
  QString valueName;

  qDebug("KNutDock::constructor");
  m_settingData = settingData;
  m_upsRecords = upsRecords;
  m_activeUpsInfo = activeUpsInfo;

//saves backgroundcolor of kicker 
//+++++++  bude potreba zjistit tuto hodnotu odjinut 
KColorScheme sysTrayColorScheme(QPalette::Active,KColorScheme::View);

m_backgroundGlobalBrush = sysTrayColorScheme.background(KColorScheme::NormalBackground);

m_textBrush = sysTrayColorScheme.foreground(KColorScheme::NormalText);
m_textColor = m_textBrush.color();
//m_textBrush.setColor();

  m_typeOfIcon = m_settingData->typeOfKIcon;

  if (!m_settingData->customKIconColor) {
     m_backgroundBrush = m_backgroundGlobalBrush;
     }
  else {
     m_backgroundBrush = QBrush(m_settingData->kIconBackgroundColor);

     }

  m_toolTipFlags=m_settingData->toolTipDockFlags;

  clearValues();

  qDebug ("load Icons");

  loadIcons();
  loadImages();

  createMainMenu();

  m_mainLayer = new QImage(22,22,QImage::Format_ARGB32);
  m_backgroundLayer = new QImage(22,22,QImage::Format_ARGB32);
  setIconBackgroundLayer();

  qDebug ("initSysTrayImage");
  initSysTrayImage();
//  setToolTip("KNutClient");
  setToolTip("knutclient","KNutClient","");

  changeKIcon(); // this function calls function repaintIcon 

  // Uses own working of signal activated because in parent class signal activated is connected to activatedOrHide,
  //  but procedure activatedOrHide doesn't work when parent object is 0.
  connect (this, SIGNAL (activated ( QSystemTrayIcon::ActivationReason )), this, SLOT(slotActivated ( QSystemTrayIcon::ActivationReason ))); 
  }


KNutDock::~KNutDock(){

  delete m_mainLayer;
  delete m_backgroundLayer;
  delete m_menu;
  }



void KNutDock::loadIcons (void) {


/*
  QIcon undoicon = QIcon::fromTheme("edit-undo");
  testPix = QIcon::fromTheme("edit-undo");
  */
  m_errorIcon  = QIcon(UserIcon("knc_error"));
  m_connectionIcon = QIcon (UserIcon("knc_conn"));
  m_idleIcon = QIcon(UserIcon("knc_main.png"));
}


void KNutDock::setIconBackgroundLayer (void) {
  QPainter paint; 
  paint.begin(m_backgroundLayer);
  paint.fillRect(QRect(0,0,22,22),m_backgroundBrush);
  paint.end();
}


void KNutDock::loadImages (void) {
  // aplications's own icons  -- there are beckground for dock icons
  m_upsPix = UserIcon("knc_ups").toImage();
  m_battPix = UserIcon("knc_batt").toImage();
}


void KNutDock::changeUps (void) {

  m_upsServerMenu->clear();
  createUpsMenu();
  }


void KNutDock::clearDockValues(void){

  clearValues();
  initSysTrayImage(); // paints standart icon
  }


void KNutDock::changeToolTip(void){

  m_toolTipFlags=m_settingData->toolTipDockFlags;
  toolTip(m_runtimeValue, m_statusValue, m_batteryChargeValue, m_upsLoadValue);
  }


void KNutDock::changeKIBColor(void) {

  if (!m_settingData->customKIconColor) {
    m_backgroundBrush = m_backgroundGlobalBrush;
    }
  else {
    m_backgroundBrush = QBrush(m_settingData->kIconBackgroundColor);
    }
  setIconBackgroundLayer();
  repaintDock(true);
  }

void KNutDock::changeKIcon (void) {
  m_typeOfIcon = m_settingData->typeOfKIcon;

  m_statusWarningValue = m_statusValue & (KNutNet::CAL | KNutNet::TRIM | KNutNet::BOOST
                     | KNutNet::OVER | KNutNet::RB);
  repaintDock(true);
  }

void KNutDock::initSysTrayImage(void) {
///DAN//  setIcon(m_idleIcon);
  setIconByPixmap(m_idleIcon); 	
  m_dataOk = m_dataOkOld= idle;  //status of ups connecting -- ups isn't connected
  }

void KNutDock::setNewUPS(void) {
  m_upsServerMenu->clear();
  createUpsMenu();
  }

void KNutDock::repaintDock (const bool always) {
  qDebug ("KNutDock::repaintDock");

  if (m_activeUpsInfo->upsNet == 0)  return;

  m_dataOkOld=m_dataOk; 
 


  if (m_activeUpsInfo->netError)  m_dataOk = error; // error is value not variable
  else { // makes OKIcon when is needed and sets m_dataOk - status of Icon
    if ((m_activeUpsInfo->upsNet->getState() == KNutNet::Connected) &&
       (m_activeUpsInfo->upsNet->readStatus() != KNutNet::NS)) { // paints icons with information values //m_statusValue : NS  is  NONE STATUS
      m_dataOk = ok;
      if (dataValueIsChanged() || always) makeOkIcon();
      toolTip(m_runtimeValue, m_statusValue, m_batteryChargeValue, m_upsLoadValue);
      }
    else {
      if ((m_activeUpsInfo->upsNet->getState() == KNutNet::Connecting) ||
        (m_activeUpsInfo->upsNet->getState() == KNutNet::HostLookup))
        m_dataOk = connecting;
      else m_dataOk = idle; 
      }
    }


  if (m_dataOk == ok) {
///DAN///	  
//    setIcon(QIcon( QPixmap::fromImage(*m_mainLayer)));
    setIconByPixmap(QIcon( QPixmap::fromImage(*m_mainLayer)));
    }
  else {
    if ((m_dataOk != m_dataOkOld) || always) {
      toolTip(0, -1, 0, 0);
      m_dataOkOld=m_dataOk; 
///DAN///      
//      if (m_dataOk == connecting) setIcon(m_connectionIcon);  // knutNet is in connecting state
      if (m_dataOk == connecting) setIconByPixmap(m_connectionIcon);  // knutNet is in connecting state
      else {
///DAN//
//        if (m_dataOk == error) setIcon(m_errorIcon);  // knutNet is in error state
        if (m_dataOk == error) setIconByPixmap(m_errorIcon);  // knutNet is in error state
        else  {
///DAN///
//          setIcon(m_idleIcon);  // knutNet is in idle state
          setIconByPixmap(m_idleIcon);  // knutNet is in idle state
          }
        }
      }
    }
  }


bool KNutDock::dataValueIsChanged(void) {
  QString valueName;
  bool okFloat;
  QString nutStringVar;

  int myStatusValue;
  int myStatusBatValue;
  int myRuntimeValue;
  int myUpsLoadValue;
  int myBatteryChargeValue;

  bool repaintNow = false;  //value isn't changed

//  **********reads values of STATUS ***************
   myStatusValue = m_activeUpsInfo->upsNet->readStatus();
   myStatusBatValue = myStatusValue & KNutNet::OB; // OB = UPS runs on battery 

//  **********reads value of RUNTIME***************
    if (!m_activeUpsInfo->upsNet->existName(m_activeUpsInfo->runtimeVar)) {
      nutStringVar = m_activeUpsInfo->upsNet->readStringVar(m_activeUpsInfo->runtimeVar);
      if (nutStringVar.isEmpty()) myRuntimeValue=knc::numberVarError;
      else {
        myRuntimeValue = (int)nutStringVar.toFloat(&okFloat);
        if (!okFloat) myRuntimeValue = knc::numberVarError;
        }
      }
    else myRuntimeValue = knc::numberVarError;

//  **********reads value of UPS LOAD***************
    if (m_activeUpsInfo->nutVariables == 1) valueName = "LOADPCT";
    else valueName = "ups.load";
    if (!m_activeUpsInfo->upsNet->existName(valueName)) {
      nutStringVar = m_activeUpsInfo->upsNet->readStringVar(valueName);
      if (nutStringVar.isEmpty()) myUpsLoadValue=knc::numberVarError;
      else {
        myUpsLoadValue = (int)nutStringVar.toFloat(&okFloat);
        if (!okFloat) myUpsLoadValue=knc::numberVarError;
        }
      }
    else myUpsLoadValue=knc::numberVarError;

//  *********reads value of  BATTERY CHARGE****************
    if (m_activeUpsInfo->nutVariables == 1) valueName="BATTPCT";
    else valueName="battery.charge";
    if (!m_activeUpsInfo->upsNet->existName(valueName)) {
      nutStringVar = m_activeUpsInfo->upsNet->readStringVar(valueName);
      if (nutStringVar.isEmpty()) myBatteryChargeValue=knc::numberVarError;
      else {
        myBatteryChargeValue = (int)nutStringVar.toFloat(&okFloat);
        if (!okFloat) myBatteryChargeValue = knc::numberVarError;
        }
      }
    else myBatteryChargeValue=knc::numberVarError;


//   **********STATUS**************
    if (myStatusValue !=m_statusValue  ) {
      m_statusValue = myStatusValue;
      m_statusWarningValue = myStatusValue & (KNutNet::CAL | KNutNet::TRIM | KNutNet::BOOST
                         | KNutNet::OVER | KNutNet::RB);
      repaintNow = true;
      }

//   **********RUNTIME***********
      if (myRuntimeValue != m_runtimeValue)  {
        m_runtimeValue = myRuntimeValue; // runtime is old value of runtime, one is defined like global value of class
        repaintNow = true;
        }

//  ************LOAD********************
    if (myUpsLoadValue != m_upsLoadValue) {
      m_upsLoadValue = myUpsLoadValue;
      repaintNow = true;
    }


//  ***********CHARGE******************
    if (myBatteryChargeValue != m_batteryChargeValue) {
      m_batteryChargeValue = myBatteryChargeValue;
      repaintNow = true;
      }

//  ************** STATUS BATT***************
//   for set repaintNow only 
      if  (myStatusBatValue != m_statusBatValue) {
        m_statusBatValue = myStatusBatValue;
        repaintNow = true;
        }

  return repaintNow;
}

void KNutDock::makeOkIcon(void) {
QPainter paint;

    switch (m_typeOfIcon) {
      case generalKI:

      *m_mainLayer = m_backgroundLayer->copy(QRect(0,0,22,22)); 
      paint.begin(m_mainLayer);

      paintStatus(paint, m_statusValue);

      paintRuntime(paint, m_runtimeValue, m_statusValue);

      paintLoadGeneralKI(paint, m_upsLoadValue);
      paintChargeGeneralKI(paint, m_batteryChargeValue);
      paint.end();
      break;

      case pictureKI:
     
      if (m_statusValue & KNutNet::OL) {  // ONLINE - icon of UPS
        int myLoad;

        QPainter paint;
        *m_mainLayer = m_upsPix.copy(QRect(0,0,22,22)); 
         paint.begin(m_mainLayer);

         if (m_upsLoadValue == knc::numberVarError) myLoad=0;
         else {
            myLoad = (int)(((double)m_upsLoadValue/10)+0.5);
            } 

         if (m_upsLoadValue != knc::numberVarError) {
         if (myLoad > 12) myLoad=12;
         if (myLoad > 10) myLoad++; // preskocime rysku pro 100 %
         if (myLoad < 9) paint.setPen(QColor(0,192,0)); //green
         else {
           if (myLoad < 11) paint.setPen(QColor(255,255,0)); //yellow
           else paint.setPen(QColor(255,0,0));//red
           }
        for (int i = 19; i < 22; i++) paint.drawLine(i,12,i,12-myLoad +1);

         if (myLoad < 13) {
           paint.setPen(QColor(168,220,255));
           for (int i = 20; i < 23; i++ ) paint.drawLine(i,0,i,12-myLoad);
           }
        }
        paint.setPen(m_textColor); // line for 100 % load
        paint.drawLine(18,2,21,2);

        paintNumbers (m_upsLoadValue, -2, 15, &paint );

        if (!m_statusWarningValue) paintOKPoint(paint);
        else paintErrorPoint(paint);
        paint.end();
        }
      else {  // offline -- battery icon
        *m_mainLayer = m_battPix.copy(QRect(0,0,22,22)); 
        paint.begin(m_mainLayer);
        paintChargePictureKI(paint, m_batteryChargeValue);
        paint.end();
        }
      break;
      } /////////////////////  end of models
}

void KNutDock::toolTip (int runtime, int status, int batteryCharge, int upsLoad ) {
qDebug("KnutDock::toolTip");

  QString text;
  QString manufacturer = "";
  QString upsModel = "";
  QString serial = "";
  QString firmware = "";
  int myNetErr = 0;

  int min = abs(runtime/60);
  int sec = runtime - (min*60);

  if (m_activeUpsInfo->upsNet != 0) {
    myNetErr= m_activeUpsInfo->upsNet->getError();
    switch (m_activeUpsInfo->nutVariables) {
      case KNutNet::versionVarOne:
        manufacturer = m_activeUpsInfo->upsNet->readStringVar("MFR");
        upsModel = m_activeUpsInfo->upsNet->readStringVar("MODEL");
        serial = m_activeUpsInfo->upsNet->readStringVar("SERIAL");
        firmware = m_activeUpsInfo->upsNet->readStringVar("FIRMREV");
      break;
      case KNutNet::versionVarTwo:
        manufacturer = m_activeUpsInfo->upsNet->readStringVar("ups.mfr");
        upsModel = m_activeUpsInfo->upsNet->readStringVar("ups.model");
        serial = m_activeUpsInfo->upsNet->readStringVar("ups.serial");
        firmware = m_activeUpsInfo->upsNet->readStringVar("ups.firmware");
      break;
      case KNutNet::versionVarUnknown:
        manufacturer = "";
        upsModel = "";
        serial = "";
        firmware = "";
      break;
      }
    }

  text = i18n("Name")+" : "+m_activeUpsInfo->record.name + "\n"
  +i18n("UPS name")+" : "+m_activeUpsInfo->record.upsName + "\n"
  +i18n("UPS address")+" : "+m_activeUpsInfo->record.upsAddress +"\n"
  +i18n("Status : ");

  if (status == -1) {
    text += i18n("Connection doesn't exist.");
    }
  else {
    if (status & KNutNet::OL) {
      text += i18n("UPS On line");
      }
    else {
      if (status & KNutNet::OB) {
        text += i18n("UPS On battery");
        }
      else {
        if (status & KNutNet::OFF) {
          text += i18n("UPS Of line");
          }
        }  
      }

    if (status & KNutNet::LB) {
      text += "\n" + i18n("Status : ") + i18n("UPS Battery is low");
      }
    if (status & KNutNet::RB) {
      text += "\n" + i18n("Status : ") + i18n("Replace battery");
      }
    if (status & KNutNet::CAL) {
      text += "\n" + i18n("Status : ") + i18n("UPS is performing calibration");
      }
    if (status & KNutNet::OVER) {
      text += "\n" + i18n("Status : ") + i18n("UPS is Overload");
      }
    if (status & KNutNet::TRIM) {
      text += "\n" + i18n("Status : ") + i18n("UPS is trimming voltage");
      }
    if (status & KNutNet::BOOST) {
      text += "\n" + i18n("Status : ")+ i18n("UPS is boosting voltage");
      }


    }
  if ((status == -1) && (myNetErr != KNutNet::NoError)) {
    text += "\n" + i18n("Error : ")+ KNutVarData::errorToText(myNetErr);
    }

  if (m_toolTipFlags & TTMFR) text += "\n" + i18n("Manufac. : ")+ manufacturer;

  if (m_toolTipFlags & TTModel) text += "\n" + i18n("Name")+" : "+ upsModel;

  if (m_toolTipFlags & TTSerial) text += "\n" + i18n("Serial")+" : "+ serial;

  if (m_toolTipFlags & TTFirm) text += "\n" + i18n("Firmware")+" : "+ firmware;

  if (m_toolTipFlags & TTRun) {
    if (runtime != knc::numberVarError) {
//      QString strRuntime = QString(" %1:%2 min").arg(min).arg(sec);
    //  text += "\n" + i18n("Runtime") + i18n(" : %1:%2 min").arg(min).arg(sec);
      text += "\n" + i18n("Runtime") + QString(" %1:%2 min").arg(min).arg(sec);
      }
    }

  if (m_toolTipFlags & TTCharge) {
    if (batteryCharge != knc::numberVarError)
      text += "\n" + i18n("Battery Charge") + QString(" : %1 %").arg(batteryCharge);
    }

  if (m_toolTipFlags & TTLoad) {
    if (upsLoad != knc::numberVarError)
      text += "\n" + i18n("UPS Load") + QString(" : %1 %").arg(upsLoad);
    }
//  setToolTip(text);
  setToolTipTitle(text);
 }



/*********************************************************************************************/
/*                                                                                           */
/*                         PRIVATE SLOTS                                                     */
/*                                                                                           */
/*********************************************************************************************/

 void KNutDock::slotActivated ( QSystemTrayIcon::ActivationReason reason ) {
 
 if (reason == QSystemTrayIcon::Trigger) { 
   activeMainWindow();
   }
 }


void KNutDock::activate(const QPoint &pos) {
   Q_UNUSED(pos);
   activeMainWindow();
}

  void KNutDock::slotShowMyAbout() {

  /* KAboutApplicationDialog *dialog = new KAboutApplicationDialog(KGlobal::config()->componentData().aboutData()); */
//	  KAboutData KAboutData::applicationData();
  
  KAboutApplicationDialog *dialog = new KAboutApplicationDialog(KAboutData::applicationData());

/*
  KAboutApplicationDialog *dialog = new KAboutApplicationDialog(KSharedConfig::openConfig()->componentData().aboutData());
*/
  dialog->exec();
  delete dialog;
  }

void KNutDock::slotExitApplication (void) {

      emit endApplication();
  }

void KNutDock::slotUpsName(QAction *upsAction) {

  QString upsName(upsAction->text());
  upsName.remove('&');
  emit UPSChanged(upsName);
  }


void KNutDock::slotMinimize(void) {

  emit minimize();
  }

void KNutDock::slotPreferences(void) {

  emit preferences();
  }

void KNutDock::slotPrintUPSVars(void) {

  emit printUPSVars();
  }

void KNutDock::slotInstComms(void) {

  emit instComms();
  }

void KNutDock::slotRWVars(void) {

  emit RWVars();
  }

void KNutDock::slotRefreshConnection(void) {

  emit makeRefreshConnection();
  }


//------------------------------------------

//bool KNutDock::event(QEvent *e) {
//
//  if ( e->type() == QEvent::MouseButtonPress) {
//    qDebug("KNutDock::mousePressEvent");
//    QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(e);
//    if( mouseEvent->button()==Qt::LeftButton  ) emit activeMainWindow();
//    if( mouseEvent->button()==Qt::RightButton ) m_menu->popup(mouseEvent->globalPos());
//    }
//  else {
//    return KSystemTrayIcon::event( e ); 
//    }
// return true; 
// }





/*********************************************************************************************/
/*                                                                                           */
/*                         PRIVATE FUNCTIONS                                                 */
/*                                                                                           */
/*********************************************************************************************/

void KNutDock::clearValues(void) {

  m_batteryChargeValue = 0;
  m_runtimeValue = 0;
  m_upsLoadValue = 0;
  m_statusValue = 0;
  }


void KNutDock::createUpsMenu(void) {
  QActionGroup *upsGroup = new QActionGroup(this);

  int n = m_upsRecords->getCount();
  for (int i =0; i < n; i++) {

    QAction *upsAction = new QAction (m_upsRecords->getName(i),this);
    upsAction->setCheckable (true);
    upsGroup->addAction(upsAction);
    if (m_activeUpsInfo->record.name == m_upsRecords->getName(i)) {
      upsAction->setChecked(true);
      }
    m_upsServerMenu->addAction(upsAction);
    }
    connect( upsGroup, SIGNAL( triggered(QAction *) ), this, SLOT(slotUpsName(QAction *) ));
 }

void KNutDock::createMainMenu(void) {
  m_menu = new QMenu();


  QString KNCName = KAboutData::applicationData().desktopFileName();
/* dan */  
  m_upsServerMenu = (QMenu *)m_menu->addMenu(QIcon(),"UPS");
  m_menu->addSeparator();

  m_menu->addAction(QIcon("view-list-tree"),i18n("&Showing UPS variables and commands"), this, SLOT(slotPrintUPSVars()));
  m_menu->addAction(i18n("&Running instant commands"), this, SLOT(slotInstComms()));
  m_menu->addAction(i18n("Setting R&W variables"), this, SLOT(slotRWVars()));
  m_menu->addSeparator();
  m_menu->addAction(QIcon("view-refresh"),i18n("Reconnect"), this, SLOT(slotRefreshConnection()));
  m_menu->addSeparator();
  m_menu->addAction(QIcon("configure"),i18n("&Preferences"), this, SLOT(slotPreferences()));
  m_menu->addSeparator();
  m_menu->addAction(QIcon(KNCName),i18n("&About KNutClient"), this, SLOT(slotShowMyAbout()));
  m_menu->addSeparator();
  m_menu->addAction(i18n("&Minimize"), this, SLOT(slotMinimize()));
  m_menu->addSeparator();
  m_menu->addAction (QIcon("application-exit"),i18n("&Exit"),this ,SLOT(slotExitApplication()));
  
  createUpsMenu();
  setContextMenu(m_menu);
  }


void KNutDock::paintChargeGeneralKI (QPainter &paint, int charge ) {

  if (charge > 199) charge = 199;

  if (m_typeOfIcon == generalKI) {
    if (charge != knc::numberVarError) {
      if (charge < 40) paint.setPen(Qt::red);
      else paint.setPen(m_textColor);
      paintNumbers(charge,8,2,&paint);
      }
    else {
      paint.setPen(m_textColor);
      paintDash (13,2,&paint);
      paintDash (18,2,&paint);
      }
    }
  }


void KNutDock::paintChargePictureKI (QPainter &paint, int charge ) {

  if (charge != knc::numberVarError) {
    if (m_batteryChargeValue < 40) paint.setPen(Qt::red);
    else paint.setPen(m_textColor);
    
    paintNumbers(m_batteryChargeValue,-2,15,&paint);
    
    if (m_batteryChargeValue < 40) paint.setPen(Qt::red);
    else paint.setPen(Qt::green);

    int delka = abs((16*m_batteryChargeValue)/100);
    delka += abs((delka-1)/4) -1;
    paint.drawLine (1,7,1+delka,7);
    paint.drawLine (1,8,1+delka,8);
    paint.drawLine (1,9,1+delka,9);
    
    paint.setPen(m_textColor);

    paint.drawLine (5,6,5,10);
    paint.drawLine (10,6,10,10);
    paint.drawLine (15,6,15,10);

    }
  else {
    paint.setPen(m_textColor);
    paintDash (3,15,&paint);
    paintDash (8,15,&paint);
    }
}

void KNutDock::paintErrorPoint ( QPainter& paint ) {

//  paint.setPen(QColor(0,0,255));
  paint.setPen(QColor(255,0,0));

  paint.drawLine(19,17,21,17);
  paint.drawLine(19,18,21,18);
  paint.drawLine(19,19,21,19);
  }

void KNutDock::paintOKPoint (QPainter& paint  ) {

  paint.setPen(QColor(0,255,0));
  paint.drawLine(19,17,21,17);
  paint.drawLine(19,18,21,18);
  paint.drawLine(19,19,21,19);
  }


void KNutDock::paintStatus (QPainter &paint, int status ) {

  int lastPos = 0;

  if (status & KNutNet::OL) {
    paint.setPen(m_textColor);
    paintChar (1,2,'P',&paint);
    }
  else {
    if (status & KNutNet::OB) {
      paint.setPen(Qt::red);
      paintChar (1,2,'B',&paint);
      }
    }

  if (status & KNutNet::LB) {
    paint.setPen(Qt::red);
    paintChar (6,2,'L',&paint);
    }
  if ((lastPos <2) && (status & KNutNet::RB)) {
    paint.setPen(Qt::red);
    paintChar (6,2,'R',&paint);
    lastPos++;
    }
  if ((lastPos <2) && (status & KNutNet::BOOST)) {
    paint.setPen(Qt::red);
    paintChar (6,2,'B',&paint);
    lastPos++;
    }
  if ((lastPos <2) && (status & KNutNet::TRIM)) {
    paint.setPen(Qt::red);
    paintChar (6,2,'T',&paint);
    lastPos++;
    }
  if ((lastPos <2) && (status & KNutNet::CAL)) {
    paint.setPen(Qt::red);
    paintChar (6,2,'C',&paint);
    lastPos++;
    }
  }

void KNutDock::paintRuntime (QPainter &paint, int runtime, int status ) {

  if (status & KNutNet::OB)
    paint.setPen(Qt::red);
  else
    paint.setPen(m_textColor);

  if (runtime != knc::numberVarError) {

    int min = abs(runtime/60);
    int sec = runtime - (min*60);

    paintNumber(1,11, abs(min/10) ,&paint);
    paintNumber(6,11,min-((abs(min/10))*10) ,&paint);
    paintNumber(13,11, abs(sec/10) ,&paint);
    paintNumber(18,11,sec-((abs(sec/10))*10) ,&paint);
    }
  else {
    paintDash (1,11,&paint);
    paintDash (6,11,&paint);
    paintDash (13,11,&paint);
    paintDash (18,11,&paint);
    }

  paint.drawLine (11,12,11,13);
  paint.drawLine (11,15,11,16);
  }

void KNutDock::paintLoadGeneralKI (QPainter &paint,  int load ) {
  int myLoad;

  if (load == knc::numberVarError) myLoad=0;
  else myLoad = abs(load/10);

  if (m_typeOfIcon == generalKI) {
    if (load != knc::numberVarError) {
      if (myLoad > 11) myLoad=11;
      if (myLoad < 9) paint.setPen(Qt::green);
      else {
        if (myLoad < 11) paint.setPen(Qt::yellow);
        else paint.setPen(Qt::red);
        }
      paint.setPen(QColor(Qt::green).light(60));
      for (int i = 0; i < myLoad; i++) {
        paint.drawLine((i*2)+1,19,(i*2)+1,21);
        }
      paint.setPen(QColor(Qt::cyan).light(150));
      for (int i = myLoad; i < 11; i++) {
        paint.drawLine((i*2)+1,19,(i*2)+1,21);
        }
      }
    }
  }




void KNutDock::paintNumbers (int numbers, int x, int y, QPainter* p ) {
  bool first = false;

  if (numbers > 199) numbers = 199;

  if (numbers != knc::numberVarError) {

    if (numbers > 99) {
      paintNumber (x,y,1,p);
      numbers -= 100;
      first=true;
      }
    int second = abs (numbers/10);
    int third = numbers - (second * 10);

    if ((second != 0) || (first)) paintNumber (x+5,y,second,p);
    paintNumber (x+10,y,third,p);

    }
  else {
    p->setPen(m_textColor);
    paintDash (x+5,y,p);
    paintDash (x+10,y,p);
    }
  }


void KNutDock::paintDash (int x, int y, QPainter *p) {

  p->drawLine(x,y+3,x+3,y+3);
  }

void KNutDock::paintNumber (int x, int y, int number, QPainter *p) {
  // x and y are left top corner

  if ((number > -1) && (number < 10)) {
   // painting - kreslim
    switch (number) {
      case 0:
      p->drawRect(x,y,3,6);
      break;
      case 1:
        p->drawLine (x+3,y,x+3,y+6);
      break;
      case 2:
        p->drawLine(x,y,x+3,y);
        p->drawLine(x+3,y,x+3,y+3);
        p->drawLine(x+3,y+3,x,y+3);
        p->drawLine(x,y+3,x,y+6);
        p->drawLine(x,y+6,x+3,y+6);
      break;
      case 3:
        p->drawLine(x,y,x+3,y);
        p->drawLine(x+3,y,x+3,y+6);
        p->drawLine(x+3,y+6,x,y+6);
        p->drawLine(x,y+3,x+3,y+3);
      break;
      case 4:
        p->drawLine(x,y,x,y+3);
        p->drawLine(x,y+3,x+3,y+3);
        p->drawLine (x+3,y,x+3,y+6);
      break;
      case 5:
        p->drawLine(x+3,y,x,y);
        p->drawLine(x,y,x,y+3);
        p->drawLine(x,y+3,x+3,y+3);
        p->drawLine(x+3,y+3,x+3,y+6);
        p->drawLine(x+3,y+6,x,y+6);
      break;
      case 6:
        p->drawRect(x,y+3,3,3);
        p->drawLine(x,y+3,x,y);
        p->drawLine(x,y,x+3,y);
      break;
      case 7:
        p->drawLine(x,y,x+3,y);
        p->drawLine(x+3,y,x+3,y+6);
      break;
      case 8:
        p->drawRect(x,y,3,6);
        p->drawLine(x,y+3,x+3,y+3);
      break;
      case 9:
        p->drawRect(x,y,3,3);
        p->drawLine(x+3,y+3,x+3,y+6);
        p->drawLine(x+3,y+6,x,y+6);
      break;
      }
    } 
  }


void KNutDock::paintChar (int x, int y, char LCDChar, QPainter *p) {
  // x and y are left up  corner
  switch (LCDChar) {
    case 'L':
      p->drawLine(x,y,x,y+6);
      p->drawLine(x,y+6,x+3,y+6);
    break;
    case 'O':
      p->drawRect(x,y,4,7);
    break;
    case 'B':
      p->drawLine(x,y,x,y+6);
      p->drawLine(x,y+6,x+2,y+6);
      p->drawLine(x,y,x+2,y);
      p->drawLine(x,y+3,x+2,y+3);
      p->drawLine(x+3,y+1,x+3,y+2);
      p->drawLine(x+3,y+4,x+3,y+5);
    break;
    case 'U':
      p->drawLine(x,y,x,y+6);
      p->drawLine(x,y+6,x+3,y+6);
      p->drawLine(x+3,y+6,x+3,y);
    break;
    case 'C':
      p->drawLine(x+3,y+1,x+3,y);
      p->drawLine(x+3,y,x,y);
      p->drawLine(x,y,x,y+6);
      p->drawLine(x,y+6,x+3,y+6);
      p->drawLine(x+3,y+6,x+3,y+5);
    break;
    case 'R':
      p->drawLine(x,y,x,y+6);
      p->drawLine(x,y,x+2,y);
      p->drawLine(x,y+3,x+2,y+3);
      p->drawLine(x+3,y+1,x+3,y+2);
      p->drawLine(x+3,y+4,x+3,y+6);
    break;
    case 'P':
      p->drawLine(x,y,x,y+6);
      p->drawLine(x,y,x+2,y);
      p->drawLine(x,y+3,x+2,y+3);
      p->drawLine(x+3,y+1,x+3,y+2);
    break;
    case 'T':
      p->drawLine(x+2,y,x+2,y+6);
      p->drawLine(x,y,x+4,y);
    break;
    }
  }



#include "moc_knutdock.cpp"
