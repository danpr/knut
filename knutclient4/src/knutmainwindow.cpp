/***************************************************************************
                          knutmainwindow.cpp  -  description
                             -------------------
    begin                : Po cen 16 2003
    copyright            : (C) 2003 by Daniel Prynych
    email                : Daniel@prynych.cz
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "knutmainwindow.h"
#include "knutvardata.h"


#include <KDE/KToolBar>
#include <KDE/KShortcutsDialog>
#include <KDE/KStatusBar>
#include <KDE/KStandardAction>
#include <KDE/KActionCollection>
#include <KDE/KConfig>

#include <QtGui/QProgressBar>
#include <QtGui/QCloseEvent>
#include <QtGui/QFont>
#include <QtGui/QPalette>
#include <QtGui/QBrush>
#include <QtGui/QColor>
#include <QtCore/QString>
#include <QtGui/QScrollArea>
#include <QtGui/QLayout>
#include <QtGui/QIconSet>
#include <QtCore/QStringList>
#include <QtGui/QLabel>


#include <math.h>
#include <stdlib.h>
#include <unistd.h>

#include <iostream>


const char *descrOfConnection = I18N_NOOP("test of conection from 5");


KNutMainWindow::KNutMainWindow(QStringList *tListKNutEvent, KNutVarData::upsInfoDef* tActiveUpsInfo, KNutVarData::SettingDataDef* tSettingData, KNutUpsData* tUpsRecords, KNutVarData* tAnalogRecords, QWidget* parent) : KXmlGuiWindow(parent),
                   m_listKNutEvent(tListKNutEvent), m_activeUpsInfo(tActiveUpsInfo),m_settingData (tSettingData),
                   m_upsRecords(tUpsRecords), m_analogRecords (tAnalogRecords) {


qDebug("KNutMainWindow::KNutMainWindo");

  m_settingData->activatedMainWindow=true; //information about state of mainWindow

  initVars();
  initStatusBar();
  initAction();
  initToolBarAction();
//  createGUI ("/home/dan/Projekty/knutclient/src/knutclientui.rc"); // pred distribuci nezapomenout odstranit retezec
  // createGUI vytvori zakladni strukturu podle souboru knutclientui.rc
  //createGUI ();
  setupGUI();
  setToolBar();

  m_descriptionAction->setChecked(m_settingData->useDescription);

  //sets input frequency and input voltage   
  //nastavime vstupni frekvenci a vstupni napeti
  m_analogRecords->setFrequency(m_settingData->inputFrequency);
  m_analogRecords->setVoltage(m_settingData->inputVoltage);

  //sets palettes
  setPalettes();

  // sets font for front panel
  QFont frontPanelFont;
  if (m_settingData->customFont)
    frontPanelFont = m_settingData->mPanelFont;
  else
    frontPanelFont  = m_settingData->systemFont;

  // makes main area // Vytvorime hlavni plochu
  m_mainArea = new QWidget (this);
  setCentralWidget (m_mainArea);

  QHBoxLayout* mainLayout = new QHBoxLayout(m_mainArea);
  m_frontPanel = new KNutFrontPanel (m_mPanelPalette, frontPanelFont, m_mainArea,  m_settingData->panelFlags);
  mainLayout->addWidget(m_frontPanel,0);

//std::cout << "Hint " << m_frontPanel->sizeHint().width() << std::endl;
//std::cout << "Maximum " << m_frontPanel->maximumWidth() << std::endl;
//std::cout << "Minumum " << m_frontPanel->minimumWidth() << std::endl;

  QScrollArea *analogPanel = new QScrollArea(m_mainArea);
  m_panelMeter = new KNutPanelMeter(m_aPanelPalette,m_activeUpsInfo,m_settingData,m_analogRecords,analogPanel);

//std::cout << "Maximum " << m_panelMeter->sizeHint().width() << std::endl;
//std::cout << "Minumum " << m_panelMeter->minimumWidth() << std::endl;

  analogPanel->setWidget(m_panelMeter);
  analogPanel->setWidgetResizable (false);
  mainLayout->addWidget(analogPanel,0);

  setPalette(m_areaPalette);
  m_mainArea->setPalette(m_areaPalette);

  m_panelMeter->setPalette(m_areaPalette);
  // we have to set palette this again, because command "setPalette(m_areaPalette)" sets same palette for children widget too 
  m_frontPanel->setPalette(m_mPanelPalette);

  // sets Combo a actives connections to meter panels
  // Nastavime COMBO  a pripadne aktivujeme spojeni na merici panely
  int activeUpsNameIndex = activeMainCombo();
  if (activeUpsNameIndex != -1) m_upsComboBox->setCurrentIndex( activeUpsNameIndex );

  //creates analogs
  showUpsData();

  //loads position
  KConfigGroup config = KGlobal::config()->group("MainWindow");
  move(config.readEntry("PosX",int(0)), config.readEntry("PosY",int(0)));

  show();

  initStatusBarMessage();
  }



void KNutMainWindow::addComboStatusItem (const QString &text) {

  if (m_infoCombo->count() >= knc::maxCountKNutEvent) m_infoCombo->removeItem(0);
  m_infoCombo->addItem(text);
  m_infoCombo->setCurrentIndex(m_infoCombo->count()-1);
  }


KNutMainWindow::~KNutMainWindow(void) {

  saveConfiguration();

  delete m_frontPanel;
  delete m_panelMeter;
  }


void KNutMainWindow::setConfig (unsigned int changeSetting) {


  KNutVarData::VarStrucDef analogMemberData;
  bool repaintVoltagePanels = false;
  bool repaintFrequencyPanels = false;
  bool repaintAllPanels = false;
  bool repaintOAPanels = false;

  // Main Setting

  
  // sets background color for mainArea
  if (changeSetting & KNutPrefDlg::ChangeCustomBColor) {
    if (!m_settingData->customBColor) { // nastavime barvu pozadi podle systemu 
      setPalette (m_mainPalette); // we set background color based on system
      m_mainArea->setPalette (m_mainPalette); // we set background color based on system
      m_panelMeter->setPalette (m_mainPalette);
      repaint();
      }
    else {
      m_areaPalette.setColor(QPalette::Active,QPalette::Background,m_settingData->mainBackgroundColor);
      m_areaPalette.setColor(QPalette::Inactive,QPalette::Background,m_settingData->mainBackgroundColor);
      m_areaPalette.setColor(QPalette::Active,QPalette::Base,m_settingData->mainBackgroundColor);
      m_areaPalette.setColor(QPalette::Inactive,QPalette::Base,m_settingData->mainBackgroundColor);
      m_areaPalette.setColor(QPalette::Active,QPalette::Button,m_settingData->mainBackgroundColor);
      m_areaPalette.setColor(QPalette::Inactive,QPalette::Button,m_settingData->mainBackgroundColor);

      setPalette (m_areaPalette);
      m_mainArea->setPalette (m_areaPalette);
      m_panelMeter->setPalette (m_areaPalette);
      repaint();
      }
    }
  else {
    if (changeSetting & KNutPrefDlg::ChangeMBColor ) {
      m_areaPalette.setColor(QPalette::Active,QPalette::Background,m_settingData->mainBackgroundColor);
      m_areaPalette.setColor(QPalette::Inactive,QPalette::Background,m_settingData->mainBackgroundColor);
      m_areaPalette.setColor(QPalette::Active,QPalette::Base,m_settingData->mainBackgroundColor);
      m_areaPalette.setColor(QPalette::Inactive,QPalette::Base,m_settingData->mainBackgroundColor);
      m_areaPalette.setColor(QPalette::Active,QPalette::Button,m_settingData->mainBackgroundColor);
      m_areaPalette.setColor(QPalette::Inactive,QPalette::Button,m_settingData->mainBackgroundColor);
      setPalette (m_areaPalette);
      m_mainArea->setPalette (m_areaPalette);
      m_panelMeter->setPalette (m_areaPalette);
      repaint();
      }
    }

  // Sets background for panels
  // sets frequence
  // sets voltage
  // XFer is switch-on and one exist for this UPS


  if ((changeSetting & KNutPrefDlg::ChangeXFer) || // je provedena nejaka zmena na napeti nebo frekvence
      (changeSetting & KNutPrefDlg::ChangeVoltage) || (changeSetting & KNutPrefDlg::ChangeFrequency)) {

    if ((changeSetting & KNutPrefDlg::ChangeXFer) && (m_settingData->lowHighXfer) && (m_existLowHighXfer)) {
    // xfer was activated
      if (changeSetting & KNutPrefDlg::ChangeXFer) {
        if (abs((int)(230 - (m_lowXfer + (m_highXfer  - m_lowXfer) /2))) > abs((int)(120 - (m_lowXfer + (m_highXfer  - m_lowXfer) /2)))) {
          m_analogRecords->setVoltage(120);
          m_analogRecords->setFrequency(60);
          }
        else {
          m_analogRecords->setVoltage(230);
          m_analogRecords->setFrequency(50);
          }
        m_analogRecords->setXfer(m_lowXfer,m_highXfer);
        repaintVoltagePanels = true;
        repaintFrequencyPanels = true;
        }
      }
    else {
    // XFER neni aktivovano nebo je nepodporuje UPS
    // XFER isn;t activated or XFER isn't supported by UPS
    if ((changeSetting & KNutPrefDlg::ChangeVoltage) || ((changeSetting & KNutPrefDlg::ChangeXFer))) {
    // byla aktivovana zmena napeti nebo zmena xfer a zaroven ho ups podporuje
      m_analogRecords->setVoltage(m_settingData->inputVoltage);
      repaintVoltagePanels=true;
      }
    if ((changeSetting & KNutPrefDlg::ChangeFrequency) || ((changeSetting & KNutPrefDlg::ChangeXFer))) {
    // byla aktivovana zmena napeti nebo zmena xfer a zaroven ho ups podporuje
      m_analogRecords->setFrequency(m_settingData->inputFrequency);
      repaintFrequencyPanels=true;
      }
    }
  }

  // Analog setting
  // now DigitalProcesing of pointer
  if  (changeSetting & KNutPrefDlg::ChangeTypeOfDigProcessing) { //type of digital processing was changed
     m_panelMeter->changeAnalogPanelsDigProcessing(); //paint pointers with new digotal processing
     }

  if ((changeSetting & KNutPrefDlg::ChangeCustomAPanelBColor) || // barva panelu je zmenena, panel color is changed
      (changeSetting & KNutPrefDlg::ChangeAPBColor)) {
    if (m_settingData->customBAnalogColor) {
      m_aPanelPalette.setColor(QPalette::Active,QPalette::Background,m_settingData->aPanelBackgroundColor);
      m_aPanelPalette.setColor(QPalette::Inactive,QPalette::Background,m_settingData->aPanelBackgroundColor);
      repaintAllPanels=true;
      m_panelMeter->setAnalogBG(m_aPanelPalette);
      }
    else {
      m_aPanelPalette = m_mainPalette;
      repaintAllPanels=true;
      m_panelMeter->setAnalogBG(m_aPanelPalette);
      }
    }

  if ((changeSetting & KNutPrefDlg::ChangeCustomAPanelOColor) || // je provedena zmena barvy panelu
      (changeSetting & KNutPrefDlg::ChangeAPOColor)) {
     if (m_settingData->customOAnalogColor) {
       repaintAllPanels=true;
       repaintOAPanels=true;
       }
     else {
       repaintAllPanels=true;
       repaintOAPanels=true;
       }
    }
  // regeneration of panel uses for setting Main and setting Analog 
  // pregenerovani panelu slozi pro setting Main a setting Analog
  m_panelMeter->repaintAnalogPanels (repaintFrequencyPanels, repaintVoltagePanels, repaintAllPanels);

  if (m_activeUpsInfo->record.name != "") {
//    m_mainSplitter->refresh();
    }

  // UPS Setting
  if (changeSetting & KNutPrefDlg::ChangeUpsSetting ) activeMainCombo();
  else {
    if (changeSetting & KNutPrefDlg::ChangeUpsCount) activeMainCombo();
    }

  // Main Panel Setting
  // change of background color
  // zmena barvy pozadi;
  if (changeSetting & KNutPrefDlg::ChangeCustomMPanelBColor) {
    if (!m_settingData->customBPanelColor) { // nastavime barvu pozadi podle systemu
      m_frontPanel->setPalette (m_mainPalette);
      }
    else {
      m_mPanelPalette.setColor(QPalette::Active,QPalette::Background,m_settingData->mPanelBackgroundColor);
      m_mPanelPalette.setColor(QPalette::Inactive,QPalette::Background,m_settingData->mPanelBackgroundColor);
      m_mPanelPalette.setColor(QPalette::Active,QPalette::Button,m_settingData->mPanelBackgroundColor);
      m_mPanelPalette.setColor(QPalette::Inactive,QPalette::Button,m_settingData->mPanelBackgroundColor);
      m_frontPanel->setPalette (m_mPanelPalette);
      }
    }
  else {
    if (changeSetting & KNutPrefDlg::ChangeMPBColor ) {
      m_mPanelPalette.setColor(QPalette::Active,QPalette::Background,m_settingData->mPanelBackgroundColor);
      m_mPanelPalette.setColor(QPalette::Inactive,QPalette::Background,m_settingData->mPanelBackgroundColor);
      m_mPanelPalette.setColor(QPalette::Active,QPalette::Button,m_settingData->mPanelBackgroundColor);
      m_mPanelPalette.setColor(QPalette::Inactive,QPalette::Button,m_settingData->mPanelBackgroundColor);
      m_frontPanel->setPalette (m_mPanelPalette);
      }
    }

  if (changeSetting & KNutPrefDlg::ChangeMainPanelSetting ) {
    if (m_settingData->panelFlags != m_frontPanel->getPanelFlags()) {
      m_frontPanel->setPanelFlags(m_settingData->panelFlags);
      //aktivujema a presuneme prvky panelu
      // activates and moves items of panel
      m_frontPanel->paintPanel();
      }
    }

  // Font panel setting
  if (changeSetting & KNutPrefDlg::ChangeAnalogFont ) {
    QFont newAnalogFont;
    if (m_settingData->customFont) {
      newAnalogFont=m_settingData->aPanelFont;
      }
    else {
      newAnalogFont=m_settingData->systemFont;
      }

    m_panelMeter->slotSetPanelsFont(newAnalogFont);
    }
  if (changeSetting & KNutPrefDlg::ChangePanelFont ) {
    QFont newPanelFont;
    if (m_settingData->customFont) {
      m_frontPanel->slotSetPanelFont(m_settingData->mPanelFont);
      }
    else {
      m_frontPanel->slotSetPanelFont(m_settingData->systemFont);
      }
    }
  }




void KNutMainWindow::showUpsData (void) {

qDebug ("KNutMainWindow::showUpsData");

  if ((m_activeUpsInfo->upsNet != 0) && (!m_activeUpsInfo->netError) &&
     (m_activeUpsInfo->upsDriverState == knc::connected)) {
    QString nutStringVar;
    bool okNumber;

//front panel part
    int upsStatus=0;
    if ((upsStatus = m_activeUpsInfo->upsNet->readStatus ()) != -1) {
      m_frontPanel->setPanel(upsStatus);
      }

    // Nastaveni nemenych dat jsou precteny pouze jednou, predpoklada se jejich nemenost
    // setting of unchange data, ones are reading only one. 
    nutStringVar = m_activeUpsInfo->upsNet->readStringVar(m_activeUpsInfo->lowxferVar);
    switch (m_activeUpsInfo->nutVariables) {
      case KNutNet::versionVarOne:
        m_manufacturer = m_activeUpsInfo->upsNet->readStringVar("MFR");
        m_upsModel = m_activeUpsInfo->upsNet->readStringVar("MODEL");
        m_serial = m_activeUpsInfo->upsNet->readStringVar("SERIAL");
        m_firmrev = m_activeUpsInfo->upsNet->readStringVar("FIRMREV");
      break;
      case KNutNet::versionVarTwo:
        m_manufacturer = m_activeUpsInfo->upsNet->readStringVar("ups.mfr");
        m_upsModel = m_activeUpsInfo->upsNet->readStringVar("ups.model");
        m_serial = m_activeUpsInfo->upsNet->readStringVar("ups.serial");
        m_firmrev = m_activeUpsInfo->upsNet->readStringVar("ups.firmware");
      break;
      case KNutNet::versionVarUnknown:
        m_manufacturer = "";
        m_upsModel = "";
        m_serial = "";
        m_firmrev = "";
      break;

      }  
    QString allUpsName = m_manufacturer + " " + m_upsModel;
    m_frontPanel->setAllNameUps (allUpsName);
    m_frontPanel->setMFRUps (m_manufacturer);
    m_frontPanel->setNameUps (m_upsModel);
    m_frontPanel->setSerialUps (m_serial);
    m_frontPanel->setFirmRevUps (m_firmrev);
    // try if LOWXFER and HIGHXFER or our values are existed
    //Zjistime zda existuji LOWXFER a HIGHXFER pripadne jeji hodnoty

// analog panel part

    int runtimeVal =0;
    if ( !m_activeUpsInfo->upsNet->existName(m_activeUpsInfo->runtimeVar)) {
      nutStringVar = m_activeUpsInfo->upsNet->readStringVar(m_activeUpsInfo->runtimeVar);
      if (nutStringVar.isEmpty()) runtimeVal=knc::numberVarError;
      else {
        runtimeVal=nutStringVar.toInt(&okNumber);
        if (!okNumber) runtimeVal = knc::numberVarError;
        }
      }
    else runtimeVal=knc::numberVarError;
    m_frontPanel->setRuntime(runtimeVal);


    if (nutStringVar.isNull()) {
      m_lowXfer =-1;
      m_highXfer=-1;
      m_existLowHighXfer = false;
      }
    else {
      m_lowXfer = nutStringVar.toDouble();
      nutStringVar = m_activeUpsInfo->upsNet->readStringVar(m_activeUpsInfo->highxferVar);
      if (nutStringVar.isEmpty()) {
        m_highXfer = -1;
        m_existLowHighXfer = false;
        }
      else {
        m_highXfer = nutStringVar.toDouble();
        m_existLowHighXfer = true;
        }
      }

    m_panelMeter->deActivateAnalogs();

    // pokud je pouzito XFER nastavime hodnoty
    // when XFER is used we set values
      if (m_settingData->lowHighXfer) {
        // xfer je nastaveno
        // xfer is seted
        if (m_existLowHighXfer) {
          if (abs((int)(230 - (m_lowXfer + (m_highXfer  - m_lowXfer) /2))) > abs((int)(120 - (m_lowXfer + (m_highXfer  - m_lowXfer) /2)))) {
            m_analogRecords->setVoltage(120);
            m_analogRecords->setFrequency(60);
            }
          else {
            m_analogRecords->setVoltage(230);
            m_analogRecords->setFrequency(50);
            }
          m_analogRecords->setXfer(m_lowXfer,m_highXfer);
          }
        }

    m_panelMeter->activateAnalogs(); // makes panel and makes background of panel
    // doesn't repaint analog pointers, because pointers will be repainted twice
    }
  else {
    cleanPanels();
    }
  }


void KNutMainWindow::zeroingPanels (void) {

  m_panelMeter->zeroingPanels();
  m_frontPanel->setRuntime(knc::numberVarError);  
  m_frontPanel->setPanel(0);
  }


void KNutMainWindow::hideUpsData (void) {
  m_frontPanel->cleanPanel();
  }

void KNutMainWindow::setCombo (const QString &name) {
  if (m_upsComboBox->currentText() != name) {
    int i = m_upsComboBox->findText(name);
    if (i != -1) m_upsComboBox->setCurrentIndex(i);
    }
  }

void KNutMainWindow::repaintPanel (void) {
qDebug ("KNutMainWindow::repaintPanel");
  QString nutStringVar;

  if (m_activeUpsInfo->upsNet == 0) return;
  if (!m_activeUpsInfo->upsNet->existName(m_activeUpsInfo->runtimeVar)) {
    nutStringVar = m_activeUpsInfo->upsNet->readStringVar(m_activeUpsInfo->runtimeVar);
    if (nutStringVar.isEmpty()) m_frontPanel->setRuntime(0);
    else m_frontPanel->setRuntime( nutStringVar.toInt());
    }
  else m_frontPanel->setRuntime(knc::numberVarError);
  // precteme status a nastavime zarovky panelu
  int status = m_activeUpsInfo->upsNet->readStatus();
  m_frontPanel->setPanel(status);
  }


/// je volan vzdy po timeoutu
/// this function is called after timeout always

void KNutMainWindow::repaintAnalogs ( void ) {
qDebug("KNutMainWindow::repaintAnalogs");

  QString nutStringVar;
  float numVar;
  bool  newRange = false;

  if ((m_settingData->lowHighXfer) && (m_existLowHighXfer)) {
    QString nutStringVar;

    nutStringVar = m_activeUpsInfo->upsNet->readStringVar(m_activeUpsInfo->lowxferVar);
    if (! nutStringVar.isEmpty()) {
      float helpLowXfer = nutStringVar.toFloat();
      nutStringVar = m_activeUpsInfo->upsNet->readStringVar(m_activeUpsInfo->highxferVar);
      if (! nutStringVar.isEmpty()) {
        float helpHighXfer = nutStringVar.toFloat();
        if ((helpLowXfer != m_lowXfer) || (helpHighXfer != m_highXfer)) {
          m_lowXfer=helpLowXfer;
          m_highXfer=helpHighXfer;
          m_analogRecords->setXfer(m_lowXfer,m_highXfer);
          newRange = true;
          }
        }
      }
    }

  if (newRange) m_panelMeter->repaintAnalogPanels (true, true, false); 
  for (int i=0; i< knc::NumberOfVars; i++) {

//qDebug("KNutMainWindow::repaintAnalogs -- pre m_panelMeter %d %d",i,m_activeUpsInfo->record.upsVarCode[i]);

    if (m_activeUpsInfo->record.upsVarCode[i] != 0) { // jistota pokud by nam to nekde propadlo
      nutStringVar = m_activeUpsInfo->upsNet->readStringVar( KNutVarData::nameUpsVar(m_activeUpsInfo->record.upsVarCode[i],m_activeUpsInfo->nutVariables));

      if (nutStringVar.isEmpty()) {
        numVar=0;
        }
      else {
        numVar = nutStringVar.toFloat();
        }
      // changes range of panel for current ups power ....
      m_panelMeter->repaintScaleAnalogPanel (m_activeUpsInfo->record.upsVarCode[i], numVar);
       // changes range of panel for in/out frequency and voltage 50/60Hz and 120/230 V
      m_panelMeter->repaintAnalogPointer(m_activeUpsInfo->record.upsVarCode[i] ,numVar);
      }
    } // end for
    m_panelMeter->repaint();
  }



void KNutMainWindow::cleanPanels (void) {

  m_frontPanel->cleanPanel();
  m_panelMeter->deActivateAnalogs();
  }

void KNutMainWindow::startConnectPB ( const int count ) {
  m_connectProggresBar->setRange(0,count);
  m_connectProggresBar->setValue(1);
  m_connectProggresBar->show();
  }


void KNutMainWindow::setProgressConnectPB ( const int progress ) {
  m_connectProggresBar->setValue(progress);
  }


void KNutMainWindow::hideConnectPB ( void ) {
  m_connectProggresBar->setValue(0);
  m_connectProggresBar->hide();
  }


/*********************************************************************************************/
/*                                                                                           */
/*                         PRIVATE SLOTS                                                     */
/*                                                                                           */
/*********************************************************************************************/


