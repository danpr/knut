/***************************************************************************
                          knutvardata.cpp  -  description
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

#include "knutvardata.h"
#include "knutnet.h"

#include <KDE/KLocale>

#include <QtGui/QColor>
#include <QtCore/QString>
#include <QtGui/QFont>

//#include <iostream>

  const char *nameUpsVars1[] = {
    "NONE", "UTILITY", "BATTPCT", "UPSTEMP", "ACFREQ", "LOADPCT", "AMBTEMP", "AMBHUMID","BATTVOLT","OUTVOLT","CURRENT","NONE","BATT_TEMP","BATT_CURRENT","NONE","NONE"};

  const char *nameUpsVars2[] = {
    "none", "input.voltage", "battery.charge", "ups.temperature", "input.frequency", "ups.load", "ambient.temperature", "ambient.humidity","battery.voltage","output.voltage","output.current","output.frequency","battery.temperature","battery.current","input.current","ups.power"};

/*
 * Nic,Vstupni Napeti,Baterie,Teplota UPSky,
 * Vstupni frekvence,Zatizeni,Vnejsi teplota,
 * Vnejsi vlhkost,Napeti baterie, Vystupni napeti
 * Vystupni proud, Teplota baterie, Proud baterie
*/

const QString KNutVarData::nameUpsVar (const int i, const KNutNet::nutVarVersion varVersion ) { 

  if (i < 0) return "";
  switch ( varVersion ) {
    case KNutNet::versionVarOne:
      return nameUpsVars1[i];
    break;
    case KNutNet::versionVarTwo:
      return nameUpsVars2[i];
    break;
    case KNutNet::versionVarUnknown:
    default:
      return "";
    break;
    }
  }



