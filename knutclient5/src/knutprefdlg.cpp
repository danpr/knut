/***************************************************************************
                          knutprefdlg.cpp  -  description
                             -------------------
    begin                : Tue Aug 21 2001
    copyright            : (C) 2001 by Daniel Prynych
    email                : Daniel.Prynych@alo.cz
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "knutprefdlg.h"
#include "knutvardata.h"
#include "knutfrontpanel.h"
#include "knutdock.h"


#include <KIconLoader>
#include <KLocalizedString>
#include <KHelpClient>

#include <QIcon>
#include <QLayout>
#include <QGroupBox>
#include <QRadioButton>
#include <QFont>
#include <QLabel>
#include <QCheckBox>
#include <QSpinBox>
#include <QStringList>
#include <QDialogButtonBox>
#include <QPushButton>

//#include <stdio.h>


KNutPrefDlg::KNutPrefDlg(KNutUpsData* const upsData, KNutVarData* const analogData, KNutVarData::SettingDataDef* const settingData,
                          const QString upsName, QWidget* parent)
            :KPageDialog( parent ), m_activeUpsName (upsName), m_myUpsData(upsData), m_myAnalogData(analogData), m_settingData(settingData) {

  setAttribute(Qt::WA_DeleteOnClose,false);
  setStandardButtons (QDialogButtonBox::RestoreDefaults | QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Help  | QDialogButtonBox::Apply ); 
  setModal(true);
  setFaceType(List);
  setWindowTitle (i18n("Preferences"));

  m_checked = false; // no change of definition of UPS // neni zadna zmena na definici UPS
  m_checkedActiveUps = false; // change of active UPS hasn't been made // neni provedena zmena aktivni UPS

  mainSettingPage =0;
  upsSettingPage = 0;
  panelSettingPage = 0;
  fontSettingPage = 0;
  analogSettingPage = 0;
  dockSettingPage = 0 ;

  initSetting();
  initUps();
  initPanel();
  initFonts();
  initAnalog();
  initDock(); // from 0.8 version
  
  buttonBox()->button(QDialogButtonBox::Apply)->setEnabled(true);

  
  QDialogButtonBox* myButtonBox = buttonBox();

  connect (myButtonBox,SIGNAL(clicked(QAbstractButton *)),this,SLOT(slotButtonClicked(QAbstractButton *)));

  setPages();
  }


KNutPrefDlg::~KNutPrefDlg(){
  delete m_listWidget2;
  }




/*********************************************************************************************/
/*                                                                                           */
/*                         PRIVATE SLOTS                                                     */
/*                                                                                           */
/*********************************************************************************************/


void KNutPrefDlg::slotSetPictureKI (bool on) {
if (!on) {
  m_checkBoxCbi->setEnabled(true);
  m_iconDockLabel->setEnabled(true);
  m_BGIButton->setEnabled(m_checkBoxCbi->isChecked());
  m_dockBackgroundColorLabel->setEnabled(m_checkBoxCbi->isChecked());
  }
else {
  m_checkBoxCbi->setEnabled(false);
  m_iconDockLabel->setEnabled(false);
  m_BGIButton->setEnabled(false);
  m_dockBackgroundColorLabel->setEnabled(false);
  }
}


void KNutPrefDlg::slotEnableVoltageGroup(bool enable) {
  m_voltageGroup->setEnabled(!enable);
  }

void KNutPrefDlg::slotEnableFrequencyGroup(bool enable) {
  m_frequencyGroup->setEnabled(!enable);
  }

void KNutPrefDlg::slotUseCustomBColor(bool enable) {
  m_BGButton->setEnabled(enable);
  m_mainBackgroundColorLabel->setEnabled(enable);
  }

void KNutPrefDlg::slotUseCustomBIconColor(bool enable) {
  m_BGIButton->setEnabled(enable);
  m_dockBackgroundColorLabel->setEnabled(enable);
  }


void KNutPrefDlg::slotUseCustomBPanelColor(bool enable) {
  m_BGPButton->setEnabled(enable);
  m_panelBackgroundColorLabel->setEnabled(enable);
  }

void KNutPrefDlg::slotUseCustomBAnalogColor(bool enable) {
  m_BGAButton->setEnabled(enable);
  m_analogBackgroundColorLabel->setEnabled(enable);
  }

void KNutPrefDlg::slotUseCustomOAnalogColor(bool enable) {
  m_AFingerButton->setEnabled(enable);
  m_AOKButton->setEnabled(enable);
  m_AWarnningButton->setEnabled(enable);
  m_AErrorButton->setEnabled(enable);
  m_AScaleButton->setEnabled(enable);
  m_AFontButton->setEnabled(enable);
  m_analogPointerColorLabel->setEnabled(enable);
  m_analogOKColorLabel->setEnabled(enable);
  m_analogWarningColorLabel->setEnabled(enable);
  m_analogErrorColorLabel->setEnabled(enable);
  m_analogScaleColorLabel->setEnabled(enable);
  m_analogFontColorLabel->setEnabled(enable);
  }



void KNutPrefDlg::slotChangeSettingFont(int index) {
  if (m_settingFont != index) {
  // vybirame jiny font
  if (m_settingFont == mainPanelFont) {
     m_mPanelFont=m_fontWidget->font();
     m_fontWidget->setFont(m_aPanelFont);
    }
  else {
    m_aPanelFont=m_fontWidget->font();
    m_fontWidget->setFont(m_mPanelFont);
    }
   m_settingFont = index;
  }
}



void KNutPrefDlg::slotDeleteUps() {
  if (m_listWidget2->currentRow() != -1) {
    // smazeme policko
    int ukListWidget = m_listWidget2->currentRow();
    if ( m_activeUpsName == m_listWidget2->currentItem()->text() ) m_checkedActiveUps = true;
    m_myUpsData->deleteName(ukListWidget);
    m_listWidget2->takeItem(ukListWidget);
    m_checked = true;
    // when active ups is deleted, will set checkedActiveUps 
    // pokud smazeme aktivni UPS nastavime checkedActiveUps
    }
  }


void KNutPrefDlg::slotEditUps() {
  if (m_listWidget2->currentRow() != -1)	{
    KNutUpsData::upsRecordDef upsRecord;
    // makes dialog
    newUpsDlg = new KNutNewUpsDlg(m_myUpsData,m_myAnalogData,false);
    // reads record
    m_myUpsData->get(m_listWidget2->currentRow(),upsRecord);
    // sets parameters for edit /  nastavime parametry pro edit
    newUpsDlg->putData(upsRecord);
    int result = newUpsDlg->exec();
    if (result == Accepted) {
      // adds into list after OK will be clicked  
      // reads data from form
      upsRecord = newUpsDlg->getData();
      // corrects data in list
      m_myUpsData->put(m_listWidget2->currentRow(),upsRecord);
      // corrects data in window
      m_listWidget2->currentItem()->setText(upsRecord.name);
      m_checked = true;
      // if active UPS is changed, will set checkedActiveUps
      // pokud zmenime aktivni UPS nastavime checkedActiveUps
      if ( m_activeUpsName == m_listWidget2->currentItem()->text() ) m_checkedActiveUps = true;
      }
    delete newUpsDlg;
    }
  }