void KNutMainWindow::slotDescription(void) {

  if (m_descriptionAction->isChecked()) {
    if (!m_activeUpsInfo->upsNet->isDescription()) m_activeUpsInfo->upsNet->getDescription();
    m_settingData->useDescription=true;
    }
  else m_settingData->useDescription=false;
  emit saveConf();
  }


void KNutMainWindow::slotPrintUPSVars(void) {

  emit showUPSVars();
}


void KNutMainWindow::slotInstComms(void) {

  emit makeInstComms();
  }

void KNutMainWindow::slotRWVars(void) {

  emit makeRWVars();
  }

void KNutMainWindow::slotRefreshConnection(void) {

  emit makeRefreshConnection();
  }


void KNutMainWindow::slotChangeRWVars ( QString ) {
  }



void KNutMainWindow::closeEvent( QCloseEvent *e ){

  m_settingData->activatedMainWindow=false; //information about state of mainWindow
//  e->accept();
  KXmlGuiWindow::closeEvent(e);
  }



void KNutMainWindow::slotConfigureKeys() {

  KShortcutsDialog::configure( actionCollection(),KShortcutsEditor::LetterShortcutsDisallowed, this );
  }


void KNutMainWindow::slotCloseKNutClient (void) {
      emit endApplication();
  }




  void KNutMainWindow::slotChangeUPS (const QString &name) {
  emit UPSChanged (name);
  }