KNutVarData::KNutVarData(){

  m_upsVarNameAnalog[0] = i18n("None");
  m_upsVarNameAnalog[1] = i18n("Input Voltage");
  m_upsVarNameAnalog[2] = i18n("Battery Charge");
  m_upsVarNameAnalog[3] = i18n("UPS Temperature");
  m_upsVarNameAnalog[4] = i18n("Input Frequency");
  m_upsVarNameAnalog[5] = i18n("UPS Load");
  m_upsVarNameAnalog[6] = i18n("Outside Temperature");
  m_upsVarNameAnalog[7] = i18n("Outside Humidity");
  m_upsVarNameAnalog[8] = i18n("Battery Voltage");
  m_upsVarNameAnalog[9] = i18n("Output Voltage");
  m_upsVarNameAnalog[10] = i18n("Output Current");
  m_upsVarNameAnalog[11] = i18n("Output Frequency");
  m_upsVarNameAnalog[12] = i18n("Battery Temperature");
  m_upsVarNameAnalog[13] = i18n("Battery Current");
  m_upsVarNameAnalog[14] = i18n("Input Current");
  m_upsVarNameAnalog[15] = i18n("UPS Power");

  static const QString upsVarNameUnit[] =	{"", "V", "%", "C", "Hz", "%", "C", "%", "V", "V", "A","Hz","C", "A", "A", "VA"};
  static const int upsVarType[] = {0,3,2,1,1,2,1,1,3,3,2,1,1,2,2,2};  //typ panelu
  static const int upsVarItem[] = {5,4,5,5,6,6,5,5,5,4,5,6,5,5,5,5}; // pocet velkych policek na stupnici
  static const int upsVarInItem[] = {0,3,0,0,4,0,0,0,0,3,5,4,0,5,5,5}; // pocet malych policek ve velkem policku na stupnici
  static const int upsVarStart[] = {0,170,0,-10,20,0,-10,0,0,170,0,20,-10,-15,-15,0};
  static const int upsVarEnd[] = {0,290,100,90,80,120,70,100,10,290,5,80,90,10,10,500};
  static const int upsVarScaleStart[ knc::NumberOfTypes ][5] = {{0,0,0,0,0},{0,210,0,0,0},{50,0,40,0,0},{-100,50,0,0,0},{0,40,45,0,0},{0,80,100,0,0},{-100,50,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{210,0,250,0,0},{0,0,0,0,0},{0,40,45,0,0},{-100,50,0,0,0},{0,0,0,0,0}, {0,0,0,0,0},{0,0,0,0,0}};
  static const int upsVarScaleEnd[ knc::NumberOfTypes ][5] = {{0,0,0,0,0},{400,250,0,0,0},{120,40,50,0,0},{50,100,0,0,0},{90,60,55,0,0},{80,100,120,0,0},{50,70,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{250,210,400,0,0},{0,0,0,0,0},{90,60,55,0,0},{50,100,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}};


  int upsVarScaleColor[ knc::NumberOfTypes ][5] = {{aRegWhite,aRegWhite,aRegWhite,aRegWhite,aRegWhite},
    {aRegRed,aRegGreen,aRegWhite,aRegWhite,aRegWhite},
    {aRegGreen,aRegRed,aRegYellow,aRegWhite,aRegWhite},
    {aRegGreen,aRegRed,aRegWhite,aRegWhite,aRegWhite},
    {aRegRed,aRegYellow,aRegGreen,aRegWhite,aRegWhite},
    {aRegGreen,aRegYellow,aRegRed,aRegWhite,aRegWhite},
    {aRegGreen,aRegRed,aRegWhite,aRegWhite,aRegWhite},
    {aRegWhite,aRegWhite,aRegWhite,aRegWhite,aRegWhite},
    {aRegGreen,aRegRed,aRegRed,aRegWhite,aRegWhite},
    {aRegGreen,aRegRed,aRegRed,aRegWhite,aRegWhite},
    {aRegWhite,aRegWhite,aRegWhite,aRegWhite,aRegWhite},
    {aRegRed,aRegYellow,aRegGreen,aRegWhite,aRegWhite},
    {aRegGreen,aRegRed,aRegWhite,aRegWhite,aRegWhite},
    {aRegWhite,aRegWhite,aRegWhite,aRegWhite,aRegWhite},
    {aRegWhite,aRegWhite,aRegWhite,aRegWhite,aRegWhite},
    {aRegWhite,aRegWhite,aRegWhite,aRegWhite,aRegWhite},};
 

  for (int i=0;i < knc::NumberOfTypes; i++) {
    m_dataVar[i].nameAnalog = m_upsVarNameAnalog[i];
    m_dataVar[i].nameUnit = upsVarNameUnit[i];
    m_dataVar[i].typeAnalog = upsVarType[i];
    m_dataVar[i].numberItem = upsVarItem[i];
    m_dataVar[i].numberInItem = upsVarInItem[i];
    m_dataVar[i].start = upsVarStart[i];
    m_dataVar[i].end = upsVarEnd[i];
    for (int j=0; j< 5; j++) {
      m_dataVar[i].scaleStruct[j].start=upsVarScaleStart[i][j];
      m_dataVar[i].scaleStruct[j].end=upsVarScaleEnd[i][j];
      m_dataVar[i].scaleStruct[j].scaleColor=upsVarScaleColor[i][j];
      }
    }

  m_dataVarVoltage[U230V]=m_dataVar[knc::VarInVolt];
  m_dataVarVoltage[U120V]=m_dataVar[knc::VarInVolt];
  m_dataVarVoltage[U120V].start=90;
  m_dataVarVoltage[U120V].end=150;
  m_dataVarVoltage[U120V].scaleStruct[0].start=0;
  m_dataVarVoltage[U120V].scaleStruct[1].start=110;
  m_dataVarVoltage[U120V].scaleStruct[0].end=200;
  m_dataVarVoltage[U120V].scaleStruct[1].end=130;

  m_dataVarFrequence[F50HZ]=m_dataVar[knc::VarInFreq];
  m_dataVarFrequence[F60HZ]=m_dataVar[knc::VarInFreq];
  m_dataVarFrequence[F60HZ].start=30;
  m_dataVarFrequence[F60HZ].end=90;
  m_dataVarFrequence[F60HZ].scaleStruct[0].start=0;
  m_dataVarFrequence[F60HZ].scaleStruct[1].start=50;
  m_dataVarFrequence[F60HZ].scaleStruct[2].start=55;
  m_dataVarFrequence[F60HZ].scaleStruct[0].end=100;
  m_dataVarFrequence[F60HZ].scaleStruct[1].end=70;
  m_dataVarFrequence[F60HZ].scaleStruct[2].end=65;
  }

KNutVarData::~KNutVarData(){
}


KNutVarData::VarStrucDef KNutVarData::getData (const int seqNumber) const {
   int i = seqNumber;

   if (i < 1) i = 1;
   else {if (i > (knc::NumberOfTypes-1)) i = knc::NumberOfTypes - 1;};
   return m_dataVar[i];
  }



QString KNutVarData::getNameAnalogData (const int seqNumber) {
  if ((seqNumber >= 0) && (seqNumber < knc::NumberOfTypes))
    return m_upsVarNameAnalog[seqNumber];
  else
    return "";
  }


void KNutVarData::putData (const int seqNumber, VarStrucDef& varStruc) {
  if (seqNumber > 0 && seqNumber < knc::NumberOfTypes) m_dataVar[seqNumber] = varStruc;
  }

void KNutVarData::setVoltage (const int typeVoltage) {
  if (typeVoltage == 230) {
    m_dataVar[knc::VarInVolt] = m_dataVarVoltage[U230V];
    m_dataVar[knc::VarOutVolt] = m_dataVarVoltage[U230V];
//    m_dataVar[knc::VarOutVolt].nameAnalog = upsVarNameAnalog[knc::VarOutVolt]; // je nutno nastavit nazev
    }
  if (typeVoltage == 120) {
    m_dataVar[knc::VarInVolt] = m_dataVarVoltage[U120V];
    m_dataVar[knc::VarOutVolt] = m_dataVarVoltage[U120V];
//    dataVar[knc::VarOutVolt].nameAnalog = upsVarNameAnalog[knc::VarOutVolt]; // je nutno nastavit nazev
    }
  m_dataVar[knc::VarOutVolt].nameAnalog = m_upsVarNameAnalog[knc::VarOutVolt]; // je nutno nastavit nazev
  }

void KNutVarData::setFrequency (const int typeFrequence) {
  if (typeFrequence == 50) {
    m_dataVar[knc::VarInFreq] = m_dataVarFrequence[F50HZ];
    m_dataVar[knc::VarOutFreq] = m_dataVarFrequence[F50HZ];
    }
  if (typeFrequence == 60) {
    m_dataVar[knc::VarInFreq] = m_dataVarFrequence[F60HZ];
    m_dataVar[knc::VarOutFreq] = m_dataVarFrequence[F60HZ];
    }
  m_dataVar[knc::VarOutFreq].nameAnalog = m_upsVarNameAnalog[knc::VarOutFreq]; // je nutno nastavit nazev
  }

void KNutVarData::setXfer (const float lowXfer, const float highXfer) {
  m_dataVar[knc::VarInVolt].scaleStruct[1].start=(int)lowXfer;
  m_dataVar[knc::VarInVolt].scaleStruct[1].end=(int)highXfer;
  m_dataVar[knc::VarOutVolt].scaleStruct[1].start=(int)lowXfer;
  m_dataVar[knc::VarOutVolt].scaleStruct[1].end=(int)highXfer;
  }


const QString KNutVarData::errorToText (const int error) {

  switch (error) {

  // knutclient error
    case KNutNet::LowMem: return i18n("No memory.");
    case KNutNet::NullAddress: return i18n("No address.");
    case KNutNet::NoSuchHost: return i18n("Unknown address.");
    case KNutNet::SocketReadErr:   // socket
    case KNutNet::ConnectErr:  // connect
      return i18n("Error of connection.");
    
    case KNutNet::NetworkError:
      return i18n("Network error, maybe net cable was plugged out.");

    case KNutNet::CantConnect:
      return i18n("No connection with server.");

    case KNutNet::NutProtocolVarErr: return i18n("Server-client protocol or variables of NUT are unknown.");  
    case KNutNet::NoUpsHere: return i18n("No UPS on this address.");
    case KNutNet::ConnectionRefused: return i18n("Connection was refused by server.");

    case KNutNet::SendErr: return i18n("Server doesn't receive data.");
    case KNutNet::RecvErr: return i18n("Server doesn't send data.");
    case KNutNet::NoServerData: return i18n("Server doesn't answer.");

    case KNutNet::UnknownFormat: return i18n("Server returns data with unknown format.");
    case KNutNet::UnknownAnswer: return i18n("Server returns unknown data.");
    case KNutNet::UnknownFormatVer: return i18n("Command VER returns data with unknown format.");

    case KNutNet::NoData: return i18n("No data.");
    case KNutNet::NotConnection: return i18n("Connection doesn't exist.");

// server error
    case KNutNet::AccessDenied: return i18n("Access denied. Failed password ?");
    case KNutNet::DataStale: return i18n("UPS doesn't sent data to server (Data Stale).");
    case KNutNet::UnknownCommand: return i18n("Server doesn't know this command.");

// driver error
    case KNutNet::DriverNotConnected: return i18n("UPS's driver isn't connected.");

    case KNutNet::PasswordRequired: return i18n("Server required password.");
    case KNutNet::PasswordIncorrect: return i18n("Incorrect password.");
    case KNutNet::NoResponse: return i18n("UPS doesn't response.");
    case KNutNet::CommandFailed: return i18n("Command failed.");
    case KNutNet::UnknownInstCmd: return i18n("UPS doesn't know this instant command.");
    case KNutNet::CmdNotSupported: return i18n("UPS doesn't support this instant command.");
    case KNutNet::VarUnknown: return i18n("UPS doesn't known this variable.");
    case KNutNet::VarNotSupported:  return i18n("UPS doesn't support this variable.");
    case KNutNet::InvalidValue:  return i18n("UPS doesn't support this value in this variable.");
    case KNutNet::UnknownUps: return i18n("Name of UPS is unknown.");

    case KNutNet::AlreadySetUsername: return i18n("Username has been already entered.");
    case KNutNet::AlreadySetPassword: return i18n("Password has been already entered.");

    case KNutNet::NoListVars: return i18n("Server doesn't send list of variables.");

//default
    default: return i18n("Unknown error.");
    }
  }