void KNutPrefDlg::slotNewUps () {
  KNutUpsData::upsRecordDef upsRecord;
  // Zalozime novou UPS
  // add new UPS
  newUpsDlg = new KNutNewUpsDlg(m_myUpsData,m_myAnalogData);
  int result = newUpsDlg->exec();
  if (result == Accepted) {
    // adds into list, when OK is pressed
    //reads data from form
    upsRecord = newUpsDlg->getData();
    //pridame data do zaznamu
    //adds data to record
    m_myUpsData->add(upsRecord);
    //adds data to window
    // pridame data do okenka
    m_listWidget2->addItem(upsRecord.name);
    // pokud pridame prvni prekreslime hlavni okno
    // when adds the first sameone, will repaint window
    m_checked = true;
    if (m_myUpsData->getCount() ==1 ) m_checkedActiveUps = true;
    }
  delete newUpsDlg;
  }



void KNutPrefDlg::slotButtonClicked(QAbstractButton *button){
 qDebug("slotButtonClicked(QAbstractButton *button)");
 if (buttonBox()->buttonRole(button)  ==  QDialogButtonBox::ApplyRole) {  //Accept
   makeApply();
   }

 if (buttonBox()->buttonRole(button) ==  QDialogButtonBox::ResetRole) {  //Defalt
   makeDefault();
   }


 if (buttonBox()->buttonRole(button)  ==  QDialogButtonBox::AcceptRole) {   //OK
   makeApply();
   accept();
   }


 if (buttonBox()->buttonRole(button)  ==  QDialogButtonBox::HelpRole) {   //OK
   KHelpClient::invokeHelp("knc-configuration","knutclient");
   }

 }


