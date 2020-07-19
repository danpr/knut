/***************************************************************************
                          knutupsdata.cpp  -  description
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

#include "knutupsdata.h"

#include <QString>
#include <QDebug>

KNutUpsData::KNutUpsData()  {
  qDebug("KNutUpsData::Constructor");	  

  // vytvori seznam UPS
  m_listRecords.clear();
  m_countUpsRecords = 0;
  }

KNutUpsData::~KNutUpsData() { m_listRecords.clear(); }

void KNutUpsData::add (const upsRecordDef upsRecord) {
// vlozime ups na konec
  m_countUpsRecords++;
  m_listRecords.append(upsRecord);
  }


void KNutUpsData::put (const int index, const upsRecordDef upsRecord ) {
  if ((index > -1 ) && (index < m_countUpsRecords)) {
    m_listRecords[index] = (upsRecord);
    }
  }


void KNutUpsData::get (const int index, upsRecordDef& upsRecord ) {
  if ((index > -1 ) && (index < m_countUpsRecords)) upsRecord=m_listRecords[index];
  }

QString KNutUpsData::getName (const int index) {
  if ((index > -1 ) && (index < m_countUpsRecords)) return m_listRecords[index].name;
  else return 0L;
  }


void KNutUpsData::deleteName (const int index) {
  if ((index > -1 ) && (index < m_countUpsRecords)) {
  QList<upsRecordDef>::Iterator it = m_listRecords.begin();
  for (int i =0; i < index; i++) it++;
  m_listRecords.erase(it);
  m_countUpsRecords--;
  }
}


int KNutUpsData::getCount ( void ) { return m_countUpsRecords; }


KNutUpsData::upsRecordDef* KNutUpsData::findName (const QString name) {
  qDebug("KNutUpsData::findName");	  
  QList<upsRecordDef>::Iterator it;
  for (it = m_listRecords.begin(); it != m_listRecords.end(); it++) {
    if ((*it).name == name) {
      return &(*it); // vratime adresu
      }
    }
   return 0l;
  }

