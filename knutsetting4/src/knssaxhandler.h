/***************************************************************************
                          knssaxhandler.h  -  description
                             -------------------
    begin                : ?t úno 17 2005
    copyright            : (C) 2005 by Daniel Prynych,Zajecov,Czech,
    email                : Daniel at Prynych dot cz
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KNSSAXHANDLER_H
#define KNSSAXHANDLER_H

#include "knscommondata.h"

#include <QtXml/QtXml>

/**
  *@author Daniel Prynych,Zajecov,Czech,
  */

class KNSSaxHandler : public QXmlDefaultHandler  {
public: 
  KNSSaxHandler(KNSCommonData* commonData);
  ~KNSSaxHandler();

  virtual bool characters(const QString &str);

  virtual bool startElement ( const QString&, const QString&, const QString& qName, const QXmlAttributes& attr);

  virtual bool endElement ( const QString&, const QString&, const QString& qName);

private:
  QString m_entityText;

  KNSCommonData* m_commonData;

  modulDefStruc m_newModulDef;  //information about one module
  paramDefStruct m_newParam;

  paramEnumItemStruct m_paramEnumItem;

};

#endif
