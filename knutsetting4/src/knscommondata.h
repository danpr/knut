/***************************************************************************
                          knscommnodata.h  -  description
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
 ***************************************************************************/

#ifndef KNSCOMMONDATA_H
#define KNSCOMMONDATA_H

#include "knsstring.h"


#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtCore/QList>

/**
  *@author Daniel Prynych,,,
  */

class QWidget;
class QString;


enum typeOfUPSPort {
  PORT_UNKNOWN =0,
  PORT_SERIAL =1,
  PORT_USB =2
  };


enum typeOfAutoPort {
  PORT_AUTO_NO = 0, // driver cannot use auto mode
  PORT_AUTO_YES =1, // driver can use auto mode
  PORT_AUTO_ONLY =2, // driver have to !!! use auto mode
  PORT_AUTO_DEF =3 // fro next using
  };

//-------- Modules begin -------------------

struct moduleItemStruct {
  QString itemName;
  bool itemFound;
  QString value;
  };

typedef QList<moduleItemStruct> moduleItems;


struct moduleStruct {
  QString name; // name of mudule
//  QString distName; // distinguish name of description of module, in XML file can be a few descriptions of one module.
  QString driver;
  QString portFileName; // ...ttS0, ...hidups0,
  QString desc;
  bool autoPort;
  int sdOrder;
  bool sdOrderFind;
  bool noLockFind;
  int maxStartDelay;
  bool maxStartDelayFind;
  moduleItems items;
  };


typedef QList<moduleStruct> modulesList;

//------- AC begin --------------------


struct listenAddressPortStruct {
  QString address;
  QString port;
  };

typedef QList<listenAddressPortStruct> listenAddressPortListDef;

// structure contains Access Control -  Data AC Data

struct ACDataStruct {
  listenAddressPortListDef listenAddressPortList;
  int maxAge;
  bool maxAgeFound;
  int maxConn;
  bool maxConnFound;
  };


  static const int maxAgeDef=35;
  static const int maxConnDef=16;


//-------- Users begin -------------------

// data about users, list Users, file upsd.user

enum typeOfMonitoring { MON_SLAVE = 0, MON_MASTER = 1 };

struct userPermStruct {
  QString name;
  bool isUser; // true user; false upsmon
  KNSString password;
  KNSString actions;
  QStringList instcmds;
  typeOfMonitoring upsmon; // 0 = slave 1 = master
  };


typedef QList<userPermStruct> usersPermList;


// data for monitorig, list Monitoring, file upsmon
//-------- Monitoring begin -------------------

struct monitorUpsStruct {
  QString upsName;
  QString hostName; // upsName and hostName is unique index
  int powerValue;
  QString userName;
  QString password;
  typeOfMonitoring upsmon; // 0 = slave 1 = master
  };

typedef QList<monitorUpsStruct> monitorUpsList;


// KNSMonValues
struct monitorValuesStruct {
  int minSupplies;
  bool minSuppliesFound;
  QString shutDownCmd;
  bool shutDownCmdFound;
  QString notifyCmd;
  bool notifyCmdFound;
  int pollFreq;
  bool pollFreqFound;
  int pollAlertFreq;
  bool pollAlertFreqFound;
  int hotSync;
  bool hotSyncFound;
  int deadTime;
  bool deadTimeFound;
  QString powerDown;
  bool powerDownFound;
  int rbWarnTime;
  bool rbWarnTimeFound;
  int noCommWarnTime;
  bool noCommWarnTimeFound;
  int finalDelay;
  bool finalDelayFound;
  };

  static const int minSuppliesDef=1;
  static const char* const shutDownCmdDef = "/sbin/shutdown -h +0";
  static const char* const notifyCmdDef = "";
  static const int pollFreqDef=5;
  static const int pollAlertFreqDef=5;
  static const int hotSyncDef=15;
  static const int deadTimeDef=15;
  static const char* const powerDownDef = "/etc/killpower";
  static const int rbWarnTimeDef=43200;
  static const int noCommWarnTimeDef=300;
  static const int finalDelayDef=5;

  static const int sdOrderDef=0;
  static const int maxStartDelayDef=45;


static const int monNotifyItemsCount = 9;

struct monNotifyItemStruct {
  QString message;
  bool syslogFlag;
  bool wallFlag;
  bool execFlag;
  bool ignoreFlag;
  };

typedef struct monNotifyItemStruct  monNotifyItemsDef[monNotifyItemsCount];


struct monitorDataStruct {
  QString runAsUser;
  bool runAsUserFind;
  monitorUpsList nutMonitorUps;
  struct monitorValuesStruct monitorValues;
  struct monNotifyItemStruct monNotifyItems[monNotifyItemsCount];
  };

//-------- Monitoring end -------------------


//----------- Def of modules---------------

enum typeOfParam {
  PARAM_STRING =0,
  PARAM_NUMBER =1,
  PARAM_ENUM =2
  };

