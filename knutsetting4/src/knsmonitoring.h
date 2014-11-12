/***************************************************************************
                          knsmonitoring.h  -  description
                             -------------------
    begin                : Út ?íj 5 2004
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

#ifndef KNSMONITORING_H
#define KNSMONITORING_H

#include "knscommondata.h"
#include "knsmonvalues.h"
#include "knsmonnotify.h"
#include "knsmonsystems.h"

#include <klineedit.h> 
#include <kpushbutton.h>

//#include <qwidget.h>
#include <qlabel.h>
//#include <qtabwidget.h>



/**
  *@author Daniel Prynych,,,
  */

class QWidget;
class QTabWidget;

class KNSMonitoring : public QLabel  {
   Q_OBJECT
public: 
  KNSMonitoring(KNSCommonData* commonData, QWidget *parent=0);
  ~KNSMonitoring();

/**
 * Nastavi vsechny vstupni widgety teto tridy
 * Sets all input widgets of this class
 *
 * @since  0.1
 **/
  void loadItems( void );

/**
 * Smaze zaznam lokalni upsky z monitorovaciho seznamu
 * Delete ups's record of localhost from list of monitoring
 *
 * @since  0.1
 **/
  void deleteUpsMonitoring( const QString upsName );


/**
 * Smaze zaznamy ktere maji stejne jmeno jako userName
 * Delete records which the same name like userName
 *
 * @since  0.1
 **/
  void deleteUserMonitoring( const QString userName );


  void addUserMonitoring( const QString userName );

public slots:

/**
 * Nastavy standartni hodnotu do widgetu grafickeho rozhrani a posle signal changed
 * Sets standars value to GUI widgets and send signal changed
 *
 * @since  0.1
 **/
  void setStandardSlot (void);


/**
 * Nastavi heslo pro vybraneho uzivatele
 * Sets password for selected user
 *
 * @param name is name od user.
 * @param name je jmeno uzivatele.
 * @param password is password of user.
 * @param password je heslo uzivatele.
 *
 * @since  0.1
 **/
  void changePasswordSlot(const QString userName, const QString password);

/**
 * Nastavi zpusob sledovani pro vybraneho uzivatele
 * Sets type of monitoring for selected user
 *
 * @param name is name od user.
 * @param name je jmeno uzivatele.
 * @param typeMen is user's type of monitoring.
 * @param typeMen je zpusob sledovani uzivatele.
 *
 * @since  0.1
 **/
  void changeMonitoringSlot(const QString userName, const typeOfMonitoring typeMon);


/**
 * Odtrani zaznamy ktere pouzivaji zadaneho uzivatel, when typ uzivatele neni sledovani
 * Deletes records, which use selected user, when type of user isn't monitoring
 *
 * @param name is name od user.
 * @param name je jmeno uzivatele.
 * @param isUser is type of selected user /true - user, false - monitoring
 * @param isUser je typ vybraneho uzivatele /true - uzivatel, false - sledovani
 *
 * @since  0.1
 **/
  void typeUserChangeSlot(const QString userName, const bool isUser);


signals:
  void changed (void);

private slots:
  void dataChangedSlot (void);


private:

//  QTabWidget* m_monitorTab;

//bool enableButton;

//  KPushButton* m_standardButton;


  KNSMonValues* m_itemsWidget;
  KNSMonNotify* m_notifyWidget;
  KNSMonSystems* m_systemWidget;

  QWidget* m_upsWidget;

//  KNSCommonData* m_commonData;

  };

#endif