void KNutPrefDlg::makeApply() {
// budeme emitovat signal podle toho co se deje
// will emit signal about situation
  unsigned int changeSetting = 0;

 // MainPage
  if (m_settingData->countCols != m_numberOfColumns->value()) {
    changeSetting += ChangeCountCols;  // nastavime pocet sloupcu
    m_settingData->countCols = m_numberOfColumns->value();  // nastavime pocet sloupcu
    }

  if (m_settingData->customBColor != m_checkBoxCbc->isChecked()) {
    m_settingData->customBColor = m_checkBoxCbc->isChecked();
    changeSetting += ChangeCustomBColor;
    }

  if (m_settingData->mainBackgroundColor != m_BGButton->color()) {
    changeSetting += ChangeMBColor;
    m_settingData->mainBackgroundColor = m_BGButton->color();  // nastavime barvu pozadi
    }

  // ovlivnuje jen ukonceni programu, neurcujeme type ChangeSetting
  if (m_settingData->areYouSure != m_checkBoxAys->isChecked()) {
    m_settingData->areYouSure = m_checkBoxAys->isChecked();
    changeSetting = changeSetting | ChangeSomeThing;
    }

  //for next start only
  if (m_comboBoxMainWindow->currentIndex() != (int)m_settingData->useMainWindow) {
    m_settingData->useMainWindow = (knc::mainWindowStartVariant)m_comboBoxMainWindow->currentIndex();
    changeSetting = changeSetting | ChangeSomeThing;
   }

//  if (m_prefData->useMainWindow != m_checkBoxMW->isChecked()) {
//    m_prefData->useMainWindow = m_checkBoxMW->isChecked();
//    changeSetting = changeSetting | ChangeSomeThing;
//    }
  //for next start only
  if (m_settingData->useMessageWindow != m_checkBoxMesW->isChecked()) {
    m_settingData->useMessageWindow = m_checkBoxMesW->isChecked();
    changeSetting = changeSetting | ChangeSomeThing;
    }

  //for next start only
  if (m_settingData->useMessageWindow != m_checkBoxMesW->isChecked()) {
    m_settingData->useMessageWindow = m_checkBoxMesW->isChecked();
    changeSetting = changeSetting | ChangeSomeThing;
    }


  int newInputVoltage, newInputFrequency;
  if ( m_vRadioButton1->isChecked()) newInputVoltage = 230; else newInputVoltage = 120;
  if ( m_fRadioButton1->isChecked()) newInputFrequency = 50; else newInputFrequency = 60;

  if (m_settingData->inputVoltage != newInputVoltage) {
    m_settingData->inputVoltage = newInputVoltage;
    changeSetting += ChangeVoltage;
    }

  if (m_settingData->inputFrequency != newInputFrequency) {
    m_settingData->inputFrequency = newInputFrequency;
    changeSetting += ChangeFrequency;
    }

 if (m_settingData->lowHighXfer != m_xfer->isChecked()) changeSetting += ChangeXFer;
   m_settingData->lowHighXfer = m_xfer->isChecked();

    //Ups page
  if ( m_checkedActiveUps ) { // zmena aktivni UPS
    //change of active UPS or add the first UPS, because the first UPS is active always
    changeSetting += ChangeUpsSetting;
    }
  else {
    if (m_checked) changeSetting += ChangeUpsCount;
    }
  // panel Page
  unsigned int prefPanelFlags = 3;
  if (m_checkBoxOver->isChecked()) prefPanelFlags += KNutFrontPanel::PBOver;
  if (m_checkBoxBL->isChecked()) prefPanelFlags += KNutFrontPanel::PBBattLow;
  if (m_checkBoxRB->isChecked()) prefPanelFlags += KNutFrontPanel::PBRepBatt;
  if (m_checkBoxCal->isChecked()) prefPanelFlags += KNutFrontPanel::PBCal;
  if (m_checkBoxMFRM->isChecked()) prefPanelFlags += KNutFrontPanel::PBMFRMod;
  if (m_checkBoxMFR->isChecked()) prefPanelFlags += KNutFrontPanel::PBMFR;
  if (m_checkBoxModel->isChecked()) prefPanelFlags += KNutFrontPanel::PBModel;
  if (m_checkBoxSerial->isChecked()) prefPanelFlags += KNutFrontPanel::PBSerial;
  if (m_checkBoxFirm->isChecked()) prefPanelFlags += KNutFrontPanel::PBFirm;
  if (m_checkBoxRun->isChecked()) prefPanelFlags += KNutFrontPanel::PBRun;

  if (m_settingData->panelFlags != prefPanelFlags) {
    m_settingData->panelFlags = prefPanelFlags;
    changeSetting += ChangeMainPanelSetting;
    }

  if (m_settingData->customBPanelColor != m_checkBoxCbpc->isChecked()) {
    m_settingData->customBPanelColor = m_checkBoxCbpc->isChecked();
    changeSetting += ChangeCustomMPanelBColor;
    }

  if (m_settingData->mPanelBackgroundColor != m_BGPButton->color()) {
    changeSetting += ChangeMPBColor;
    m_settingData->mPanelBackgroundColor = m_BGPButton->color();  // sets color of background
    }


    // font Page:
    /*
    unsigned int changeFonts;
    changeFonts =0;
    dan
    */

    if (m_settingFont == mainPanelFont)
      m_mPanelFont=m_fontWidget->font();
    else
      m_aPanelFont=m_fontWidget->font();

    if ( m_customFont->isChecked() != m_settingData->customFont) {
      // zmena typy fontu systemove <-> uzivatelske
       m_settingData->customFont = m_customFont->isChecked();
       if (m_settingData->customFont) {
         // prejdeme na uzivatelske fonty
         m_settingData->aPanelFont = m_aPanelFont;
         m_settingData->mPanelFont = m_mPanelFont;
         changeSetting += ChangeAnalogFont;
         changeSetting += ChangePanelFont;
         }
       else {
         // pouzijeme systemove fonty
         changeSetting += ChangeAnalogFont;
         changeSetting += ChangePanelFont;
         }
      }
    else {
      if ( m_settingData->customFont ) {
        // otestujeme zda se zmenili nase fonty
        if (m_aPanelFont != m_settingData->aPanelFont) {
          m_settingData->aPanelFont = m_aPanelFont;
          changeSetting += ChangeAnalogFont;
          }
        if (m_mPanelFont != m_settingData->mPanelFont) {
          m_settingData->mPanelFont = m_mPanelFont;
          changeSetting += ChangePanelFont;
          }
        }
      }
  // analog page

  if (m_settingData->customBAnalogColor != m_checkBoxCbac->isChecked()) {
    m_settingData->customBAnalogColor = m_checkBoxCbac->isChecked();
    changeSetting += ChangeCustomAPanelBColor;
    }

  if (m_settingData->aPanelBackgroundColor != m_BGAButton->color()) {
    changeSetting += ChangeAPBColor;
    m_settingData->aPanelBackgroundColor = m_BGAButton->color();  // nastavime barvu pozadi
    }

  if (m_settingData->customOAnalogColor != m_checkBoxCoac->isChecked()) {
    m_settingData->customOAnalogColor = m_checkBoxCoac->isChecked();
    changeSetting += ChangeCustomAPanelOColor;
    }

  if ((m_settingData->analogFingerColor != m_AFingerButton->color()) ||
      (m_settingData->analogOKColor != m_AOKButton->color()) ||
      (m_settingData->analogWarnningColor != m_AWarnningButton->color()) ||
      (m_settingData->analogErrorColor != m_AErrorButton->color()) ||
      (m_settingData->analogScaleColor != m_AScaleButton->color()) ||
      (m_settingData->analogFontColor != m_AFontButton->color())) {
    changeSetting += ChangeAPOColor;
    m_settingData->analogFingerColor = m_AFingerButton->color();  // sets color of pointers-- nastavime barvu rucicky
    m_settingData->analogOKColor = m_AOKButton->color();  // sets color of OK --  nastavime barvu ok
    m_settingData->analogWarnningColor = m_AWarnningButton->color();  // nastavime barvu varovano
    m_settingData->analogErrorColor = m_AErrorButton->color();  // nastavime barvu chyby
    m_settingData->analogScaleColor = m_AScaleButton->color();  // nastavime barvu stupnice
    m_settingData->analogFontColor = m_AFontButton->color();  // nastavime barvu textu
    }

  if (m_settingData->typeDigitalProcessing != m_digitalProcesingOfPointers->currentIndex()) {
    m_settingData->typeDigitalProcessing = m_digitalProcesingOfPointers->currentIndex();
    changeSetting += ChangeTypeOfDigProcessing;
    }

  // Kicker /SysTray/ Icon page

  int stateKIcon;
  if (m_iconRadioButton1->isChecked()) stateKIcon = KNutDock::pictureKI;
  else stateKIcon = KNutDock::generalKI;
  if (m_settingData->typeOfKIcon != stateKIcon) {
    if (m_iconRadioButton1->isChecked()) m_settingData->typeOfKIcon = KNutDock::pictureKI;
    if (m_iconRadioButton2->isChecked()) m_settingData->typeOfKIcon = KNutDock::generalKI;
    changeSetting += ChangeTypeOfKIcon;
    }

  if (m_settingData->customKIconColor != m_checkBoxCbi->isChecked()) {
    m_settingData->customKIconColor = m_checkBoxCbi->isChecked();
    changeSetting += ChangeCustomKIconColor;
    }

  if (m_settingData->kIconBackgroundColor != m_BGIButton->color()) {
    changeSetting += ChangeKIconBColor;
    m_settingData->kIconBackgroundColor = m_BGIButton->color();  // nastavime barvu pozadi
    }

  unsigned int prefToolTipFlags = 0;
  if (m_checkBoxTTMFR->isChecked()) prefToolTipFlags += KNutDock::TTMFR;
  if (m_checkBoxTTModel->isChecked()) prefToolTipFlags += KNutDock::TTModel;
  if (m_checkBoxTTSerial->isChecked()) prefToolTipFlags += KNutDock::TTSerial;
  if (m_checkBoxTTFirm->isChecked()) prefToolTipFlags += KNutDock::TTFirm;
  if (m_checkBoxTTRun->isChecked()) prefToolTipFlags += KNutDock::TTRun;
  if (m_checkBoxTTCharge->isChecked()) prefToolTipFlags += KNutDock::TTCharge;
  if (m_checkBoxTTLoad->isChecked()) prefToolTipFlags += KNutDock::TTLoad;

  if (m_settingData->toolTipDockFlags != prefToolTipFlags) {
    m_settingData->toolTipDockFlags = prefToolTipFlags;
    changeSetting += ChangeToolTip;
    }

  if (changeSetting)  emit signalChangeConfig( changeSetting );
  }


void KNutPrefDlg::makeDefault() {
  qDebug ("KNutPrefDlg::makeDefault()");
  setDefaultPage (currentPage());
  }


/*********************************************************************************************/
/*                                                                                           */
/*                         PRIVATE FUNCTIONS                                                 */
/*                                                                                           */
/*********************************************************************************************/


