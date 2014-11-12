/***************************************************************************
                          knsmonvalues.h  -  description
                             -------------------
    begin                : ?t ?íj 21 2004
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

#ifndef KNSMONVALUES_H
#define KNSMONVALUES_H

#include "knscommondata.h"
#include "knsintlineedit.h"


#include <KDE/KLineEdit>
#include <KDE/KNumInput>
#include <KDE/KPushButton>

#include <QtGui/QWidget>

/**
  *@author Daniel Prynych,,,
  */

class QLabel;
class QCheckBox;
class QString;

class KNSMonValues : public QWidget  {
   Q_OBJECT
public: 
  KNSMonValues(KNSCommonData* commonData, QWidget *parent=0);
  ~KNSMonValues();

  void loadItems(void);
  void clear(void);

public slots:
void setStandardSlot (void);


signals:
  void changed (void);

private slots:

  void notifyCmdUsedSlot(void);
  void pollFreqUsedSlot(void);
  void pollAlertFreqUsedSlot(void);
  void rbWarnTimeUsedSlot(void);
  void noCommWarnTimeUsedSlot(void);

  void intValuesChangedSlot(int);
  void stringValuesChangedSlot(const QString &);


private:

  static const int m_minSuppliesMin=1;
  static const int m_pollFreqMin=1;
  static const int m_pollAlertFreqMin=1;
  static const int m_hotSyncMin=1;
  static const int m_deadTimeMin=1;
  static const int m_rbWarnTimeMin=1;
  static const int m_noCommWarnTimeMin=1;
  static const int m_finalDelayMin=1;

/**
 * Inicializuje vstupni widgety.
 * Init input widgets
 *
 * @since  0.1
 **/
  void initValues(void);

  void slotsConnect(void);

  void slotsDisconnect(void);


  monitorValuesStruct* m_monitorValues;

  bool m_enableButton;

  KIntNumInput* m_minSupplies;
  KLineEdit* m_shutDownCmd;
  KLineEdit* m_notifyCmd;
  QCheckBox* m_notifyCmdUsed;
  KIntNumInput* m_pollFreq;
  QCheckBox* m_pollFreqUsed;
  KIntNumInput* m_pollAlertFreq;
  QCheckBox* m_pollAlertFreqUsed;
  KIntNumInput* m_hotSync;
  KIntNumInput* m_deadTime;
  KLineEdit* m_powerDown;
  KIntNumInput* m_rbWarnTime;
  QCheckBox* m_rbWarnTimeUsed;
  KIntNumInput* m_noCommWarnTime;
  QCheckBox* m_noCommWarnTimeUsed;
  KIntNumInput* m_finalDelay;


};

#endif
