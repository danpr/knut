/***************************************************************************
                          knsmodulesedit.h  -  description
                             -------------------
    begin                : St ?íj 20 2004
    copyright            : (C) 2004 by Daniel Prynych,,,
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

#ifndef KNSMODULESEDIT_H
#define KNSMODULESEDIT_H

#include "knscommondata.h"
#include "knsmodstdval.h"
#include "knsmoddrivval.h"
#include "knsmodusrval.h"


#include <kpushbutton.h>
#include <kcombobox.h>

#include <qlabel.h>
//#include <qwidget.h>
//#include <qtabwidget.h>
//#include <qcheckbox.h>


/**
  *@author Daniel Prynych,,,
  */


class QWidget;
class QTabWidget;
class QCheckBox;

class KNSModulesEdit : public QLabel  {
   Q_OBJECT
public: 
  KNSModulesEdit(KNSCommonData* commonData, QWidget *parent=0);
  ~KNSModulesEdit();

  void setReadOnly (const bool readOnly);

  void putData( modulesList::iterator moduleIt );
  void getData( modulesList::iterator moduleIt );

  void clear( void);

private slots:
  void getNewPortSlot(void);
  void setPortLabelSlot (int index);
  void portAutoUsedSlot(void);

private:


  void init( void);

  modulesDefList::const_iterator findModDefIt (QString driverName);


  KNSCommonData* m_commonData;

  KNSModStdVal* m_standardWidget;
  KNSModDrivVal* m_driverWidget;
  KNSModUsrVal* m_userWidget;

  QLabel* m_portTypeLabel;
  QLabel* m_modelNameLabel;

  KComboBox* m_portDevCombo;
  QLabel* m_portDevLabel;
  QCheckBox* m_portAuto;
  KPushButton* m_portAdd;

  QString m_oldPort;

  typeOfAutoPort m_autoPort;

};

#endif