void KNutPrefDlg::setPages( void ) {
//mainPage
  m_numberOfColumns->setValue(m_settingData->countCols);
  m_BGButton->setColor(m_settingData->mainBackgroundColor);  // sets color of background
  m_checkBoxAys->setChecked(m_settingData->areYouSure);
  m_checkBoxMesW->setChecked(m_settingData->useMessageWindow);
  m_comboBoxMainWindow->setCurrentIndex(m_settingData->useMainWindow);

  if (m_settingData->inputVoltage == 230)
    m_vRadioButton1->setChecked( true );
  else
    m_vRadioButton2->setChecked( true );
  if (m_settingData->inputFrequency == 50)
    m_fRadioButton1->setChecked( true );
  else
   m_fRadioButton2->setChecked( true );

  m_xfer->setChecked(m_settingData->lowHighXfer);
  m_voltageGroup->setEnabled(!m_settingData->lowHighXfer);
  m_frequencyGroup->setEnabled(!m_settingData->lowHighXfer);
  m_checkBoxCbc->setChecked(m_settingData->customBColor); 
  m_BGButton->setColor(m_settingData->mainBackgroundColor);
  m_BGButton->setEnabled(m_settingData->customBColor);
  m_mainBackgroundColorLabel->setEnabled(m_settingData->customBColor);

//upsPage
  // sets listbox
  for (int i=0; i < m_myUpsData->getCount (); i++) m_listWidget2->addItem (m_myUpsData->getName(i));

//panelPage
  m_checkBoxOver->setChecked((m_settingData->panelFlags) & KNutFrontPanel::PBOver);
  m_checkBoxBL->setChecked((m_settingData->panelFlags) & KNutFrontPanel::PBBattLow);
  m_checkBoxRB->setChecked((m_settingData->panelFlags) & KNutFrontPanel::PBRepBatt);
  m_checkBoxCal->setChecked((m_settingData->panelFlags) & KNutFrontPanel::PBCal);
  m_checkBoxMFRM->setChecked((m_settingData->panelFlags) & KNutFrontPanel::PBMFRMod);
  m_checkBoxMFR->setChecked((m_settingData->panelFlags) & KNutFrontPanel::PBMFR);
  m_checkBoxModel->setChecked((m_settingData->panelFlags) & KNutFrontPanel::PBModel);
  m_checkBoxSerial->setChecked((m_settingData->panelFlags) & KNutFrontPanel::PBSerial);
  m_checkBoxFirm->setChecked((m_settingData->panelFlags) & KNutFrontPanel::PBFirm);
  m_checkBoxRun->setChecked((m_settingData->panelFlags) & KNutFrontPanel::PBRun);
  m_checkBoxCbpc->setChecked(m_settingData->customBPanelColor);
  m_BGPButton->setColor(m_settingData->mPanelBackgroundColor);
  m_BGPButton->setEnabled(m_settingData->customBPanelColor);
  m_panelBackgroundColorLabel->setEnabled(m_settingData->customBPanelColor);
  m_panelBackgroundColorLabel->setEnabled(m_settingData->customBPanelColor);


// font page
  m_customFont->setChecked(m_settingData->customFont);
  m_aPanelFont = m_settingData->aPanelFont;
  m_mPanelFont = m_settingData->mPanelFont;
  m_settingFont = mainPanelFont; // sets font for Main Panel /  nastavujeme font pro Main Panel
  m_fontWidget->setFont(m_mPanelFont);
  m_fontWidget->setEnabled(m_settingData->customFont);

// analog page
  m_checkBoxCbac->setChecked(m_settingData->customBAnalogColor);
  m_checkBoxCoac->setChecked(m_settingData->customOAnalogColor);
  m_BGAButton->setColor(m_settingData->aPanelBackgroundColor);
  m_BGAButton->setEnabled(m_settingData->customBAnalogColor);
  m_AFingerButton->setColor(m_settingData->analogFingerColor);
  m_AFingerButton->setEnabled(m_settingData->customOAnalogColor);
  m_AOKButton->setColor(m_settingData->analogOKColor);
  m_AOKButton->setEnabled(m_settingData->customOAnalogColor);
  m_AWarnningButton->setColor(m_settingData->analogWarnningColor);
  m_AWarnningButton->setEnabled(m_settingData->customOAnalogColor);
  m_AErrorButton->setColor(m_settingData->analogErrorColor);
  m_AErrorButton->setEnabled(m_settingData->customOAnalogColor);
  m_AScaleButton->setColor(m_settingData->analogScaleColor);
  m_AScaleButton->setEnabled(m_settingData->customOAnalogColor);
  m_AFontButton->setColor(m_settingData->analogFontColor);
  m_AFontButton->setEnabled(m_settingData->customOAnalogColor);
  m_analogBackgroundColorLabel->setEnabled(m_settingData->customBAnalogColor);
  m_analogPointerColorLabel->setEnabled(m_settingData->customOAnalogColor);
  m_analogOKColorLabel->setEnabled(m_settingData->customOAnalogColor);
  m_analogWarningColorLabel->setEnabled(m_settingData->customOAnalogColor);
  m_analogErrorColorLabel->setEnabled(m_settingData->customOAnalogColor);
  m_analogScaleColorLabel->setEnabled(m_settingData->customOAnalogColor);
  m_analogFontColorLabel->setEnabled(m_settingData->customOAnalogColor);
  m_digitalProcesingOfPointers->setCurrentIndex(m_settingData->typeDigitalProcessing);

// dock page
  m_checkBoxTTMFR->setChecked((m_settingData->toolTipDockFlags) & KNutDock::TTMFR);
  m_checkBoxTTModel->setChecked((m_settingData->toolTipDockFlags) & KNutDock::TTModel);
  m_checkBoxTTSerial->setChecked((m_settingData->toolTipDockFlags) & KNutDock::TTSerial);
  m_checkBoxTTFirm->setChecked((m_settingData->toolTipDockFlags) & KNutDock::TTFirm);
  m_checkBoxTTRun->setChecked((m_settingData->toolTipDockFlags) & KNutDock::TTRun);
  m_checkBoxTTCharge->setChecked((m_settingData->toolTipDockFlags) & KNutDock::TTCharge);
  m_checkBoxTTLoad->setChecked((m_settingData->toolTipDockFlags) & KNutDock::TTLoad);

  m_checkBoxCbi->setChecked(m_settingData->customKIconColor);
  m_BGIButton->setColor(m_settingData->kIconBackgroundColor);

  if (m_settingData->typeOfKIcon == KNutDock::pictureKI) {
    m_iconRadioButton1->setChecked( true );
    m_iconDockLabel->setEnabled(false);
    m_checkBoxCbi->setEnabled(false);
    m_BGIButton->setEnabled(false);
    m_dockBackgroundColorLabel->setEnabled(false);
    }
  if (m_settingData->typeOfKIcon == KNutDock::generalKI) {
    m_iconRadioButton2->setChecked( true );
    m_iconDockLabel->setEnabled(true);
    m_BGIButton->setEnabled(true);
    m_BGIButton->setEnabled(m_settingData->customKIconColor);
    m_dockBackgroundColorLabel->setEnabled(m_settingData->customKIconColor);
    }

}



