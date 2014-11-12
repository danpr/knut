/***************************************************************************
                          knsprocesinfo.h  -  description
                             -------------------
    begin                : Ne ?ec 18 2004
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

#ifndef KNSPROCESINFO_H
#define KNSPROCESINFO_H

#include "knscommondata.h"

#include <QtCore/QObject>

/**
  *@author Daniel Prynych,,,
  * Trida poskytuje informace o bezich procesech upsd a upsmon
  *Class shows informations about running process upsd and upsmon
  */

class QWidget;
class QDir;
class QString;
class QStringList;
class QTimer;

class KNSProcesInfo : public QObject  {
    Q_OBJECT

public:
  KNSProcesInfo(KNSCommonData* commonData, QWidget *parent=0);

  ~KNSProcesInfo();


void upsmonRestart(QString upsmonPath);


  
signals:

/**
 * Tento signal je vyslan pokud byl upsd proces nastartovan.
 * This signal is sended when upsd proces was started
 *
 * @since  0.1
 **/
void upsdProcesStarted (void);

/**
 * Tento signal je vyslan pokud byl upsd proces zastaven.
 * This signal is sended when upsd proces was stoped
 *
 * @since  0.1
 **/
void upsdProcesStoped (void);

/**
 * Tento signal je vyslan pokud byl upsmon proces nastartovan.
 * This signal is sended when upsmon proces was started
 *
 * @since  0.1
 **/
void upsmonProcesStarted(void);


/**
 * Tento signal je vyslan pokud byl upsmon proces zastaven.
 * This signal is sended when upsmon proces was stoped
 *
 * @since  0.1
 **/
void upsmonProcesStoped(void);


protected slots:
  void timerProcesSlot();


protected:
  struct procesInformationsDef {
    QString path;
    bool running;
    int uid;
    int pid;
    int childPid;
    };

  // common data for all classes
  KNSCommonData* m_commonData;

  QDir* m_procDir;

  QTimer* m_procesTimer;


  procesInformationsDef upsdProces;

  procesInformationsDef upsmonProces;

  void parseDir(void);

  void cleanProcesInformations (procesInformationsDef* myProcesInfo);

  int findParentPid (const QString name);


};

#endif
