/***************************************************************************
                          knutvardata.h  -  description
                             -------------------
    begin                : Tue Aug 21 2001
    copyright            : (C) 2001 by Daniel Prynych
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

#ifndef KNUTVARDATA_H
#define KNUTVARDATA_H


#include "knutnet.h"
#include "knutupsdata.h"
#include "knutconst.h"

#include <QColor>
#include <QFont>




/**
  *@author Daniel Prynych
  */

 /**
  * Class contains data about analog panels,
  * name, ...
  * Describes view of panel.
  * Trida obsahuje data o analogovych panelech,
  * nazev, atd.
  * Popisuje jak ma prislusny panel vypadat
  *
  * @author Daniel Prynych
  * @short Information about analog panels
  * @version 0.4
  **/

class QString;

class KNutVarData {

public:

struct ScaleStructDef {
  int start;
  int end;
  int scaleColor;
  };

struct SettingDataDef {
  int countCols;
  QColor mainBackgroundColor;
  QColor mPanelBackgroundColor;
  QColor aPanelBackgroundColor;
  QColor analogFingerColor;
  QColor analogOKColor;
  QColor analogWarnningColor;
  QColor analogErrorColor;
  QColor analogScaleColor;
  QColor analogFontColor;
  unsigned int panelFlags;
  int inputVoltage;
  int inputFrequency;
  bool lowHighXfer;
  bool areYouSure;
  knc::mainWindowStartVariant useMainWindow;
  int useMessageWindow;
  bool customBColor;
  bool customBPanelColor;
  bool customBAnalogColor;
  bool customOAnalogColor;
  QFont mPanelFont; // main panel font
  QFont aPanelFont; // analog panel font
  QFont systemFont; // system font
  bool customFont; // use custom font
  bool useDescription;
  unsigned int toolTipDockFlags;
  bool customKIconColor;
  QColor kIconBackgroundColor;
  int typeOfKIcon;
  int typeDigitalProcessing;
  bool activatedMainWindow;
  };

struct upsInfoDef{  //include generally informations about active (selected) UPS
  KNutNet* upsNet;  // pointer to instance of KNutNet
  KNutUpsData::upsRecordDef record; // include information about information of ups (name, address, login, password ...)
  int netError; // kod chyby pri spojeni // kode if error of connection
  bool errorUpsData;
  // state of connection to usbd, this isn't state of knutnet
  knc::upsDriverConnection upsDriverState; // defined in knutconst.h
  int oldState; // code of lost state of UPS  kod posledniho stavu UPS
  QString name;
  QString password;
  KNutNet::nutVarVersion nutVariables;
  int nutProtocol;
  QString runtimeVar;
  QString lowxferVar;
  QString highxferVar;
  QString statusVar;
  QString upsLoadVar;
  bool reconnecting;  // is true when program is in reconnecting stage
  int reconnectCount; 
  };

struct VarStrucDef {
  QString nameAnalog; // jmeno promene v meraku  napr. UPS Load
  QString nameUnit; // merna jednotka napr %
  int typeAnalog; //typ meraku
  int numberItem; //pocet casti stupnice neni v souvislosti barvou
  int numberInItem; //pocet casti stupnice uvnitr dilku stupnice
  int start; // pocatecni hodnota v meraku
  int end; // konecna hodnota v meraku
  struct ScaleStructDef scaleStruct[5];
  };


  KNutVarData();
  ~KNutVarData();

 /**
  * Gets data of analog measure panel, type of measure panel, 
  * Vrati data analogoveho mericiho panelu.
  * typ meraku, pocet dilku stupnice, merici jedntotku atd
  * data je mozno take nastavovat
  * @param seqNumber Kod typu meraky.
  * @return  Data pro zadany typ panelu.
  * @since  0.1
  **/
VarStrucDef getData (const int seqNumber) const;


 /**
  * Puts data of analog measure panel
  * Nastavi data analogoveho mericiho panelu.
  *
  * @param seqNumber Kod typu meraky.
  * @param data Data pro zadany typ panelu.
  *
  * @since  0.1
  **/
void putData (const int seqNumber, VarStrucDef& varStruc);

 /**
  * Sets data of analog measure panel for input and output voltage
  * Nastavi data analogoveho mericiho panelu pro vstupni a vystupni napeti.
  *
  * @param typeVoltage Typ vstupniho napeti 230/120.
  *
  * @since  0.1
  **/
void setVoltage (const int typeVoltage);

 /**
  * Sets data of analog measure panel for input frequence
  * Nastavi data analogoveho mericiho panelu pro vstupni frekvence.
  *
  * @param typeFrequence Typ vstupni frekvence 50/60.
  *
  * @since  0.1
  **/
void setFrequency (const int typeFrequence);

 /**
  * Nastavi vnitrni cast vybarveni podle low/high xfer,
  * low/high-XFER udava rozpeti pri kterem UPS zpracovava napeti ze site.
  *
  * @param lowXfer Pocatecni hodnota.
  * @param highXfer Konecna hodnota.
  * @since  0.1
  **/
void setXfer (const float lowXfer, const float highXfer);

 /**
  * Vrati dlouhy nazev analogoveho panelu,
  * je to jmeno merene veliciny
  *
  * @param seqNumber Kod typu merakuss.
  * @since  0.1
  **/
QString getNameAnalogData (const int seqNumber);


/**
 * Translates code of ups variable to its text value
 *
 * @since  0.2
 **/
static const QString nameUpsVar (const int i, const KNutNet::nutVarVersion varVersion );

/**
 * Prevede kod chyby na jeho textovou reprezentaci
 *
 * @since  0.2
 **/
  static const QString errorToText (const int error);

private:

  enum {aRegWhite,aRegGreen,aRegYellow,aRegRed,aRegBlue,aRegCyan,aRegMagenta,aRegBlack};

  VarStrucDef m_dataVar[ knc::NumberOfTypes ];  /* udrzuje popis Anagovych meraku pro danou promenou jmeno zabarveni typ meraky */
  VarStrucDef m_dataVarVoltage[2];
  VarStrucDef m_dataVarFrequence[2];

  static const int U230V = 0;
  static const int U120V = 1;
  static const int F50HZ = 0;
  static const int F60HZ = 1;


  QString m_upsVarNameAnalog[ knc::NumberOfTypes ];//array of strings - includes name of analog meters
};


#endif