void KNutMainWindow::slotPreferencesUps() {
    emit preferenceUps();
  }


/*********************************************************************************************/
/*                                                                                           */
/*                         PRIVATE FUNCTIONS                                                 */
/*                                                                                           */
/*********************************************************************************************/



void KNutMainWindow::initStatusBar ( void ) {
  // sets main message if statusbar
  // nastavime hlavni hlaseni v statusbaru

 // QString progressString("%v ");// += descrOfConnection;
//  progressString.append(descrOfConnection);

  m_progressFormatString.append("%v ");
  m_progressFormatString.append(descrOfConnection);
  m_infoCombo = new KComboBox (statusBar());
  m_infoCombo->setMaxCount(knc::maxCountKNutEvent+1); // nastavime maximalni pocet polozek v Combu
                                                 // pro rezervu pridame jeste jednu
//sets maximal if items in Combo 
  statusBar()->addWidget(m_infoCombo,10);

  m_connectProggresBar = new QProgressBar(statusBar());
  statusBar()->addWidget(m_connectProggresBar,5);
  m_connectProggresBar->setValue(0);
//  m_connectProggresBar->setFormat("%v test of conection from 5");
  m_connectProggresBar->setFormat(m_progressFormatString);
  m_connectProggresBar->hide();
  }


void KNutMainWindow::initStatusBarMessage ( void ) {
  // loads data into comboBox
  QStringList::iterator it;

  m_infoCombo->insertItems(m_infoCombo->count(),*m_listKNutEvent);
  statusBar()-> setItemAlignment(0,Qt::AlignHCenter|Qt::AlignVCenter);
  m_infoCombo->setCurrentIndex(m_infoCombo->count()-1);
  }


