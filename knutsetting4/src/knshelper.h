/***************************************************************************
                          knshelper.h  -  description
                             -------------------
    begin                : ?t led 1 2012
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

#ifndef KNSHELPER_H
#define KNSHELPER_H

#include <KAuth/Action>
#include <KAuth/ActionReply>
#include <KAuth/ActionWatcher>
#include <KAuth/HelperSupport>


#include <QtCore/QObject>


class QDir;
class QString;
class QStringList;


using namespace KAuth;


class KNSHelper : public QObject
{
    Q_OBJECT    
    
    public slots:
        ActionReply save(const QVariantMap &map);
        ActionReply load(const QVariantMap &map);
        ActionReply exec(const QVariantMap &map);

private:

  struct procesInformationsDef {
    QString path;
    bool running;
    int uid;
    int pid;
    int childPid;
    };



  void parseProcDir(void);

  void cleanProcesInformations (procesInformationsDef* myProcesInfo);

  int findParentPid (const QString name);



/**
 * Zavola program upsdrvctrl pro nastartovani/zastaveni vsech modulu
 * Calls program upsdrvctrl for start/stop of modules.
 *
 * @since  0.1
 **/
  void upsModulesStartStop (QString upsDrvCtlPath, bool start, QStringList &errorMessages, QString upsName = "" );




/**
 * Zavola program upsdrvctrl pro zastaveni vsech modulu.
 * Calls program upsdrvctrl for stop of modules.
 *
 * @param upsDrvCtlPath je cesta k programu upsdrvctl
 * @param upsDrvCtlPath is path to program upsdrvctl
 *
 * @since  0.1
 **/
void upsModulesStop (const QString &upsDrvCtlPath, QStringList &errorMessages);

/**
 * Zavola program upsdrvctrl pro nastartovani vsech modulu
 * Calls program upsdrvctrl for start of modules.
 * driverNames contains all name of drivers // QStringList
 *
 * @since  0.1
 **/
void upsModulesStart (const QString &upsDrvCtlPath ,const QStringList &driverNames, QStringList &errorMessages );


void upsdStop(void);


void upsdRestart(QString upsdPath, QStringList &errorMessages);


/**
 * Posle procesu upsd signal pro nove nacteni konfigurace,
 * pokud upsd nebezi bude spusteno, pokud bezi upsd proces s odlisnou
 * cestou, je ukoncen a spusten novy proces upsd;
 * Send signal to proces upsd for rereading new configuracion,
 * when upsd isn't running, upsd will run, when running upsd proces with
 * different path, one is stoped and new upsd proces will run.
 *
 * @param upsdPath je cesta k programu upsd
 * @param upsdPath is path to program upsd
 *
 * @since  0.1
 **/

void upsmonRestart(const QString upsmonPath, QStringList &errorMessages);

// data

procesInformationsDef upsdProces;
procesInformationsDef upsmonProces;

QDir *m_procDir;
        
};


#endif