void KNutPrefDlg::setDefaultPage( KPageWidgetItem *page) {

qDebug ("KNutPrefDlg::setDefaultPage()");
  if (page ==  mainSettingPage) {
qDebug ("KNutPrefDlg::makeDefault() -- mainSettingPage");
    m_numberOfColumns->setValue( knc::DefaultCountCols );
    m_checkBoxCbc->setChecked(false);
     m_mainBackgroundColorLabel->setEnabled(false);
    m_BGButton->setEnabled(false);
    m_BGButton->setColor(knc::defaultBGColor);
    m_checkBoxAys->setChecked ( false );
    m_checkBoxMesW->setChecked ( false );
    m_comboBoxMainWindow->setCurrentIndex(knc::mw_no); // sets NO
    m_vRadioButton1->setChecked( true );
    m_fRadioButton1->setChecked( true );
    m_xfer->setChecked( false );
    m_voltageGroup->setEnabled(true);
    m_frequencyGroup->setEnabled(true);
    
    }
  if (page == panelSettingPage) {
qDebug ("KNutPrefDlg::makeDefault() -- panelSettingPage");
    m_checkBoxOver->setChecked(KNutFrontPanel::DefaultPanelFlags & KNutFrontPanel::PBOver);
    m_checkBoxBL->setChecked(KNutFrontPanel::DefaultPanelFlags & KNutFrontPanel::PBBattLow);
    m_checkBoxRB->setChecked(KNutFrontPanel::DefaultPanelFlags & KNutFrontPanel::PBRepBatt);
    m_checkBoxCal->setChecked(KNutFrontPanel::DefaultPanelFlags & KNutFrontPanel::PBCal);
    m_checkBoxMFRM->setChecked(KNutFrontPanel::DefaultPanelFlags & KNutFrontPanel::PBMFRMod);
    m_checkBoxMFR->setChecked(KNutFrontPanel::DefaultPanelFlags & KNutFrontPanel::PBMFR);
    m_checkBoxModel->setChecked(KNutFrontPanel::DefaultPanelFlags & KNutFrontPanel::PBModel);
    m_checkBoxSerial->setChecked(KNutFrontPanel::DefaultPanelFlags & KNutFrontPanel::PBSerial);
    m_checkBoxFirm->setChecked(KNutFrontPanel::DefaultPanelFlags & KNutFrontPanel::PBFirm);
    m_checkBoxCbpc->setChecked(false);
    m_panelBackgroundColorLabel->setEnabled(false);
    m_BGPButton->setColor(knc::defaultBGColor);
    m_BGPButton->setEnabled(false);
    }
  if (page == fontSettingPage) {
qDebug ("KNutPrefDlg::makeDefault() -- fontSettingPage");
    m_customFont->setChecked(false);
    m_panelFonts->setEnabled(false);
    m_fontWidget->setEnabled(false);
    }
  if (page == analogSettingPage) {
qDebug ("KNutPrefDlg::makeDefault() -- analogSettingPage");
    m_checkBoxCbac->setChecked(false);
    m_BGAButton->setColor(knc::defaultBGColor);
    m_BGAButton->setEnabled(false);
    m_checkBoxCoac->setChecked(false);
    m_AFingerButton->setColor(knc::defaultFingerColor);
    m_AFingerButton->setEnabled(false);
    m_AOKButton->setColor(knc::defaultOkColor);
    m_AOKButton->setEnabled(false);
    m_AWarnningButton->setColor(knc::defaultWarnningColor);
    m_AWarnningButton->setEnabled(false);
    m_AErrorButton->setColor(knc::defaultErrorColor);
    m_AErrorButton->setEnabled(false);
    m_AScaleButton->setColor(knc::defaultScaleColor);
    m_AScaleButton->setEnabled(false);
    m_AFontButton->setColor(knc::defaultFontColor);
    m_AFontButton->setEnabled(false);

    m_analogBackgroundColorLabel->setEnabled(false);
    m_analogPointerColorLabel->setEnabled(false);
    m_analogOKColorLabel->setEnabled(false);
    m_analogWarningColorLabel->setEnabled(false);
    m_analogErrorColorLabel->setEnabled(false);
    m_analogScaleColorLabel->setEnabled(false);
    m_analogFontColorLabel->setEnabled(false);

    m_digitalProcesingOfPointers->setCurrentIndex(0);
    } 
  if (page == dockSettingPage) {
qDebug ("KNutPrefDlg::makeDefault() -- dockSettingPage");
    m_checkBoxCbi->setChecked(false);
    m_BGAButton->setColor(knc::defaultKIconColor);
    m_checkBoxTTMFR->setChecked(KNutDock::DefaultToolTipFlags & KNutDock::TTMFR);
    m_checkBoxTTModel->setChecked(KNutDock::DefaultToolTipFlags & KNutDock::TTModel);
    m_checkBoxTTSerial->setChecked(KNutDock::DefaultToolTipFlags & KNutDock::TTSerial);
    m_checkBoxTTFirm->setChecked(KNutDock::DefaultToolTipFlags & KNutDock::TTFirm);
    m_checkBoxTTRun->setChecked(KNutDock::DefaultToolTipFlags & KNutDock::TTRun);
    m_checkBoxTTCharge->setChecked(KNutDock::DefaultToolTipFlags & KNutDock::TTCharge);
    m_checkBoxTTLoad->setChecked(KNutDock::DefaultToolTipFlags & KNutDock::TTLoad);
    m_iconRadioButton1->setChecked( true );
    }
  }


