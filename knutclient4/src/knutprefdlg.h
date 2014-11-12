/***************************************************************************
                          knutprefdlg.h  -  description
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

#ifndef KNUTPREFDLG_H
#define KNUTPREFDLG_H

#include "knutnewupsdlg.h"
#include "knutupsdata.h"
#include "knutvardata.h"
#include "knutconst.h"


#include <KDE/KPageDialog>
#include <KDE/KPushButton>
#include <KDE/KListWidget>
#include <KDE/KLocale>
#include <KDE/KFontDialog>
#include <KDE/KColorButton>
#include <KDE/KComboBox>
#include <KDE/KPageWidgetModel>

#include <QtGui/QWidget>

class QLabel;
class QFont;
class QCheckBox;
class QRadioButton;
class QSpinBox;


/**
  *@author Daniel Prynych
  */

/**
 * This class makes configure dialog
 * Tato trida je vytvari konfiguracni dialog.
 * @author Daniel Prynych
 * @short Konfiguracni dialog
 * @version 0.9
 */
class KNutPrefDlg : public KPageDialog
{
  Q_OBJECT
  public:

enum changePref {
  ChangeCountCols =             0x1, //1,
  ChangeCustomBColor =          0x2, //2,
  ChangeMBColor =               0x4, //4,
  ChangeVoltage =               0x8, // 8,
  ChangeFrequency =            0x10, // 16,
  ChangeXFer =                 0x20, // 32,

  ChangeUpsSetting =           0x40, // 64,
  ChangeUpsCount =             0x80, // 128,

  ChangeMainPanelSetting =    0x100, // 256,
  ChangeCustomMPanelBColor =  0x200, // 512,
  ChangeMPBColor =            0x400, // 1024,

  ChangeAnalogFont =          0x800, // 2048,
  ChangePanelFont =          0x1000, // 4096,

  ChangeCustomAPanelBColor = 0x2000, // 8192,
  ChangeAPBColor =           0x4000, // 16384,

  ChangeCustomAPanelOColor = 0x8000, // 32768,
  ChangeAPOColor =          0x10000, // 65536,

  ChangeCustomKIconColor =  0x20000, // 131072,
  ChangeKIconBColor =       0x40000, // 262144,

  ChangeToolTip =           0x80000, // 524288,

  ChangeTypeOfKIcon =      0x100000,  // 1048576

  ChangeTypeOfDigProcessing =      0x200000,  // 1048576 *2

  ChangeSomeThing =        0x400000  // 1048576 *4

  };


enum  {mainPanelFont = 0, analogPanelFont};



/**
 * Constructor makes configure dialog.
 * Konstruktor vytvori konfiguracni dialog.
 *
 * @param upsData Is pointer on instance of class of KNutUpsData,  which manage informations about UPSes.
 * @param analogData Is pointer on instance of class of KNutVarData, which manages data about analog panels.
 * @param settingData Is pointer on struct, which contain values is setted in dialog.
 * @param upsName Is name of active UPS.
 *
 * @param upsData Je odkaz na instanci tridy KNutUpsData, ktera spravuje informace o UPS-kach.
 * @param analogData Je odkaz na na instanci tridy KNutVarData, ktera spravuje data o analogovych panelech.
 * @param settingData Je odkaz na strukturu ktera obsahuje hodnoty nastavovane v dialogu.
 * @param upsName Je jmeno aktivni UPS-ky.
 *
 * @since  0.6
 **/
  KNutPrefDlg(KNutUpsData* const upsData, KNutVarData* const analogData, KNutVarData::SettingDataDef* const settingData, const QString upsName, QWidget* parent = 0);

/**
 * @since  0.3
 **/
  ~KNutPrefDlg();

  signals:

/**
 * Signal is emited when statement is changed same pages.
 * Signal je aktivovan pokud je zmenen udaj na kterekoliv strance.
 *
 * @param changeSetting Declare, what item was changed.
 * @param changeSetting Udava, ktery prvek byl zmenen.
 *
 * @since  0.2
 **/
  void signalChangeConfig (unsigned int changeSetting );


  private slots:

/**
 * @internal
 */
  virtual void slotDefault();

/**
 * @internal
 */
  virtual void slotApply();

/**
 * @internal
 */
  virtual void slotOk();

/**
 * @internal
 */
  void slotChangeSettingFont(int index);

/**
 * @internal
 */
  void slotSetPictureKI (bool on);


/**
 * @internal
 */
  void slotUseCustomBColor(bool enable);

/**
 * @internal
 */
  void slotUseCustomBPanelColor(bool enable);

/**
 * @internal
 */
void slotUseCustomBAnalogColor(bool enable);


/**
 * @internal
 */
void slotUseCustomOAnalogColor(bool enable);

/**
 * @internal
 */
void slotUseCustomBIconColor(bool enable);

/**
 * @internal
 */
void slotEnableVoltageGroup(bool enable);

  
/**
 * @internal
 */
void slotEnableFrequencyGroup(bool enable);

/**
 * @internal
 */
  void slotNewUps();

/**
 * @internal
 */
  void slotEditUps();

/**
 * @internal
 */
  void slotDeleteUps();


