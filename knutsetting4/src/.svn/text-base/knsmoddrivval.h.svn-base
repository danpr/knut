/***************************************************************************
                          knsmoddrivval.h  -  description
                             -------------------
    begin                : Po b?e 14 2005
    copyright            : (C) 2005 by Daniel Prynych,Zajecov,Czech,
    email                : Danieln at prynych dot cz
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KNSMODDRIVVAL_H
#define KNSMODDRIVVAL_H

#include "knscommondata.h"
#include "knsvalueedit.h"


#include <qwidget.h>
#include <QtCore/QVector>

/**
  *@author Daniel Prynych,Zajecov,Czech,
  */

class QString;
class QLabel;
class QHBoxLayout;
class QGridLayout;

class KNSModDrivVal : public QWidget  {
   Q_OBJECT
public: 
  KNSModDrivVal( QWidget *parent=0);
  ~KNSModDrivVal();

/**
 * Smaze vstupni widgety v tomto panelu
 * Clears input widgets in this panel;
 *
 * @since  0.1
 **/
void clear(void);

/**
 * Nastavi vsechny vstupni widgety pro zapis a cteni nebo jen pro cteni.
 * Sets all input widgets for writing and reading or reading only.
 *
 * @since  0.1
 **/
void setReadOnly( const bool readOnly );

/**
 * Nacte data ze vstupnich widgetu.
 * Reads datas from input widgets in this panel.
 *
 * @param moduleIt je iterator seznamu modulu
 * @param moduleIt is iterator from list of modules
 * @param moduleDefIt je iterator seznamu definice modulu
 * @param moduleDefIt is iterator from list of modules of definition
 *
 * @since  0.1
 **/
void putData(modulesDefList::const_iterator moduleDefIt, modulesList::iterator moduleIt);

/**
 * Nastavi vstupni widgety v tomto panelu
 * Sets input widgets in this panel;
 *
 * @param moduleIt je iterator seznamu modulu
 * @param moduleIt is iterator from list of modules
 *
 * @since  0.1
 **/
void getData( modulesList::iterator moduleIt);

private:

  struct valueInfoDef {
    QString name;
    QLabel* label;
    KNSValueEdit* value;
    };

  void init(modulesDefList::const_iterator moduleDefIts);

  QVector<valueInfoDef> m_valueInfoList;

  KNSCommonData* m_commonData;
//  modulesDefList* m_modDefList;

  modulesList* m_modulesValues;

  QGridLayout* m_itemLayout;
  QHBoxLayout* m_gridEnvLayout;

  //modulesDefList::const_iterator m_myModuleDefIt;


  };

#endif