struct paramEnumItemStruct {
  QString value;
  QString text;
  };
  
typedef QList<paramEnumItemStruct> paramEnumItemList;

struct paramDefStruct {
  typeOfParam type;
  QString name;
  bool force;
  bool variable;
  QString parDef;
  int parDefNum;
  int min;
  int max;
  paramEnumItemList paramEnumItems;
  };

typedef QList<paramDefStruct> paramDefList;

// Includes description of driver/module from modules_def apcsmart, bcmxcp_usb, ....
struct modulDefStruc {
  QString name;
  QString distName; // distinguish name of description of module, in XML file can be a few descriptions of one module.
  QString description;  // description of driver
  typeOfUPSPort portType;
  typeOfAutoPort portAuto;
  int portSpeed;
  int portBits;
  QStringList portNames;  // ...ttS0, ...hidups0,
  paramDefList param;
  };

typedef QList<modulDefStruc> modulesDefList;

//----------- Def of modules end---------------


//------------Def of ups start ----------------

struct paramDriverStruct {
  QString paramName;
  QString value;
  };
typedef QList<paramDriverStruct> ParamsDriverList;

// Includes description of ups and attached driver/module from ups_def SMART-UPS 620 - apcsmart, .
struct upsModelDriverStruct {
  QString driverName;
  QString distName;
  int pref;  // preference in line of drivers for same type pf UPS. Some UPSes can use a few drivers.
  int driverMaxStartDelay;
  bool driverMaxStartDelayFound;
  bool driverNoLockFound;
  typeOfUPSPort portType;
  QString description; //Description for model's driver
  ParamsDriverList paramsList;
  };
// LIST of all drivers 
typedef QList<upsModelDriverStruct> upsModelDriversList;
// i don't use vector because iterator of this list is included in modelItemStruct

//----------------- tree list from manufacture->model->driverId

typedef QList<int> driversListId; //includes ordnumber of upsModelDriverStruct from upsModelDriversList

// LIST of Models, every item of list includes LIST of drivers's iterators into list of drivers
struct modelItemStruct {
  QString modelName;
  driversListId drivers;
  };
typedef QList<modelItemStruct> modelsList;


// LIST of Manufacture, every item of list includes LIST of models
struct manufactItemStruct {
  QString manufactName;
  modelsList models;
  };
typedef QList<manufactItemStruct> manufactList;

//------------Def of ups end ----------------

struct allCommonDataStruct {
  modulesList modulesListData;   // list of modules
  ACDataStruct ACData;
  usersPermList usersData;
  monitorDataStruct monitorData;
  modulesDefList modulesDef;     // list of modules/drivers - apcsmart,...
  manufactList manufactListData;   // list of manufact
  upsModelDriversList driverData; // list of models for selected driver/module
  };


class KNSCommonData : public QObject  {
   Q_OBJECT
  public:
    KNSCommonData(QWidget *parent=0);
    ~KNSCommonData();

/**
 * Return address (pointer) on list of modules/drivers (UPS drivers)
 * Navraci adresu (ukazatel) na seznam modules (ovladace pro UPS)
 *
 * @since  0.1
 **/
    modulesList* modulesDataAddr(void);

/**
 * Return address (pointer) on Access Control (AC) data
 * Navraci adresu (ukazatel) na data rizeni pristupu
 *
 * @since  0.1
 **/
    ACDataStruct* ACDataAddr(void);

/**
 * Return address (pointer) on list of Listen addresses a ports
 * Navraci adresu (ukazatel) na seznam naslouchajicich adres a portu 
 *
 * @since  0.2
 **/
    listenAddressPortListDef* ListenAPAddr(void);


    usersPermList* usersDataAddr(void);

    monitorDataStruct* monitorDataAddr(void);
    monitorUpsList* monitorUpsListDataAddr(void);
    monitorValuesStruct* monitorValuesDataAddr(void);
    //returns pointer on the first item of array
    monNotifyItemStruct* monitorNotifyDataAddr(void);


    void monitorDataClean(void);

    void ACDataClean(void);

    void modulesDataClean(void);

