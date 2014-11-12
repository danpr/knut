/***************************************************************************
                          knsupssaxhandler.h  -  description
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

#ifndef KNSUPSSAXHANDLER_H
#define KNSUPSSAXHANDLER_H

#include "knscommondata.h"

#include <QtXml/QtXml>


/**
  *@author Daniel Prynych,Zajecov,Czech,
  */

class KNSUPSSaxHandler : public QXmlDefaultHandler  {
public: 
  KNSUPSSaxHandler(KNSCommonData* commonData);
  ~KNSUPSSaxHandler();


  virtual bool characters(const QString &str);

  virtual bool startElement ( const QString&, const QString&, const QString& qName, const QXmlAttributes& attr);

  virtual bool endElement ( const QString&, const QString&, const QString& qName);


private:
  QString m_entityText;
  QString m_driverName;
  QString m_driverPort;
  QString m_driverDesc;
  int m_pref;
  int m_driverMaxStartDelay;
  bool m_driverMaxStartDelayFound;
  bool m_driverNoLockFound;
  KNSCommonData* m_commonData;

  QStringList m_manufactList;
  QStringList m_modelList;
  ParamsDriverList m_paramsList;

};

#endif