void KNutPrefDlg::initSetting () {

//KStandardDirs *mdirs;

//mdirs = kapp->sessionConfig()->componentData().dirs();

//QString dirStr=mdirs->findResource("appicon","knc_mset.png");
//qDebug(dirStr.toAscii().constData());

//QStringList mstrl = mdirs->resourceDirs("appicon");

//    QStringListIterator javaStyleIterator(mstrl);
//     while (javaStyleIterator.hasNext())
//          std::cout << javaStyleIterator.next().toLocal8Bit().constData() << std::endl;

//  KIconLoader *loader = KIconLoader::global();
//  m_mSetPix = loader->loadIcon("knc_mset.png", KIconLoader::User);

  QFrame *mainPageWidget = new QFrame;
  mainSettingPage = new KPageWidgetItem (mainPageWidget,i18n("Setting"));
  mainSettingPage->setHeader (i18n("Main Setting"));
  mainSettingPage->setIcon(QIcon(UserIcon("knc_mset.png")));
  addPage(mainSettingPage);

  QVBoxLayout *topLayout = new QVBoxLayout( mainPageWidget );

  QGridLayout *dataLayout = new QGridLayout();

  QLabel *initSettingLabel1 = new QLabel(i18n("Number of columns :"),mainPageWidget);
  m_numberOfColumns = new QSpinBox(mainPageWidget);
  m_numberOfColumns->setMinimum(knc::MinCols);
  m_numberOfColumns->setMaximum(knc::MaxCols);
  QLabel *initSettingLabel2 = new QLabel(i18n("Use custom background color"),mainPageWidget);
  m_checkBoxCbc = new QCheckBox ("",mainPageWidget);
  m_mainBackgroundColorLabel = new QLabel(i18n("Color of background :"),mainPageWidget);
  m_BGButton = new KColorButton (mainPageWidget);
  QLabel *initSettingLabel4 = new QLabel(i18n("Use dialog \"Are you sure\""),mainPageWidget);
  m_checkBoxAys = new QCheckBox ("",mainPageWidget);
  QLabel *initSettingLabel5 = new QLabel(i18n("Use Main window when program is started"),mainPageWidget);
  m_comboBoxMainWindow = new KComboBox (mainPageWidget);
  m_comboBoxMainWindow->addItem (i18n("Yes"));
  m_comboBoxMainWindow->addItem (i18n("No"));
  m_comboBoxMainWindow->addItem (i18n("Auto"));

  QLabel *initSettingLabel6 = new QLabel(i18n("Show message window, when program reports error"),mainPageWidget);
  m_checkBoxMesW = new QCheckBox ("",mainPageWidget);

  dataLayout->addWidget (initSettingLabel1,0,0);
  dataLayout->addWidget (m_numberOfColumns,0,1);
  dataLayout->addWidget (initSettingLabel2,1,0);
  dataLayout->addWidget (m_checkBoxCbc,1,1);
  dataLayout->addWidget (m_mainBackgroundColorLabel,2,0);
  dataLayout->addWidget (m_BGButton,2,1);
  dataLayout->addWidget (initSettingLabel4,3,0);
  dataLayout->addWidget (m_checkBoxAys,3,1);
  dataLayout->addWidget (initSettingLabel5,4,0);
  dataLayout->addWidget (m_comboBoxMainWindow,4,1);
  dataLayout->addWidget (initSettingLabel6,5,0);
  dataLayout->addWidget (m_checkBoxMesW,5,1);
  topLayout->addLayout(dataLayout);

  QGroupBox *VFGroup = new QGroupBox(i18n("Voltage/Frequency"),mainPageWidget);
  QGridLayout *setVoltFreqLayout = new QGridLayout (VFGroup );

  m_voltageGroup = new QGroupBox(i18n("Nominal Input Voltage"),VFGroup);
  QGridLayout *voltLayout = new QGridLayout (m_voltageGroup );
  m_vRadioButton1 = new QRadioButton(i18n("&230 V"), m_voltageGroup );
  m_vRadioButton2 = new QRadioButton(i18n("&120 V"), m_voltageGroup );
  voltLayout->addWidget ( m_vRadioButton1,1,0 );
  voltLayout->addWidget ( m_vRadioButton2,2,0 );

  m_frequencyGroup = new QGroupBox(i18n("Nominal Input Frequency"),VFGroup);
  QGridLayout *freqLayout = new QGridLayout (m_frequencyGroup );
  m_fRadioButton1 = new QRadioButton(i18n("&50 Hz"), m_frequencyGroup);
  m_fRadioButton2 = new QRadioButton(i18n("&60 Hz"), m_frequencyGroup);
  freqLayout->addWidget ( m_fRadioButton1,1,0 );
  freqLayout->addWidget ( m_fRadioButton2,2,0 );

  m_xfer = new QCheckBox (i18n("&Use High-Low input.transfer values"),VFGroup);
  setVoltFreqLayout->addWidget ( m_voltageGroup,1,0 );
  setVoltFreqLayout->addWidget ( m_frequencyGroup,1,1 );
  setVoltFreqLayout->addWidget ( m_xfer,2,0 );

  topLayout->addWidget (VFGroup,10);
  topLayout->addStretch( 20 );

  connect (m_xfer,SIGNAL(toggled(bool)),this,SLOT( slotEnableVoltageGroup(bool)));
  connect (m_xfer,SIGNAL(toggled(bool)),this,SLOT( slotEnableFrequencyGroup(bool)));
  connect (m_checkBoxCbc,SIGNAL(toggled(bool)),this,SLOT(slotUseCustomBColor(bool)));

  setDefaultPage(mainSettingPage);
  }

void KNutPrefDlg::initUps () {

  QFrame *mainPageWidget = new QFrame;
  upsSettingPage = new KPageWidgetItem (mainPageWidget,i18n("UPS"));
  upsSettingPage->setHeader (i18n("UPS Setting"));
  upsSettingPage->setIcon(QIcon(UserIcon("knc_upses.png")));
  addPage(upsSettingPage);


  QHBoxLayout *topLayout = new QHBoxLayout( mainPageWidget );

  // makes box for names of UPS's / Vytvorime box pro jmena UPS-ek
  //m_listWidget2 = new KListWidget( mainPageWidget );
  m_listWidget2 = new QListWidget( mainPageWidget );
  m_listWidget2->setSizePolicy( QSizePolicy( (QSizePolicy::Policy)5 ,(QSizePolicy::Policy)5 ));
  m_listWidget2->setFrameShape( QListWidget::StyledPanel );
  m_listWidget2->setFrameShadow( QListWidget::Sunken );
  m_listWidget2->setMinimumSize (100,100);

  //includes listbox into horizontal layout on the first place
  topLayout->addWidget( m_listWidget2, 20 );
  // makes vertical layout and includes it in horizontal layout into the second place 
  //QVBoxLayout *upsLayout = new QVBoxLayout( topLayout );
  QVBoxLayout *upsLayout = new QVBoxLayout();
  topLayout->addLayout(upsLayout);

  // vytvorime tlacitka
  //KPushButton *newButton = new KPushButton (i18n("&Add"),mainPageWidget);
  //KPushButton *editButton = new KPushButton (i18n("&Edit"),mainPageWidget);
  //KPushButton *deleteButton = new KPushButton (i18n("&Delete"),mainPageWidget);

  QPushButton *newButton = new QPushButton (i18n("&Add"),mainPageWidget);
  QPushButton *editButton = new QPushButton (i18n("&Edit"),mainPageWidget);
  QPushButton *deleteButton = new QPushButton (i18n("&Delete"),mainPageWidget);


  // includes buttons into layout /  vlozime tlacitka do layoutu
  upsLayout->addWidget( newButton );
  upsLayout->addWidget( editButton );
  upsLayout->addWidget( deleteButton );

  upsLayout->addStretch( 10 );

  connect (newButton,SIGNAL(clicked()),this,SLOT(slotNewUps()));
  connect (editButton,SIGNAL(clicked()),this,SLOT(slotEditUps()));
  connect (deleteButton,SIGNAL(clicked()),this,SLOT(slotDeleteUps()));
  connect (m_listWidget2,SIGNAL(doubleClicked(QListWidgetItem *,const QPoint &)),this,SLOT(slotEditUps()));
  setDefaultPage(upsSettingPage);
  }

