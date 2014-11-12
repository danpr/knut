/***************************************************************************
                          knscommnodata.cpp  -  description
                             -------------------
    begin                : ?t dub 16 2010
    copyright            : (C) 2010 by Daniel Prynych,,,
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

#include "knscommondata.h"

#include <QtGui/QWidget>
#include <QtCore/QString>

#include <QtDebug>


KNSCommonData::KNSCommonData(QWidget *parent ) : QObject(parent) {

  m_allCommonData = new allCommonDataStruct();

  // clear modules' lists
  modulesDataClean();
  monitorDataClean();
  }

KNSCommonData::~KNSCommonData(){

  modulesDataClean();
  ACDataClean();
  usersDataClean();
//  monitorDataClean doesn't free memory or record in list, only sets default values
//  monitorDataClean();
  monitorUpsDataClean();

  UPSDataClean();
  m_allCommonData->driverData.clear();
  delete m_allCommonData;
  }


modulesList* KNSCommonData::modulesDataAddr(void){

  return  &m_allCommonData->modulesListData;
  }


ACDataStruct* KNSCommonData::ACDataAddr(void){

  return  &m_allCommonData->ACData;
  }

listenAddressPortListDef* KNSCommonData::ListenAPAddr(void){

  return  &m_allCommonData->ACData.listenAddressPortList;
  }



usersPermList* KNSCommonData::usersDataAddr(void){

  return  &m_allCommonData->usersData;
  }


monitorDataStruct* KNSCommonData::monitorDataAddr(void){

  return  &m_allCommonData->monitorData;
  }


monitorValuesStruct* KNSCommonData::monitorValuesDataAddr(void){

  return  &m_allCommonData->monitorData.monitorValues;
  }

monNotifyItemStruct* KNSCommonData::monitorNotifyDataAddr(void){

  return  &m_allCommonData->monitorData.monNotifyItems[0];
  }


void KNSCommonData::monitorDataClean(void){

   monitorUpsDataClean();

  m_allCommonData->monitorData.runAsUser="";
  m_allCommonData->monitorData.runAsUserFind=false;
  m_allCommonData->monitorData.monitorValues.minSupplies=minSuppliesDef;
  m_allCommonData->monitorData.monitorValues.minSuppliesFound=false;
  m_allCommonData->monitorData.monitorValues.shutDownCmd=shutDownCmdDef;
  m_allCommonData->monitorData.monitorValues.shutDownCmdFound=false;
  m_allCommonData->monitorData.monitorValues.notifyCmd=notifyCmdDef;
  m_allCommonData->monitorData.monitorValues.notifyCmdFound=false;
  m_allCommonData->monitorData.monitorValues.pollFreq=pollFreqDef;
  m_allCommonData->monitorData.monitorValues.pollFreqFound=false;
  m_allCommonData->monitorData.monitorValues.pollAlertFreq=pollAlertFreqDef;
  m_allCommonData->monitorData.monitorValues.pollAlertFreqFound=false;
  m_allCommonData->monitorData.monitorValues.hotSync=hotSyncDef;
  m_allCommonData->monitorData.monitorValues.hotSyncFound=false;
  m_allCommonData->monitorData.monitorValues.deadTime=deadTimeDef;
  m_allCommonData->monitorData.monitorValues.deadTimeFound=false;
  m_allCommonData->monitorData.monitorValues.powerDown=powerDownDef;
  m_allCommonData->monitorData.monitorValues.powerDownFound=false;
  m_allCommonData->monitorData.monitorValues.rbWarnTime=rbWarnTimeDef;
  m_allCommonData->monitorData.monitorValues.rbWarnTimeFound=false;
  m_allCommonData->monitorData.monitorValues.noCommWarnTime=noCommWarnTimeDef;
  m_allCommonData->monitorData.monitorValues.noCommWarnTimeFound=false;
  m_allCommonData->monitorData.monitorValues.finalDelay=finalDelayDef;
  m_allCommonData->monitorData.monitorValues.finalDelayFound=false;

  for (int i = 0; i < monNotifyItemsCount; i++) {
    m_allCommonData->monitorData.monNotifyItems[i].syslogFlag=false;
    m_allCommonData->monitorData.monNotifyItems[i].wallFlag=false;
    m_allCommonData->monitorData.monNotifyItems[i].execFlag=false;
    m_allCommonData->monitorData.monNotifyItems[i].ignoreFlag=false;
    m_allCommonData->monitorData.monNotifyItems[i].message="";
    }
  }

void KNSCommonData::modulesDataClean(void){
  modulesList::iterator it;

  for (it=m_allCommonData->modulesListData.begin(); it != m_allCommonData->modulesListData.end();++it )
      (*it).items.clear();
  m_allCommonData->modulesListData.clear();
  }

bool KNSCommonData::modulesDataFindPar(const QString moduleName, QString const parName, moduleItems::iterator parItem){
  modulesList::iterator it;
  moduleItems::iterator its;

  for (it=m_allCommonData->modulesListData.begin(); it != m_allCommonData->modulesListData.end();++it ) {
    if ((*it).name== moduleName) {
      for (its=(*it).items.begin(); its != (*it).items.end(); ++its ) {
        if ((*its).itemName==parName) {
          parItem = its;
          return true;
          }
        }
      return false;
      }
    }
  return false;
  }

void KNSCommonData::usersDataClean(void){
  usersPermList::Iterator it;

  for (it = m_allCommonData->usersData.begin(); it != m_allCommonData->usersData.end(); it++) {
    (*it).instcmds.clear();
    }
  m_allCommonData->usersData.clear();
  }


void KNSCommonData::ACDataClean(void){

  m_allCommonData->ACData.listenAddressPortList.clear();
  m_allCommonData->ACData.maxAge=maxAgeDef;
  m_allCommonData->ACData.maxAgeFound=false;
  m_allCommonData->ACData.maxConn=maxConnDef;
  m_allCommonData->ACData.maxConnFound=false;
  }

QString KNSCommonData::findUserPassword (const QString name) {
  usersPermList::const_iterator it;

  for (it = m_allCommonData->usersData.begin(); it != m_allCommonData->usersData.end(); it++) {
    if ((*it).name == name) return (*it).password;
    }
  return "";
  }

typeOfMonitoring KNSCommonData::findUserUpsmon (const QString name) {
  usersPermList::const_iterator it;

  for (it = m_allCommonData->usersData.begin(); it != m_allCommonData->usersData.end(); it++) {
    if ((*it).name == name) {
      return (*it).upsmon;
      }
    }
  return MON_MASTER;
  }

//*********************************************************************
// ------ Procedures work with monitorUpsList ------------------
//------- QList<monitorUpsStruct> monitorUpsList----------------
//------- pointer is m_allCommonData->monitorData.nutMonitorUp--

void KNSCommonData::monitorUpsDataClean (void) {

  m_allCommonData->monitorData.nutMonitorUps.clear();
  }

monitorUpsList* KNSCommonData::monitorUpsListDataAddr(void){

  return  &m_allCommonData->monitorData.nutMonitorUps;
  }


typeOfMonitoring KNSCommonData::findMonitorUpsmon (const QString upsName, const QString hostName) {
  monitorUpsList::const_iterator it;

  for (it = m_allCommonData->monitorData.nutMonitorUps.begin(); it != m_allCommonData->monitorData.nutMonitorUps.end(); it++) {
    if (((*it).upsName == upsName) && ((*it).hostName == hostName)) {
      return (*it).upsmon;
      }
    }
  return MON_MASTER;
  }


void KNSCommonData::deleteMonitorUpsItem (const QString upsName, const QString hostName) {

  monitorUpsList::iterator it;

  for (it = m_allCommonData->monitorData.nutMonitorUps.begin(); it != m_allCommonData->monitorData.nutMonitorUps.end(); it++) {
    if (((*it).upsName == upsName) && ((*it).hostName == hostName)) {
      m_allCommonData->monitorData.nutMonitorUps.erase(it); // delete item
      break;
      }
    }
  }

bool KNSCommonData::monitorUpsItemExist (const QString upsName, const QString hostName) {
  monitorUpsList::const_iterator it;

  for (it = m_allCommonData->monitorData.nutMonitorUps.begin(); it != m_allCommonData->monitorData.nutMonitorUps.end(); it++) {
    if (((*it).upsName == upsName) && ((*it).hostName == hostName)) return true;
    }
  return false;
  }

//***********************************************************************************

void KNSCommonData::modulesDefClean(void){
  modulesDefList::iterator it;
  paramDefList::iterator itp;

  for (it = m_allCommonData->modulesDef.begin(); it != m_allCommonData->modulesDef.end(); it++) {
    for (itp = (*it).param.begin(); itp != (*it).param.end(); itp++) {
      (*itp).paramEnumItems.clear();
       }
    (*it).param.clear();
    (*it).portNames.clear();
    }
  m_allCommonData->modulesDef.clear();
  }

modulesDefList* KNSCommonData::modulesDefAddr(void){

  return  &m_allCommonData->modulesDef;
  }


bool KNSCommonData::modulesDefExist(const QString name){
  modulesDefList::const_iterator it;

  for (it = m_allCommonData->modulesDef.begin(); it != m_allCommonData->modulesDef.end(); it++)
    if ((*it).name == name) return true;
  return false;
  }

modulesDefList::iterator KNSCommonData::modulesDefFind(const QString name){
  modulesDefList::iterator it;

  for (it = m_allCommonData->modulesDef.begin(); it != m_allCommonData->modulesDef.end(); it++)
    if ((*it).name == name) return it;
  return m_allCommonData->modulesDef.end();
  }



void KNSCommonData::UPSDataClean(void){
  manufactList::iterator it;
  upsModelDriversList::iterator itDriver;

  for ( it = m_allCommonData->manufactListData.begin();it != m_allCommonData->manufactListData.end(); it++) {
    (*it).models.clear();
    }
  m_allCommonData->manufactListData.clear();

  for ( itDriver = m_allCommonData->driverData.begin();itDriver != m_allCommonData->driverData.end(); itDriver++) {
    (*itDriver).paramsList.clear();
    }
  m_allCommonData->driverData.clear();
  }

void KNSCommonData::sortManufacts(void) {
  manufactList::iterator itFirst, itSecond;
  struct manufactItemStruct manufactValue;
  bool notSearched;
  int i;
  int recordsCount;

  recordsCount = m_allCommonData->manufactListData.count();
  if ( recordsCount > 1) {
    notSearched = true;
    recordsCount--;
    while (notSearched) {
      notSearched = false;
      itFirst = m_allCommonData->manufactListData.begin();
      itSecond =itFirst;
      itSecond++;
      for ( i =0; i < recordsCount; i++) {
        if (((*itFirst).manufactName) > ((*itSecond).manufactName)) {
          //change values;
          manufactValue=(*itFirst);
          (*itFirst)=(*itSecond);
          (*itSecond)=manufactValue;
          notSearched = true;
          }

        itFirst=itSecond;
        itSecond++;
        }
      recordsCount--;  
      }

    for ( itFirst = m_allCommonData->manufactListData.begin();itFirst != m_allCommonData->manufactListData.end(); itFirst++) {
      sortModels(&(*itFirst).models);
      }
    }
  }

manufactList::iterator KNSCommonData::findManufact(const QString name){
  manufactList::iterator it;
  for ( it = m_allCommonData->manufactListData.begin();it != m_allCommonData->manufactListData.end(); it++) {
    if ((*it).manufactName == name) return it;
    }
  return m_allCommonData->manufactListData.end();
  }


void KNSCommonData::sortModels (modelsList* models) {
  modelsList::iterator itFirst, itSecond;
  struct modelItemStruct modelValue;
  bool notSearched;
  int i;
  int recordsCount;


  recordsCount = models->count();
  if ( recordsCount > 1) {
    notSearched = true;
    recordsCount--;
    while (notSearched) {
      notSearched = false;
      itFirst = models->begin();
      itSecond =itFirst;
      itSecond++;
      for ( i =0; i < recordsCount; i++) {
        if (((*itFirst).modelName) > ((*itSecond).modelName)) {
          //change values;
          modelValue=(*itFirst);
          (*itFirst)=(*itSecond);
          (*itSecond)=modelValue;
          notSearched = true;
          }
        itFirst=itSecond;
        itSecond++;
        }
      recordsCount--;
      }

    for ( itFirst = models->begin();itFirst != models->end(); itFirst++) {
      sortDrivers(&(*itFirst).drivers);
      }

    }
  }
 

void KNSCommonData::sortDrivers(driversListId* drivers) {

  driversListId::iterator itFirst, itSecond;
  bool notSearched;
  int i;
  int driverId;

  int recordsCount = drivers->count();
  if ( recordsCount > 1) {
    notSearched = true;
    recordsCount--;
    while (notSearched) {
      notSearched = false;
      itFirst = drivers->begin();
      itSecond =itFirst;
      itSecond++;
      for ( i =0; i < recordsCount; i++) {
        if ( m_allCommonData->driverData[(*itFirst)].pref < m_allCommonData->driverData[(*itSecond)].pref) {
        driverId = (*itFirst);
        (*itFirst)=(*itSecond);
        (*itSecond)=driverId;
        notSearched = true;
        }
      itFirst=itSecond;
      itSecond++;
      }
      recordsCount--;  
     }
   }
 }


modelsList::iterator KNSCommonData::findModel(modelsList* models, const QString name){
  modelsList::iterator it;

  for (it = models->begin(); it != models->end(); it++) {
    if ((*it).modelName == name ) return it;
    }
  return models->end();  
  }


int KNSCommonData::addModelDriver (upsModelDriverStruct modelDriver) {

  m_allCommonData->driverData.append(modelDriver);
  return m_allCommonData->driverData.count()-1;
  }
 

void KNSCommonData::addManufactModelDriver (const QString manufactName, const QString modelName, const upsModelDriverStruct newModelDriver){

  modelItemStruct*  modelItemPtr = addManufactModel(manufactName,modelName);

  int modelDriverId = addModelDriver (newModelDriver);
  modelItemPtr->drivers.append(modelDriverId);

 }
 
modelItemStruct* KNSCommonData::addManufactModel(const QString manufactName, const QString modelName){
  struct manufactItemStruct manufactItem;
  struct modelItemStruct modelItem;


  modelItem.modelName=modelName;
  // delete after testing phase
  modelItem.drivers.clear();

  manufactList::iterator manufactIt= findManufact(manufactName);
  if (manufactIt == m_allCommonData->manufactListData.end()) {
    //adds new record for Manufact
    manufactItem.manufactName=manufactName;
    manufactItem.models.clear();

    modelsList::iterator modelIt = manufactItem.models.insert(manufactItem.models.end(),modelItem);  // append of end
    m_allCommonData->manufactListData.append(manufactItem);

    return &(*modelIt);
    }
  else {
   
     modelsList::iterator modelIt = findModel(&(*manufactIt).models,modelName);
     if (modelIt == (*manufactIt).models.end() ) {
      // add new record for model into manufact  
       modelIt = (*manufactIt).models.insert((*manufactIt).models.end(),modelItem);

       return &(*modelIt);
       }
     else return &(*modelIt);
    }
  }



driversListId* KNSCommonData::findDriverList(const QString manufactName, const QString modelName ){

  for ( manufactList::iterator it = m_allCommonData->manufactListData.begin();it != m_allCommonData->manufactListData.end(); it++) {
    if ((*it).manufactName == manufactName) { 
      modelsList* models = &((*it).models);
      for ( modelsList::iterator itModel=  models->begin(); itModel != models->end(); itModel++) {
        if ((*itModel).modelName == modelName ) return &((*itModel).drivers);
        }
      return 0;
      }
    }
  return 0;
  }


QString KNSCommonData::getModelDescription (const QString manufactName, const QString modelName, const int driverId) {

  for ( manufactList::iterator it = m_allCommonData->manufactListData.begin();it != m_allCommonData->manufactListData.end(); it++) {
    if ((*it).manufactName == manufactName) {
      modelsList* models = &((*it).models);
      for ( modelsList::iterator itModel=  models->begin(); itModel != models->end(); itModel++) {
        if ((*itModel).modelName == modelName ) {
           int driverModelId = (*itModel).drivers[driverId];
           return m_allCommonData->driverData[driverModelId].description;
           }
        }
      return QString();
      }
    }
  return QString();
  }


int KNSCommonData::manufactCount (void) {

  return m_allCommonData->manufactListData.count();
  }


QStringList KNSCommonData::getManufactList (void) {
  manufactList::const_iterator it;
  QStringList manufactList;
  
  for (it = m_allCommonData->manufactListData.begin(); it != m_allCommonData->manufactListData.end(); it++) {
    manufactList.push_back((*it).manufactName);
    }
  return manufactList;
  } 



upsModelDriversList* KNSCommonData::driverDataAddr(void){

  return  &m_allCommonData->driverData;
  }

#include "knscommondata.moc"