void KNutMainWindow::initToolBarAction ( void ) {

  QLabel *upsNazev = new QLabel ("UPS : ",this);
  m_upsComboBox = new KComboBox(this);

  KAction *upsToolBarLabel = new KAction(i18n("UPS Label"),this);
  upsToolBarLabel->setDefaultWidget(upsNazev);
  actionCollection()->addAction("upslabel",upsToolBarLabel);

  KAction *upsToolBarComboBox = new KAction(i18n("UPS ComboBox"),this);
  upsToolBarComboBox->setDefaultWidget(m_upsComboBox);
  actionCollection()->addAction("upscombobox",upsToolBarComboBox);
  connect (m_upsComboBox,SIGNAL (activated (const QString &)),this,SLOT (slotChangeUPS (const QString &)));
}


void KNutMainWindow::setToolBar ( void ) {

  m_upsComboBox->setEditable(false); 

  m_upsComboBox->clear();
  toolBar("mainToolBar")->setAllowedAreas (Qt::TopToolBarArea);
  //sets toolbarr no movable /  nastavime toolBar nepresouvatelny
  toolBar("mainToolBar")->setMovable(false);
  }


void KNutMainWindow::initAction ( void ) {

  // KAction and KStdAction must be to make before command createGUI, function initToolBar is runed after create GUI
  m_quit = KStandardAction::quit (this, SLOT (slotCloseKNutClient()), actionCollection());
  m_quit->setStatusTip(i18n("Quits the application"));

  m_preferencesUps = KStandardAction::preferences (this, SLOT (slotPreferencesUps()), actionCollection());

  KAction *actPrintUPSVars = new KAction(KIcon("view-list-tree"),i18n("&Showing UPS variables and commands"),this); 
  actionCollection()->addAction("printupsvars",actPrintUPSVars);
  connect (actPrintUPSVars, SIGNAL(triggered(bool)),this,SLOT(slotPrintUPSVars()));

  KAction *actInstComms= new KAction(i18n("&Running instant commands"),this);
  actionCollection()->addAction("instcomms",actInstComms);
  connect ( actInstComms,SIGNAL(triggered(bool)), this,SLOT(slotInstComms()));

  KAction *actRWVars = new KAction(i18n("Setting R&W variables"),this);
  actionCollection()->addAction("rwvars",actRWVars);
  connect (actRWVars, SIGNAL(triggered(bool)),this,SLOT(slotRWVars()));

  setStandardToolBarMenuEnabled(true);

  m_descriptionAction = new KToggleAction(i18n("&Using descriptions"),this);
  actionCollection()->addAction("use_description",m_descriptionAction);
  connect (m_descriptionAction, SIGNAL(triggered(bool)),this,SLOT(slotDescription()));

  KStandardAction::keyBindings( this, SLOT(slotConfigureKeys()),actionCollection());

  KAction *actRefreshConnection = new KAction (KIcon("view-refresh"),i18n("Reconnect"),this);
  actionCollection()->addAction("reconnect",actRefreshConnection);
  connect (actRefreshConnection,SIGNAL(triggered(bool)),this,SLOT(slotRefreshConnection()));
  actRefreshConnection->setShortcut(Qt::Key_F5);
  actRefreshConnection->setStatusTip(i18n("Reconnect the application"));
  }

