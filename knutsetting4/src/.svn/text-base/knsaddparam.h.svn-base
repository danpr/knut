/***************************************************************************
                          knsaddparam.h  -  description
                             -------------------
    begin                : So ?en 25 2005
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

#ifndef KNSADDPARAM_H
#define KNSADDPARAM_H


#include <KDE/KDialog>
#include <KDE/KComboBox>
#include <KDE/KLineEdit>


/**
  *@author Daniel Prynych,Zajecov,Czech,
  */

class QWidget;
class QString;
class QStringList;

class KNSAddParam : public KDialog {
   Q_OBJECT
public: 
  KNSAddParam(QStringList* nameList, QWidget *parent=0);
  ~KNSAddParam();

signals:

void paramAdded (const QString, const QString);

private slots:

void addParamSlot (void);
void nameChangedSlot (const QString&);


private:

KLineEdit* m_nameEdit;
KLineEdit* m_valueEdit;
QStringList* m_nameList;

};

#endif