void KNutPrefDlg::initDock () {

  QFrame *mainPageWidget = new QFrame;
  dockSettingPage = new KPageWidgetItem (mainPageWidget,i18n("Dock bar"));
  dockSettingPage->setHeader (i18n("Docking Setting"));
  dockSettingPage->setIcon(QIcon(UserIcon("knc_dock.png")));
  addPage(dockSettingPage);

  QVBoxLayout *topLayout = new QVBoxLayout( mainPageWidget );

  QGroupBox *typeGroup = new QGroupBox(i18n("Type of dock's showing"),mainPageWidget);
  QGridLayout *typeLayout = new QGridLayout (typeGroup);
  topLayout->addWidget(typeGroup);

  QGroupBox *infoBox = new QGroupBox (i18n("Items of tooltip"),mainPageWidget);
  QGridLayout *infoLayout = new QGridLayout (infoBox);
  topLayout->addWidget(infoBox);

  QGridLayout *colorLayout = new QGridLayout ();
  topLayout->addLayout(colorLayout);

  m_iconRadioButton1 = new QRadioButton(i18n("&Picture"), typeGroup );
  m_iconRadioButton2 = new QRadioButton(i18n("&General"), typeGroup );

  m_checkBoxTTMFR = new QCheckBox (i18n("M&anufacturer"),infoBox);
  m_checkBoxTTModel = new QCheckBox (i18n("M&odel"),infoBox);
  m_checkBoxTTSerial = new QCheckBox (i18n("&Serial"),infoBox);
  m_checkBoxTTFirm = new QCheckBox (i18n("&Firm. rev."),infoBox);
  m_checkBoxTTRun = new QCheckBox (i18n("&Runtime"),infoBox);
  m_checkBoxTTCharge = new QCheckBox (i18n("&Battery Charge"),infoBox);
  m_checkBoxTTLoad = new QCheckBox (i18n("&UPS Load"),infoBox);

  m_iconDockLabel = new QLabel(i18n("Use custom icon's background color"),mainPageWidget);
  m_checkBoxCbi = new QCheckBox ("",mainPageWidget);

  m_dockBackgroundColorLabel = new QLabel(i18n("Color of icon's background :"),mainPageWidget);
  m_BGIButton = new KColorButton (mainPageWidget);

//  typeLayout->addRowSpacing (0 ,fontMetrics().lineSpacing() );
  typeLayout->addWidget ( m_iconRadioButton1,1,0 );
  typeLayout->addWidget ( m_iconRadioButton2,2,0 );

//  infoLayout->addRowSpacing (0 ,fontMetrics().lineSpacing() );
  infoLayout->addWidget ( m_checkBoxTTMFR ,1,0);
  infoLayout->addWidget ( m_checkBoxTTModel ,2,0);
  infoLayout->addWidget ( m_checkBoxTTSerial ,3,0);
  infoLayout->addWidget ( m_checkBoxTTFirm ,4,0);
  infoLayout->addWidget ( m_checkBoxTTRun ,5,0);
  infoLayout->addWidget ( m_checkBoxTTCharge ,6,0);
  infoLayout->addWidget ( m_checkBoxTTLoad ,7,0);

  colorLayout->addWidget (m_iconDockLabel,0,0);
  colorLayout->addWidget (m_checkBoxCbi,0,1);
  colorLayout->addWidget (m_dockBackgroundColorLabel,1,0);
  colorLayout->addWidget (m_BGIButton,1,1);

  topLayout->addStretch( 20 );

  connect (m_checkBoxCbi,SIGNAL(toggled(bool)),this,SLOT(slotUseCustomBIconColor(bool)));
  connect (m_iconRadioButton1,SIGNAL(toggled(bool)),this,SLOT(slotSetPictureKI(bool)));

  setDefaultPage(dockSettingPage);
  }


void KNutPrefDlg::initPanel () {

  QFrame *mainPageWidget = new QFrame;
  panelSettingPage = new KPageWidgetItem (mainPageWidget,i18n("Panel"));
  panelSettingPage->setHeader (i18n("Panel Setting"));
  panelSettingPage->setIcon(QIcon(UserIcon("knc_panel.png")));
  addPage(panelSettingPage);

  QVBoxLayout *topLayout = new QVBoxLayout( mainPageWidget );

  QGroupBox *infoBox = new QGroupBox (mainPageWidget);
  QGridLayout *infoLayout = new QGridLayout (infoBox);
  topLayout->addWidget(infoBox);

  QGridLayout *colorLayout = new QGridLayout ();
  topLayout->addLayout(colorLayout);

  // vytvorime widgety - Nazev Zatrhavaci box
  //makes widgets - name checkbox
  m_checkBoxOver = new QCheckBox (i18n("UPS &Overload"),infoBox);
  m_checkBoxBL = new QCheckBox (i18n("UPS &Battery low"),infoBox);
  m_checkBoxRB = new QCheckBox (i18n("R&eplace battery"),infoBox);
  m_checkBoxCal = new QCheckBox (i18n("Ups &calibration"),infoBox);
  m_checkBoxMFRM = new QCheckBox (i18n("&Manufac. + Model"),infoBox);
  m_checkBoxMFR = new QCheckBox (i18n("M&anufacturer"),infoBox);
  m_checkBoxModel = new QCheckBox (i18n("M&odel"),infoBox);
  m_checkBoxSerial = new QCheckBox (i18n("&Serial"),infoBox);
  m_checkBoxFirm = new QCheckBox (i18n("&Firm. rev."),infoBox);
  m_checkBoxRun = new QCheckBox (i18n("&Runtime"),infoBox);


  QLabel *initPanelLabel1 = new QLabel(i18n("Use custom background color"),mainPageWidget);
  m_checkBoxCbpc = new QCheckBox ("",mainPageWidget);
  m_panelBackgroundColorLabel = new QLabel(i18n("Color of background :"),mainPageWidget);
  m_BGPButton = new KColorButton (mainPageWidget);

  // includes widgets
  infoLayout->addWidget ( m_checkBoxOver, 0,0);
  infoLayout->addWidget ( m_checkBoxBL ,1,0);
  infoLayout->addWidget ( m_checkBoxRB ,2,0);
  infoLayout->addWidget ( m_checkBoxCal ,3,0);
  infoLayout->addWidget ( m_checkBoxMFRM ,4,0);
  infoLayout->addWidget ( m_checkBoxMFR ,5,0);
  infoLayout->addWidget ( m_checkBoxModel ,6,0);
  infoLayout->addWidget ( m_checkBoxSerial ,7,0);
  infoLayout->addWidget ( m_checkBoxFirm ,8,0);
  infoLayout->addWidget ( m_checkBoxRun ,9,0);

  colorLayout->addWidget (initPanelLabel1,0,0);
  colorLayout->addWidget (m_checkBoxCbpc,0,1);
  colorLayout->addWidget (m_panelBackgroundColorLabel,1,0);
  colorLayout->addWidget (m_BGPButton,1,1);

  topLayout->addStretch( 20 );

  connect (m_checkBoxCbpc,SIGNAL(toggled(bool)),this,SLOT(slotUseCustomBPanelColor(bool)));
  setDefaultPage(panelSettingPage);
  }


