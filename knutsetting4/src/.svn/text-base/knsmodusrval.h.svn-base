/***************************************************************************
                          knsmodusrval.h  -  description
                             -------------------
    begin                : Po b?e 14 2005
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

#ifndef KNSMODUSRVAL_H
#define KNSMODUSRVAL_H

#include "knscommondata.h"
#include "knsvalueedit.h"

#include <kpushbutton.h>

//#include <qstring.h>
//#include <qstringlist.h>
//#include <qwidget.h>
//#include <qlayout.h>
//#include <qlabel.h>

#include <QtCore/QVector>



/**
  *@author Daniel Prynych,Zajecov,Czech,
  */

class QString;
class QStringList;
class QLayout;
class QLabel;
class QHBoxLayout;
class QGridLayout;

class KNSModUsrVal : public QWidget  {
   Q_OBJECT
public: 
  KNSModUsrVal(QWidget *parent=0 );

  ~KNSModUsrVal();


  void clear(void);

  void setReadOnly( const bool readOnly );

  void putData(modulesDefList::const_iterator moduleDefIt, modulesList::iterator moduleIt);

  void getData( modulesList::iterator moduleIt);

private slots:
  void addParamSlot( void );

  void addedNewParamSlot(const QString name,const QString value);


private:
  struct valueInfoStruct {
    QString name;
    QLabel* label;
    KNSValueEdit* value;
    };

  void init(modulesDefList::const_iterator moduleDefIt, modulesList::iterator moduleIt);


  QVector<valueInfoStruct> m_valueInfoList;

//  KNSCommonData* m_commonData;
  bool m_enableButton;
  //modulesDefList* m_modDefList;
//  modulesDefList::const_iterator m_myModuleDefIt;

//  modulesList::iterator selectedModuleIt;
  QStringList m_nameParamsList;

//  modulesList* m_modulesValues;

//  QHBoxLayout* m_hLayout;
  QGridLayout* m_itemLayout;
//  QLabel* m_gridWidget;
  QHBoxLayout* m_gridEnvLayout;

  KPushButton* m_addButton;

};

#endif
