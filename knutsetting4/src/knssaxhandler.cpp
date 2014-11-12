/***************************************************************************
                          knssaxhandler.cpp  -  description
                             -------------------
    begin                : ?t úno 17 2005
    copyright            : (C) 2005 by Daniel Prynych,Zajecov,Czech,
    email                : Daniel at prynych dot cz
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "knssaxhandler.h"
#include "knsstring.h"

#include <QtDebug>


KNSSaxHandler::KNSSaxHandler(KNSCommonData* commonData) : m_commonData(commonData){
  }
KNSSaxHandler::~KNSSaxHandler(){
  }

bool KNSSaxHandler::characters(const QString &str) {

  m_entityText += str;
  return true;
  }

bool KNSSaxHandler::startElement ( const QString&, const QString&, const QString& qName, const QXmlAttributes& attr ){
  QString attrName;
  QString attrValue;
  QString attrType;
  QString attrForce;
  QString attrVariable;

  m_entityText = "";
//  attrName = attr.value("name");
//  attrValue = attr.value("value");
//  attrType = attr.value("type");

  if (qName == "Modules") {
    m_commonData->modulesDefClean(); //clean up definition of modules (list of modules)
    return true;
    }
  if (qName == "Driver") {
    m_newModulDef.name=attr.value("name");
    m_newModulDef.distName=attr.value("name"); //Default common name is module name;
    m_newModulDef.description="";
    m_newModulDef.portBits=-1;
    m_newModulDef.portSpeed=-1;
    m_newModulDef.portType=PORT_UNKNOWN;
    m_newModulDef.portAuto=PORT_AUTO_NO;
    // clear param enum items
    paramDefList::iterator itp;
    for (itp = m_newModulDef.param.begin(); itp != m_newModulDef.param.end(); itp++)
      (*itp).paramEnumItems.clear();
    m_newModulDef.param.clear();
    return true;
    }
  if (qName == "PortSpeed") {
    int value = attr.value("value").toInt();
    if (value) m_newModulDef.portSpeed=value;
    return true;
    }
  if (qName == "PortBits") {
    int value = attr.value("value").toInt();
    if (value) m_newModulDef.portBits=value;
    return true;
    }
  if (qName == "Port") {
    QString type = attr.value("type");
    if (type == "serial") m_newModulDef.portType=PORT_SERIAL;
    if (type == "usb") m_newModulDef.portType=PORT_USB;
    QString autoVal = attr.value("auto");
    if (autoVal == "yes") m_newModulDef.portAuto=PORT_AUTO_YES;
    else {
      if (autoVal == "only") m_newModulDef.portAuto=PORT_AUTO_ONLY;
      else {
        if (autoVal == "no") m_newModulDef.portAuto=PORT_AUTO_NO;
        }
      }
    return true;
    }

  if (qName == "PortNameList") {
    m_newModulDef.portNames.clear();
    return true;
    }
  if (qName == "PortName") {
    KNSString value = attr.value("value");
    value.trimmedOwn();
    if (!value.isEmpty()) m_newModulDef.portNames.push_back(value);
    return true;
    }
  if (qName == "ParamList") {
    m_newModulDef.param.clear();
    return true;
    }
  if (qName == "ParamNumber") {
    m_newParam.name=attr.value("name");
    m_newParam.type=PARAM_NUMBER;
    if (attr.value("force") == "no") m_newParam.force = false;
    else m_newParam.force = true;
    m_newParam.variable = true;
    m_newParam.max=99;
    m_newParam.min=1;
    m_newParam.paramEnumItems.clear();
    m_newParam.parDef="";
    m_newParam.parDefNum=m_newParam.min;
    return true;
    }

  if (qName == "ParamString") {
    m_newParam.name=attr.value("name");
    m_newParam.type=PARAM_STRING;
    if (attr.value("force") == "no") m_newParam.force = false;
    else m_newParam.force = true;
    if (attr.value("variable") == "no") m_newParam.variable = false;
    else m_newParam.variable = true;
    m_newParam.max=0;
    m_newParam.min=0;
    m_newParam.paramEnumItems.clear();
    m_newParam.parDef="";
    m_newParam.parDefNum=0;
    return true;
    }
  if (qName == "ParamNumberDefault") {
    m_newParam.parDefNum=attr.value("value").toInt();
    }
  if (qName == "ParamMinimum") {
    m_newParam.min=attr.value("value").toInt();
    }
  if (qName == "ParamMaximum") {
    m_newParam.max=attr.value("value").toInt();
    }
  if (qName == "ParamEnumDefault") {
    m_newParam.parDef=attr.value("value");
    }
  if (qName == "ParamEnum") {
    m_newParam.name=attr.value("name");
    m_newParam.type=PARAM_ENUM;
    if (attr.value("force") == "no") m_newParam.force = false;
    else m_newParam.force = true;
    m_newParam.variable = true;
    m_newParam.max=0;
    m_newParam.min=0;
    m_newParam.paramEnumItems.clear();
    m_newParam.parDef="";
    m_newParam.parDefNum=0;
    return true;
    }
  if (qName == "ParamEnumValueList") {
    m_newParam.paramEnumItems.clear();
    return true;
    }
  if (qName == "ParamEnumValue") {
    m_paramEnumItem.value=attr.value("value");
    return true;
    }


  return true;
  }

bool KNSSaxHandler::endElement ( const QString&, const QString&, const QString& qName) {

  if (qName == "Driver") {
    if (!(m_newModulDef.name.isEmpty())) {
      if (!m_commonData->modulesDefExist(m_newModulDef.name)) {
        m_commonData->modulesDefAddr()->push_back(m_newModulDef); //add definition of modules into list of modules
        }
      }
    }

  if (qName == "Description") m_newModulDef.description=m_entityText;

  //When part common name is included in definition of module, commonName will be setted
  if (qName == "DistinguishName") m_newModulDef.distName=m_entityText;
  if (qName == "ParamStringDefault") {
    m_newParam.parDef=m_entityText;
    }
//  if (qName == "ParamNumberDefault") {
//    newParam.parDefNum=entityText.toInt();
//    }
  if (qName == "ParamNumber") {
    if (m_newParam.parDefNum < m_newParam.min) m_newParam.parDefNum=m_newParam.min;
    if (m_newParam.parDefNum > m_newParam.max) m_newParam.parDefNum=m_newParam.max;
    m_newModulDef.param.push_back(m_newParam);
    }
  if (qName == "ParamString") {
    m_newModulDef.param.push_back(m_newParam);
    }
  if (qName == "ParamEnumValue") {
    m_paramEnumItem.text=m_entityText;
    m_newParam.paramEnumItems.push_back(m_paramEnumItem);
    }
  if (qName == "ParamEnum") {
    m_newModulDef.param.push_back(m_newParam);
    }

  m_entityText = "";
  return true;

  }