void KNutMainWindow::initVars ( void ) {

  //m_activeUpsIndex = -1;

  //nastavime fonty
  // sets fonts
  m_mainPalette= kapp->palette(); //saves init system palette
  m_areaPalette=m_mainPalette;

  m_aPanelPalette=m_mainPalette;
  m_mPanelPalette=m_mainPalette;
  }

void KNutMainWindow::setPalettes ( void ) {
  //sets palette for analog panels
  if (m_settingData->customBAnalogColor) {
    m_aPanelPalette.setColor(QPalette::Active,QPalette::Background,m_settingData->aPanelBackgroundColor);
    m_aPanelPalette.setColor(QPalette::Inactive,QPalette::Background,m_settingData->aPanelBackgroundColor);
    }

  //sets palette for front panel
  if (m_settingData->customBPanelColor) {
    m_mPanelPalette.setColor(QPalette::Active,QPalette::Background,m_settingData->mPanelBackgroundColor);
    m_mPanelPalette.setColor(QPalette::Inactive,QPalette::Background,m_settingData->mPanelBackgroundColor);
    }

 if (m_settingData->customBColor) {
    m_areaPalette.setColor(QPalette::Active,QPalette::Background,m_settingData->mainBackgroundColor);
    m_areaPalette.setColor(QPalette::Inactive,QPalette::Background,m_settingData->mainBackgroundColor);
    m_areaPalette.setColor(QPalette::Active,QPalette::Base,m_settingData->mainBackgroundColor);
    m_areaPalette.setColor(QPalette::Inactive,QPalette::Base,m_settingData->mainBackgroundColor);
    m_areaPalette.setColor(QPalette::Active,QPalette::Button,m_settingData->mainBackgroundColor);
    m_areaPalette.setColor(QPalette::Inactive,QPalette::Button,m_settingData->mainBackgroundColor);
    }
}


int KNutMainWindow::activeMainCombo (void) {
  int activeUpsNameIndex = -1; // zadne upsName neni aktivni , any upsName is not active

//  m_upsComboBox = new KComboBox;
//  toolBar("mainToolBar")->addWidget (m_upsComboBox);
//  m_upsComboBox->setVisible(true);


  //naplnime comboBox
  //loads comboBox
  m_upsComboBox->clear();
  for (int i=0; i < m_upsRecords->getCount (); i++) {
    m_upsComboBox->insertItem (i,m_upsRecords->getName(i));
    // ulozime si poradove cislo jmena aktivniho ups
    // save serial number of active usp name
    if ((m_upsRecords->getName(i)) == m_activeUpsInfo->record.name) activeUpsNameIndex = i;
    }
  return activeUpsNameIndex;
  }

void KNutMainWindow::saveConfiguration ( void ) {

  KConfigGroup config = KGlobal::config()->group("MainWindow");

  //saves info about main window
  config.writeEntry ("PosY",y()); //
  config.writeEntry ("PosX",x()); //
  config.sync();
}



#include "knutmainwindow.moc"