void KNutPrefDlg::initFonts () {

  QFrame *mainPageWidget = new QFrame;
  fontSettingPage = new KPageWidgetItem (mainPageWidget,i18n("Fonts"));
  fontSettingPage->setHeader (i18n("Setting Fonts"));
  fontSettingPage->setIcon(QIcon(DesktopIcon("fontforge")));
  addPage(fontSettingPage);

  QVBoxLayout *topLayout = new QVBoxLayout( mainPageWidget );

  QHBoxLayout *setSysLayout = new QHBoxLayout();
  m_customFont = new QCheckBox (i18n("&Use custom font"),mainPageWidget);
  setSysLayout->addWidget ( m_customFont ,10);
  setSysLayout->addStretch( 10 );
  topLayout->addLayout(setSysLayout);

  QHBoxLayout *setTypeLayout = new QHBoxLayout();
  m_panelFonts = new KComboBox(mainPageWidget);
  m_panelFonts->addItem (i18n("Main panel"));
  m_panelFonts->addItem (i18n("Analog panels"));
  setTypeLayout->addWidget ( m_panelFonts ,0);
  topLayout->addLayout(setTypeLayout);

  QHBoxLayout *setFontLayout = new QHBoxLayout();
  QStringList fontsList;
  KFontChooser::getFontList(fontsList, KFontChooser::SmoothScalableFonts);
  /*
  m_fontWidget = new KFontChooser(mainPageWidget, false, fontsList);
  dan
  */
  m_fontWidget = new KFontChooser(mainPageWidget, KFontChooser::DisplayFrame | KFontChooser::ShowDifferences , fontsList);
  setFontLayout->addWidget (m_fontWidget ,0);
  topLayout->addLayout(setFontLayout);

  topLayout->addStretch( 20 );

  connect (m_panelFonts,SIGNAL(activated(int)),this,SLOT(slotChangeSettingFont(int)));
  connect (m_customFont,SIGNAL(toggled(bool)),m_fontWidget,SLOT(setEnabled(bool)));
  connect (m_customFont,SIGNAL(toggled(bool)),m_panelFonts,SLOT(setEnabled(bool)));

  setDefaultPage(fontSettingPage);
  }

void KNutPrefDlg::initAnalog () {

  QFrame *mainPageWidget = new QFrame;
  analogSettingPage = new KPageWidgetItem (mainPageWidget,i18n("Analog"));
  analogSettingPage->setHeader (i18n("Setting Analog panel"));
  analogSettingPage->setIcon(QIcon(UserIcon("knc_analog.png")));
  addPage(analogSettingPage);

  QVBoxLayout *topLayout = new QVBoxLayout( mainPageWidget );

  QGridLayout *setColorLayout = new QGridLayout();
  topLayout->addLayout(setColorLayout);
  topLayout->addStretch( 20 );

  QLabel *initAnalogLabel1 = new QLabel(i18n("Use custom background color"),mainPageWidget);
  m_checkBoxCbac = new QCheckBox ("",mainPageWidget);
  m_analogBackgroundColorLabel = new QLabel(i18n("Color of background :"),mainPageWidget);
  m_BGAButton = new KColorButton (mainPageWidget);
  QLabel *initAnalogLabel12 = new QLabel(i18n("Use custom other colors"),mainPageWidget);
  m_checkBoxCoac = new QCheckBox ("",mainPageWidget);
  m_analogPointerColorLabel = new QLabel(i18n("Color of pointer :"),mainPageWidget);
  m_AFingerButton = new KColorButton (mainPageWidget);
  m_analogOKColorLabel = new QLabel(i18n("Color of OK range :"),mainPageWidget);
  m_AOKButton = new KColorButton (mainPageWidget);
  m_analogWarningColorLabel = new QLabel(i18n("Color of warning range :"),mainPageWidget);
  m_AWarnningButton = new KColorButton (mainPageWidget);
  m_analogErrorColorLabel= new QLabel(i18n("Color of error range :"),mainPageWidget);
  m_AErrorButton = new KColorButton (mainPageWidget);
  m_analogScaleColorLabel = new QLabel(i18n("Color of scale :"),mainPageWidget);
  m_AScaleButton = new KColorButton (mainPageWidget);
  m_analogFontColorLabel = new QLabel(i18n("Color of font :"),mainPageWidget);
  m_AFontButton = new KColorButton (mainPageWidget);

  m_digitalProcessingLabel = new QLabel(i18n("Digital processing of pointers :"),mainPageWidget);
  m_digitalProcesingOfPointers = new KComboBox (mainPageWidget);


  setColorLayout->addWidget ( initAnalogLabel1, 0,0);
  setColorLayout->addWidget ( m_checkBoxCbac ,0,1);
  setColorLayout->addWidget ( m_analogBackgroundColorLabel, 1,0);
  setColorLayout->addWidget ( m_BGAButton ,1,1);
  setColorLayout->addWidget ( initAnalogLabel12, 2,0);
  setColorLayout->addWidget ( m_checkBoxCoac ,2,1);
  setColorLayout->addWidget ( m_analogPointerColorLabel, 3,0);
  setColorLayout->addWidget ( m_AFingerButton ,3,1);
  setColorLayout->addWidget ( m_analogOKColorLabel, 4,0);
  setColorLayout->addWidget ( m_AOKButton ,4,1);
  setColorLayout->addWidget ( m_analogWarningColorLabel, 5,0);
  setColorLayout->addWidget ( m_AWarnningButton ,5,1);
  setColorLayout->addWidget ( m_analogErrorColorLabel, 6,0);
  setColorLayout->addWidget ( m_AErrorButton ,6,1);
  setColorLayout->addWidget ( m_analogScaleColorLabel, 7,0);
  setColorLayout->addWidget ( m_AScaleButton ,7,1);
  setColorLayout->addWidget ( m_analogFontColorLabel, 8,0);
  setColorLayout->addWidget ( m_AFontButton ,8,1);
  setColorLayout->addWidget ( m_digitalProcessingLabel, 9,0);
  setColorLayout->addWidget ( m_digitalProcesingOfPointers, 9,1);

  m_digitalProcesingOfPointers->addItem(i18n("Nothing processing"));
  m_digitalProcesingOfPointers->addItem(i18n("Fast antialiasing"));
  m_digitalProcesingOfPointers->addItem(i18n("Fine antialiasing"));
  m_digitalProcesingOfPointers->addItem(i18n("Fast antialiasing and blur motion"));
  m_digitalProcesingOfPointers->addItem(i18n("Fine antialiasing and blur motion"));
 
  topLayout->addStretch( 20 );

  connect (m_checkBoxCbac,SIGNAL(toggled(bool)),this,SLOT(slotUseCustomBAnalogColor(bool)));
  connect (m_checkBoxCoac,SIGNAL(toggled(bool)),this,SLOT(slotUseCustomOAnalogColor(bool)));
  setDefaultPage(analogSettingPage);
  }



#include "moc_knutprefdlg.cpp"
