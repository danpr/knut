/***************************************************************************
                          knsupssaxhandler.cpp  -  description
                             -------------------
    begin                : ?t kv? 12 2005
    copyright            : (C) 2005 by Daniel Prynych,Zajecov,Czech,
    email                : Daniel Prynych@alo.cz
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "knsupssaxhandler.h"

#include <QtDebug>
#include <QDebug>



KNSUPSSaxHandler::KNSUPSSaxHandler (KNSCommonData* commonData) : m_commonData(commonData) {

  }

KNSUPSSaxHandler::~KNSUPSSaxHandler(){
}


bool KNSUPSSaxHandler::characters(const QString &str) {

  m_entityText += str;
  return true;
  }

bool KNSUPSSaxHandler::startElement ( const QString&, const QString&, const QString& qName, const QXmlAttributes& attr ){
  struct paramDriverStruct paramDriver;

  m_entityText = "";

  if (qName == "Upses") {
    return true;
    }

  if (qName == "Model") {
    m_pref = attr.value("pref").toInt();
    m_driverName="";
    m_driverPort="";
    m_driverDesc="";
    m_driverMaxStartDelayFound=false;
    m_driverMaxStartDelay=0;
    m_driverNoLockFound=false;
    m_paramsList.clear();
    return true;
    }

  if (qName == "ManufactNameList") {
    m_manufactList.clear();
    return true;
    }

  if (qName == "ManufactName") {
    m_manufactList.push_back(attr.value("value"));
    return true;
    }

  if (qName == "ModelNameList") {
    m_modelList.clear();
    return true;
    }

  if (qName == "ModelName") {
    m_modelList.push_back(attr.value("value"));
    return true;
    }


  if (qName == "DriverName") {
    m_driverName=attr.value("name");
    m_driverPort=attr.value("port");
    return true;
    }

  if (qName == "DriverMaxStartDelay") {
    m_driverMaxStartDelay= attr.value("value").toInt();
    m_driverMaxStartDelayFound=true;
    return true;
    }

  if (qName == "DriverNoLock") {
    m_driverNoLockFound=true;
    return true;
    }


  if (qName=="Description") {
    m_driverDesc="";
    return true;
    }

  if (qName == "ParamDefList") {
    m_paramsList.clear();
    return true;
    }

  if (qName == "ParamDef") {
    paramDriver.paramName=attr.value("name");
    paramDriver.value=attr.value("value");
    m_paramsList.push_back(paramDriver);
    return true;
    }
  return true;
  }

bool KNSUPSSaxHandler::endElement ( const QString&, const QString&, const QString& qName) {
  upsModelDriverStruct newModelDriver;
  upsModelDriversList::iterator driverIt;
  QStringList::const_iterator modelIt;
  QStringList::const_iterator manufactIt;

  if (qName=="Description") {
    m_driverDesc+=m_entityText;
    return true;
    }


  if (qName=="Model") {
    //save informations about ups's model to list
    if ((m_modelList.count() > 0) && (m_manufactList.count() > 0) && (!m_driverName.isEmpty())) {
      newModelDriver.pref = m_pref;
      if (m_paramsList.count() > 0)
        newModelDriver.paramsList=m_paramsList;
      else
        newModelDriver.paramsList.clear();
      newModelDriver.driverName=m_driverName;  // name of driver , blazer_ser, blazer_usb ....
      newModelDriver.description=m_driverDesc;
      newModelDriver.driverMaxStartDelay=m_driverMaxStartDelay;
      newModelDriver.driverMaxStartDelayFound=m_driverMaxStartDelayFound;
      newModelDriver.driverNoLockFound=m_driverNoLockFound;
      if (m_driverPort=="serial") newModelDriver.portType=PORT_SERIAL;
      else newModelDriver.portType=PORT_USB;


      if ( m_commonData->modulesDefExist(m_driverName)) { 

// adds driver for all ups and all manufactory

        for (manufactIt = m_manufactList.begin();manufactIt != m_manufactList.end(); manufactIt++) {
          for (modelIt = m_modelList.begin();modelIt != m_modelList.end(); modelIt++) {
          // add new record of model(driver) for ups
       
            m_commonData->addManufactModelDriver((*manufactIt),(*modelIt),newModelDriver); //QString, QString
            }
          }
        }
      }
    return true;
    }
  return true;
  }