    void usersDataClean(void);

/**
 * Returns password for selected name of user
 * Vraci heslo pro vybrane jmeno uzivatele
 *
 * @param name is name od user.
 * @param name je jmeno uzivatele.
 *
 * @since  0.1
 **/
    QString findUserPassword (const QString name);

/**
 * Returns type of monitoring for selected name of user.
 * Vraci zpusob sledovani pro vybrane jmeno uzivatele.
 *
 * @param name is name of user.
 * @param name je jmeno uzivatele.
 *
 * @since  0.1
 **/
    typeOfMonitoring findUserUpsmon (const QString name);


/**
 * Returns type of monitoring for ordinary number of row.
 * Vraci zpusob sledovani pro poradove cislo radky radku.
 *
 * @param upsName is name of ups.
 * @param upsName je jmeno ups.
 * @param hostName is name of pc/device.
 * @param hostName je jmeno pc/zarizeni.
 *
 * @since  0.1
 **/
   typeOfMonitoring findMonitorUpsmon (const QString upsName, const QString  hostName);

/**
 * Return true when ups exist in list of minitoring UPSes
 * Vraci true kdyz ups existuje v seznamu sledovanych upsek 
 * @param upsName is name of ups.
 * @param upsName je jmeno ups.
 * @param hostName is name of pc/device.
 * @param hostName je jmeno pc/zarizeni.
 *
 * @since  0.1
 **/
   bool monitorUpsItemExist (const QString upsName, const QString hostName);


/**
 * Delete usp item/record from list of monitoring UPSes
 * Smaze prvek/zaznam se seznamu sledovanych upsesk
 * @param upsName is name of ups.
 * @param upsName je jmeno ups.
 * @param hostName is name of pc/device.
 * @param hostName je jmeno pc/zarizeni.
 *
 * @since  0.1
 **/
   void deleteMonitorUpsItem (const QString upsName, const QString hostName);



/**
 * Hleda prvek v prvcich modulu pro dany modul
 * Funkce vraci true kdyz je prvek nalezen
 * Finds item of moduleItems structure for module
 * Function returns true when item is found
 *
 * @param moduleName.je jmeno modulu
 * @param moduleName.is name of module
 * @param parName je jmeno parametru v modulu jehoz jmeno je zapsano in paremtru moduleName
 * @param parName is name of parametr in module which name is entered in parametr moduleName
 * @param parItem returns iterator of item
 * @@param parItem navraci iterator prvku
 *
 * @since  0.1
 **/
    bool modulesDataFindPar(const QString moduleName, const QString parName, moduleItems::iterator parItem);


/**
 *
 * Smaze vsechny zaznamy se seznamu modulu
 * Deletes all records from list of modules.
 *
 * @since  0.1
 **/
    void modulesDefClean(void);

/**
 *
 * Vraci adresu seznamu modulu.
 * Returns address of list of modules.
 *
 * @since  0.1
 **/
    modulesDefList* modulesDefAddr(void);

    bool modulesDefExist(const QString name);

    modulesDefList::iterator modulesDefFind(const QString name);


/**
 * Smaze vsechny zaznamy se seznamu UPSek
 * Deletes all records from list of UPSes.
 * @since  0.1
 **/
    void UPSDataClean(void);


/**
 * Vrati pocet zaznamu zaznamy se seznamu UPSek
 * Reurns count of records from list of UPSes.
 * @since  0.1
 **/
    int manufactCount (void);   



/**
 * Vrati vsechny seznam vyrobcu se seznamu UPSek
 *Returns list of manufactores from list of UPSes.
 * @since  0.1
 **/
   QStringList getManufactList (void);



/**
 * Setridi seznam vyrobcu.
 * Sorts list of manufacturers..
 * @since  0.1
 **/
   void sortManufacts(void);


/**
 * Setridi seznam modelu UPSek pro vybraneho vyrobce.
 * Sorts list of UPS's models for selected manufacturer.
 *
 * @param models je ukazatel na seznam modelu UPSek.
 * @param models is pointer to list of UPS's models.
 *
 * @since  0.1
 **/
   void sortModels (modelsList* models);



/**
 * Setridi seznam id driveru UPSek pro vybrany model.
 * Sorts list of id drivers for selected model.
 *
 * @param models je ukazatel na seznam id driveru.
 * @param models is pointer to list of id drivers.
 *
 * @since  0.1
 **/
  void sortDrivers(driversListId* drivers);


/**
 *
 * Vraci iterator na datovou structuru hledaneho vyrobce.
 * Returns iterator to data structure of found manufacturer.
 *
 * @param name je jmeno vyrobce.
 * @param name is name of manufacturer.
 *
 * @since  0.1
 **/
    manufactList::iterator findManufact(const QString name);

    void addManufactModelDriver (const QString manufactName, const QString modelName, const upsModelDriverStruct newModelDriver);

    driversListId* findDriverList (const QString manufactName, const QString modelName);

    QString getModelDescription (const QString manufactName, const QString modelName, const int driverId);

    upsModelDriversList* driverDataAddr(void);

  private:


/**
 * Smaze vsechny zaznamy se seznamu sledovanych UPSek
 * Deletes all records from list of monitoring UPSes.
 * @since  0.1
 **/
    void monitorUpsDataClean (void);

    modelItemStruct* addManufactModel(const QString manufactName, const QString modelName);

    int addModelDriver (upsModelDriverStruct modelDriver);

    modelsList::iterator findModel(modelsList* models, const QString name);


    allCommonDataStruct* m_allCommonData;
};

#endif
