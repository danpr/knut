/***************************************************************************
                          knutupsdata.h  -  description
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

#ifndef KNUTUPSDATA_H
#define KNUTUPSDATA_H

#include "knutconst.h"

#include <QList>

class QString;

/**
  *@author Daniel Prynych
  */


 /**
  * Trida obsahuje data o UPS-kach,
  * nazev, adresu port, protokol, promenne atd.
  * Class includes data about UPSes like name. address, port, protocol, variables ...
  *
  * @author Daniel Prynych
  * @short Information about UPS
  * @version 0.6
  **/
class KNutUpsData {


public:


struct upsRecordDef {  //include information about one UPS
  QString name; // name
  QString upsName; // name of UPSky <> mojeups
  QString upsAddress; // address of UPS <> muj.pc.cz
  int delay;  // time between read date from UPS
  //unsigned short port; // port for connecting server
  quint16 port; // port for connecting server //unsigned 16 bit integer number always
  QString userName;
  QString password;
  bool savePassword; //save password into configuration 
  int upsVarCode[knc::NumberOfVars]; //list of analogs
  };



 /**
  * Konstruktor
  *
  * @since  0.3
  **/
  KNutUpsData();

 /**
  * Destruktor
  *
  * @since  0.3
  **/
  ~KNutUpsData();

 /**
  * Adds record
  * Prida zaznam.
  *
  * @param upsRecord are data of record in structure of upsRecordStruct.
  * @param upsRecord jsou data zaznamu ve strukture upsRecordStruct.
  * @since  0.4
  **/
  void add (const upsRecordDef upsRecord);

 /**
  * Rewrites record
  * Prepise zaznam.
  *
  * @param index Describes order number od record.
  * @param index Udava poradove cislo zaznamu..
  * @param upsRecord are data of record in structure of upsRecordStruct.
  * @param upsRecord jsou data zaznamu ve strukture upsRecordStruct.
  * @since  0.4
  **/
  void put (const int index, const upsRecordDef upsRecord );

 /**
  * Returns record.
  * Vrati zaznam.
  *
  * @param index Describes order number od record.
  * @param index Udava poradove cislo zaznamu.
  * @param upsRecord are data of record in structure upsRecordStruct.
  * @param upsRecord jsou data zaznamu ve strukture upsRecordStruct.
  *
  * @since  0.4
  **/
  void get (const int index, upsRecordDef& upsRecord );

 /**
  * Returns name of record.
  * Vrati jmeno zaznamu.
  *
  * @param index Describes order number od record.
  * @param index Udava poradove cislo zaznamu.
  * @since  0.3
  **/
  QString getName (const int index);

 /**
  * Deletes record;
  * Zrusi zaznam.
  *
  * @param index Describes order number od record.
  * @param index Udava poradove cislo zaznamu.
  *
  * @since  0.3
  **/
  void deleteName (const int index);

 /**
  * Returns count of records
  * Vrati pocet zaznamu.
  *
  * @since  0.3
  **/
  int getCount ( void );

 /**
  * Returns pointer on record, when record is not existed returns 0
  * Vrati ukazatel na zaznam, pokud zaznam neexistuje vrati 0.
  *
  * @param name Is name of list.
  * @param name Je jmeno zaznamu.
  * @since  0.3
  **/
  upsRecordDef* findName (const QString name);



private:
  int m_countUpsRecords;

  QList<upsRecordDef> m_listRecords;
};


#endif
