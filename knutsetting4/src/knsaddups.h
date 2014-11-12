/***************************************************************************
                          knsaddups.h  -  description
                             -------------------
    begin                : ?t b?e 31 2005
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

#ifndef KNSADDUPS_H
#define KNSADDUPS_H

#include "knscommondata.h"


#include <KDE/KDialog>
#include <KDE/KComboBox>
#include <KDE/KPushButton>
#include <KDE/KLineEdit>
#include <KDE/KTextEdit>



/**
  *@author Daniel Prynych,Zajecov,Czech,
  */

class QWidget;
class QLabel;
class QListWidget;
class QListWidgetItem;
class QString;
class QCheckBox;

class KNSAddUPS : public KDialog  {
   Q_OBJECT
public: 
  KNSAddUPS(KNSCommonData* commonData, QWidget *parent=0);
  ~KNSAddUPS();

signals:

void upsAdded (QString);

private slots:

void manufactChangedSlot (void);

void modelChangedSlot (void);

void driverSelectedSlot (int index);

void getNewPortSlot(void);

void autoToggledSlot (bool on);

void nameChangedSlot (const QString&);

void addModuleSlot (void);




private:

  KNSCommonData* m_commonData;
  modulesList* m_modulesData;

  KComboBox* m_driverCombo;  
  QLabel* m_connectLabel;
  KTextEdit* m_desc;

  QListWidget* m_manufactBox;
  QListWidget* m_modelBox;

  QCheckBox* m_portAuto;
  KComboBox* m_portDevCombo;
  KPushButton* m_portAdd;
  KLineEdit* m_nameLineEdit;

  QListWidgetItem* m_selectedManufactItem;

  driversListId* m_driverSeletedList; // list of id for selected model
  int m_driverCurrentId; // selected driver for selected model
  
  bool m_portAutoOnly; // if true - driver support only auto mode


void initUPSData (void);

void showManufact (void);


//naplni seznam modulu (modulesDefAddr z KNSCommonDate) popisem modulu z XML File    
void parseUPSXMLFile (void);

void disableDriverPart (void);

void clearDriverPart (void);

void setButtonOK (void);


};

#endif