  private:

/**
 * @internal
 */
  void initSetting();

/**
 * @internal
 */
  void initUps();

/**
 * @internal
 */
  void initPanel();

/**
 * @internal
 */
  void initFonts();

/**
 * @internal
 */
  void initAnalog ();

/**
 * @internal
 */
  void initDock ();


/**`
 * @internal
 *  Sets default setting for selected page of this dialog.
 */
  void setDefaultPage( KPageWidgetItem *page);


/**`
 * @internal
 *  Sets pages of this dialog by setting information
 */
   void setPages( void ); 

// Private values
//Pointers on pages from KPageDialog
  KPageWidgetItem *mainSettingPage;
  KPageWidgetItem *upsSettingPage;
  KPageWidgetItem *panelSettingPage;
  KPageWidgetItem *fontSettingPage;
  KPageWidgetItem *analogSettingPage;
  KPageWidgetItem *dockSettingPage;

  QString m_activeUpsName; // name of UPS which is active
  KNutUpsData *m_myUpsData;
  KNutVarData *m_myAnalogData;
  KNutVarData::SettingDataDef *m_settingData;
  KNutNewUpsDlg *newUpsDlg;

  KListWidget *m_listWidget2; // uses in part UPS, contain name of UPS

  bool m_checked; //it's settted when, definition(parameters) of UPS were changed // nastaveno kdyz doslo k zmene v definici UPS
  bool m_checkedActiveUps; //it's setted when, parameters of active UPS were changed // nastaveno pokud doslo ke zmene na aktivni UPS

  QSpinBox *m_numberOfColumns;
  KColorButton *m_BGButton;
  QCheckBox *m_checkBoxCbc;
  QCheckBox *m_checkBoxAys;
  QCheckBox *m_checkBoxMesW;
  KComboBox *m_comboBoxMainWindow;
  QCheckBox *m_xfer;

  QCheckBox *m_checkBoxOver;
  QCheckBox *m_checkBoxBL;
  QCheckBox *m_checkBoxRB;
  QCheckBox *m_checkBoxCal;
  QCheckBox *m_checkBoxMFRM;
  QCheckBox *m_checkBoxMFR;
  QCheckBox *m_checkBoxModel;
  QCheckBox *m_checkBoxSerial;
  QCheckBox *m_checkBoxFirm;
  QCheckBox *m_checkBoxRun;
  QCheckBox *m_checkBoxCbpc;
  QCheckBox* m_checkBoxCbac;
  QCheckBox* m_checkBoxCoac;
  QCheckBox *m_checkBoxCbi;
  QCheckBox *m_checkBoxTTMFR;
  QCheckBox *m_checkBoxTTModel;
  QCheckBox *m_checkBoxTTSerial;
  QCheckBox *m_checkBoxTTFirm;
  QCheckBox *m_checkBoxTTRun;
  QCheckBox *m_checkBoxTTCharge;
  QCheckBox *m_checkBoxTTLoad;

  QLabel *m_panelBackgroundColorLabel;
  QLabel *m_mainBackgroundColorLabel;
  QLabel *m_analogBackgroundColorLabel;
  QLabel *m_dockBackgroundColorLabel;
  QLabel *m_analogPointerColorLabel;
  QLabel *m_analogOKColorLabel;
  QLabel *m_analogWarningColorLabel;
  QLabel *m_analogErrorColorLabel;
  QLabel *m_analogScaleColorLabel;
  QLabel *m_analogFontColorLabel;
  QLabel *m_digitalProcessingLabel;
  QLabel *m_iconDockLabel;

  QRadioButton *m_vRadioButton1;
  QRadioButton *m_vRadioButton2;
  QRadioButton *m_fRadioButton1;
  QRadioButton *m_fRadioButton2;
  QRadioButton *m_iconRadioButton1;
  QRadioButton *m_iconRadioButton2;

  KColorButton *m_BGPButton;
  KColorButton *m_BGAButton;
  KColorButton *m_AFingerButton;
  KColorButton *m_AOKButton;
  KColorButton *m_AWarnningButton;
  KColorButton *m_AErrorButton;
  KColorButton *m_AScaleButton;
  KColorButton *m_AFontButton;
  KColorButton *m_BGIButton;

  KComboBox *m_digitalProcesingOfPointers;

  QGroupBox *m_voltageGroup;
  QGroupBox *m_frequencyGroup;

  QCheckBox *m_customFont; // User's font 
  KFontChooser *m_fontWidget;

  KComboBox *m_panelFonts;  //  Combo box for select between analog panel a main panel
  QFont m_aPanelFont; // Analog panel font
  QFont m_mPanelFont; // Main panel font

  int m_settingFont; // which font is setted (selected)

//  QPixmap m_panelPix;
//  QPixmap m_mSetPix;
//  QPixmap m_upsesPix;
//  QPixmap m_analogPix;
//  QPixmap m_dockPix;

};

#endif
