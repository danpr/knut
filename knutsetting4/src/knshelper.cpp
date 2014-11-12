/***************************************************************************
                          knshelper.cpp  -  description
                             -------------------
    begin                : ?t led 1 2012
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

#include "knshelper.h"

#include <KMessageBox>
#include <KLocale>

#include <KComponentData>
#include <KConfig>
#include <KStandardDirs>
#include <KProcess>

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QFile>

#include <QtCore/QRegExp>
#include <QtCore/QDir>

#include <QDebug>

#include <grp.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>





void KNSHelper::parseProcDir(void){

  cleanProcesInformations(&upsdProces);
  cleanProcesInformations(&upsmonProces);

  QStringList nameList = m_procDir->entryList(QDir::Dirs,QDir::Name);

  for (QStringList::const_iterator it=nameList.begin(); it != nameList.end();it++) {


    if ((*it).indexOf( QRegExp("^[0-9]*$")) == 0) {
      // (*it) is number of proces
      // we found directory for proceses

      QFile cmdLineFile("/proc/"+(*it)+"/cmdline");
      if (cmdLineFile.open(QIODevice::ReadOnly)) {
        QTextStream ts(&cmdLineFile);
        //ts.read reads only first part of cmdline, parts are divide to char zero '\0'

        QString cmdLine=ts.readAll().trimmed().toAscii();
        if (!cmdLine.isEmpty() && cmdLine.contains("/")) {
          // make cmdline
          int pos;
          QString cmdPath;
          QString cmdName;
          if ((pos = cmdLine.lastIndexOf("/")) != -1) {
            cmdPath = cmdLine.left(pos);
            cmdName = cmdLine.right(cmdLine.length()-pos-1);
            }
          else {
           cmdPath = "";
           cmdName = cmdLine;
           }
          if (cmdName == "upsd") {
            // upsd is found
            upsdProces.running=true;
            upsdProces.pid=(*it).toInt();
            upsdProces.path=cmdPath;
            }
          else
            if (cmdName=="upsmon") {
             // upsd is found
              if (!upsmonProces.running) {
                upsmonProces.running=true;
                upsmonProces.path=cmdPath;
                upsmonProces.pid=(*it).toInt();
                upsmonProces.childPid = -1;
                }
              else { //upsmon is running
                if (upsmonProces.childPid == -1) {
                  // finds the second proced
                  if (upsmonProces.pid != (*it).toInt()) {
                  // the second pid is found
                  //sets pid
                  int ppid = findParentPid(QString::number(upsmonProces.pid));
                  int cppid = findParentPid((*it));
                  if (cppid == upsmonProces.pid) upsmonProces.childPid = (*it).toInt();
                  else {
                    if(ppid == (*it).toInt()) {
                      upsmonProces.childPid =upsmonProces.pid;
                      upsmonProces.pid=(*it).toInt();
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }


int KNSHelper::findParentPid (const QString name) {
  QString word;
  int parentPid;

  QFile statFile("/proc/"+name+"/stat");
  if (statFile.open(QIODevice::ReadOnly)) {
    QTextStream ts(&statFile);
    ts >> word; ts >> word; ts >> word;
    ts >> parentPid;
    return parentPid;
    }

  return -1;
  }


void KNSHelper::cleanProcesInformations (procesInformationsDef* myProcesInfo) {

  myProcesInfo->path="";
  myProcesInfo->running=false;
  myProcesInfo->uid=0;
  myProcesInfo->pid=0;
  myProcesInfo->childPid=-1;
  }


void KNSHelper::upsModulesStartStop (QString upsDrvCtlPath, bool start, QStringList &errorMessages, QString upsName ) {
  int cpid;
  char* myarg[5];
  char* myenv[1];
  QByteArray asciiUpsName;

  myarg[0]=(char *)"upsdrvctl";
  // sets params for command line
  if (start) {
    myarg[1]=(char *)"-v";
    myarg[2]=(char *)"start";
    if (upsName.isEmpty()) myarg[3]=NULL;
    else {
      asciiUpsName = upsName.toAscii();
      myarg[3]=(char *) asciiUpsName.constData();
      myarg[4]=NULL;
      }
    }
  else {
    myarg[1]=(char *)"stop";
    myarg[2]=NULL;
    }
  myenv[0]=NULL;

  if (!upsDrvCtlPath.isEmpty()) {
    if ((cpid = fork()) < 0 )
      errorMessages << i18n("I can't run upsdrvctl command");
    else {
      if (cpid == 0) {
          // this is our new proces
        QByteArray asciiUpsDrvCtlPath = (QString(upsDrvCtlPath ).append("/upsdrvctl")).toAscii();
        if (execve (asciiUpsDrvCtlPath.constData(),myarg,myenv) == -1)
           errorMessages << i18n("I can't run upsdrvctl command");
        }
      else {
        // this runs main proces
        int status;
        waitpid (cpid,&status,0);
        }
      }
    }
  }



void KNSHelper::upsModulesStart (const QString &upsDrvCtlPath ,const QStringList &driverNames, QStringList &errorMessages ) {
 // i cannot start dummycons or dummyups driver, see man dummycons

  for (QStringList::const_iterator it = driverNames.constBegin(); it < driverNames.constEnd(); it++) {
      upsModulesStartStop (upsDrvCtlPath, true, errorMessages, (*it) );
    }
  }


void KNSHelper::upsModulesStop (const QString& upsDrvCtlPath, QStringList &errorMessages) {

  upsModulesStartStop (upsDrvCtlPath, false, errorMessages );
  }



void KNSHelper::upsdStop() {

  if (upsdProces.running) {
    if (kill (upsdProces.pid,0) == 0) { //we test to send signal
      kill (upsdProces.pid,SIGTERM);
      }
    }
  }


 

void KNSHelper::upsdRestart(QString upsdPath, QStringList &errorMessages) {
  int cpid;
  char* myarg[2];
  char* myenv[1];

  myarg[0]=(char *)"upsd";
  myarg[1]=NULL;
  myenv[0]=NULL;

  if (upsdProces.running) {

    if (upsdPath == (upsdProces.path)) {
     //we must send signal to upsd proces
      if (kill (upsdProces.pid,0) == 0) { //tests to send signal
        if (kill (upsdProces.pid,SIGHUP) != 0)
           errorMessages << i18n("I can't restart upsd daemon");
        } 
      else errorMessages << i18n("I can't restart upsd daemon");
      }
    else {
      if (!upsdPath.isEmpty()) {
        // we must stop old upsd proces (upsdProces.path)
        if (kill (upsdProces.pid,0) == 0) { //we test to send signal
          kill (upsdProces.pid,SIGTERM);
          }
        // we must start upsd (upsdProces.path)
        if ((cpid = fork()) < 0 )
        errorMessages << 0,i18n("I can't start upsd daemon");
        else {
          if (cpid == 0) {
            // this is our new proces
            QByteArray asciiUpsdPath = (QString(upsdPath ).append("/upsd")).toAscii();
            if (execve (asciiUpsdPath.constData(),myarg,myenv) == -1)
              errorMessages << i18n("I can't start upsd daemon");
            }
          }
        }
      }
    }
  else { // upsd isn't running
      if (!upsdPath.isEmpty()) {
        // we must start upsd (upsdPath)
        if ((cpid = fork()) < 0 ) {
            errorMessages << i18n("I can't start upsd daemon");
            } 
        else {
          if (cpid == 0) {
          // this is our new proces
            QByteArray asciiUpsdPath = (QString(upsdPath ).append("/upsd")).toAscii();
            if (execve (asciiUpsdPath.constData(),myarg,myenv) == -1)
              errorMessages << i18n("I can't start upsd daemon");
            }
          }
        }
      }
  }


void KNSHelper::upsmonRestart(const QString upsmonPath, QStringList &errorMessages) {
  int cpid;
  char* myarg[2];
  char* myenv[1];

  myarg[0]=(char *)"upsmon";
  myarg[1]=NULL;
  myenv[0]=NULL;

  if (upsmonProces.running) {

    if (upsmonPath == (upsmonProces.path)) {
      if (kill (upsmonProces.pid,0) == 0) { //we test to send signal only, exit code 0 indicates if a signal may be sent
        if (kill (upsmonProces.pid,SIGHUP) != 0) {
          errorMessages << i18n("I can't restart upsmon client");
          }
        } 
      else  { 
        errorMessages << i18n("I can't restart upsmon client");
        }
      }

    else {
      if (!upsmonPath.isEmpty()) {
        // we must stop old upsmon proces (upsmonProces.path)
        // must stop two proceses main and slave

        if (kill (upsmonProces.pid,0) == 0) { //we test to send signal , exit code 0 indicates if a signal may be sent
          kill (upsmonProces.pid,SIGTERM);
          }
        if (upsmonProces.childPid != -1) {
          if (kill (upsmonProces.childPid,0) == 0) { //we test to send signal
            kill (upsmonProces.childPid,SIGTERM);
            }
          }
        // we must start upsd (upsdProces.path)
        if ((cpid = fork()) < 0 ) {
          errorMessages << i18n("I can't start upsmon client");
          }
        else {
          if (cpid == 0) {
            // this is our new proces
// error is here

            QByteArray asciiUpsmonPath = (QString(upsmonPath ).append("/upsmon")).toAscii();
            if (execve (asciiUpsmonPath.constData(),myarg,myenv) == -1)
              errorMessages << i18n("I can't start upsmon client");
            }
          }
        }
      }

    }
  else { // upsmon isn't running
    if (!upsmonPath.isEmpty()) {
      // we must start upsmon (upsmonPath)
      if ((cpid = fork()) < 0 ) {
        errorMessages << i18n("I can't start upsmon client");
        }
      else {
        if (cpid == 0) {
        // this is our new proces
          QByteArray asciiUpsmonPath = (QString(upsmonPath ).append("/upsmon")).toAscii();
          if (execve (asciiUpsmonPath.constData(),myarg,myenv) == -1)
            errorMessages << i18n("I can't start upsmon client");
          }
        }
      }
    }
  }




//********************** LOAD **********************************

ActionReply KNSHelper::load(const QVariantMap &args) {    
  KComponentData data( "kcmknutsettinghelper" );  

  ActionReply reply;
  QVariantMap retdata;


  QVariantMap::const_iterator iter = args.constBegin();
  while (iter != args.constEnd()) {
  
  QString filename = args[iter.key()].toString();
  QFile file(filename);


  if (file.exists() ) {
    if (!file.open(QIODevice::ReadOnly)) {


//    reply = ActionReply::HelperErrorReply;
//    reply.setErrorCode(file.error());
//    return reply;
      }
    else
      {


      QTextStream stream(&file);
      QString contents = stream.readAll();
      retdata[iter.key()] = contents;
      }
    }
  else {
    retdata[iter.key()] = "";
    }
    iter++;
  }
  reply.setData(retdata);
  return reply;
}


//*************************** SAVE ******************************

//look out! QVariantMap must be const !!!
ActionReply KNSHelper::save(const QVariantMap &args) {    
  KComponentData data( "kcmknutsettinghelper" );  

  QString drivCtrlPath;
  QStringList drivCtrlData;
  QString drivCtrlPathData;
  QString serverPath;
  QString monitoringPath;
  QStringList errorMessages;


  

  m_procDir = new QDir ("/proc",QString::null,QDir::Name,QDir::Dirs);

  drivCtrlPathData = (args.value("drivCtrlPath")).toString();
  serverPath =  (args.value("serverPath")).toString();
  monitoringPath = (args.value("monitoringPath")).toString(); 


  parseProcDir();

  if (! drivCtrlPathData.isEmpty()) {
    drivCtrlData = drivCtrlPathData.split(" ",QString::SkipEmptyParts);
    if (drivCtrlData.size() > 0) {
      drivCtrlPath = drivCtrlData.takeFirst();
      }
    }

  if (drivCtrlPath.size() > 0) {
    upsModulesStop(drivCtrlPath,errorMessages);
 
   if (! serverPath.isEmpty()) {
       upsdStop();

       }
    }

  QVariantMap::const_iterator iter = args.constBegin();

  while (iter != args.constEnd()) {
 
  if (iter.key().indexOf("Path",-4) == -1) {

    QString filename = iter.key(); //+".test";
    QString fileString = args[iter.key()].toString();

    QFile file(filename);

    bool fileExists = file.exists();

      if (!file.open(QIODevice::WriteOnly)) {
        }
      else
        {
        // save file

        QTextStream stream(&file);
        stream << fileString;
        }

      file.close();
      file.setPermissions (QFile::ReadOwner | QFile::WriteOwner | QFile::ReadGroup);
      
      if (! (fileExists)) {

        struct group *nutGroup;
        nutGroup = getgrnam("nut");
        gid_t gr_gid;
        if (nutGroup != 0L) {
          gr_gid = nutGroup->gr_gid;
          }
        else {
          gr_gid = 0;
          }
 
        chown(filename.toAscii().data(),0,gr_gid);
        }
    }
    iter++;
  }

  // start and restart modules and daemons
  parseProcDir();

  if (drivCtrlPath.size() > 0) {
    upsModulesStart(drivCtrlPath, drivCtrlData,errorMessages);
    }

  if (! serverPath.isEmpty()) {
    upsdRestart(serverPath,errorMessages);
   }

  if (! monitoringPath.isEmpty()) {
   upsmonRestart (monitoringPath,errorMessages);
   }

return ActionReply::SuccessReply;
}

//********************** EXEC ************************* 

//look out! QVariantMap must be const !!!
ActionReply KNSHelper::exec(const QVariantMap &args) {    
  KComponentData data( "kcmknutsettinghelper" );  


  m_procDir = new QDir ("/proc",QString::null,QDir::Name,QDir::Dirs);


  QStringList errorMessages;
  QString serverPath =  (args.value("serverPath")).toString();
  QString monitoringPath = (args.value("monitoringPath")).toString(); 

  parseProcDir();

  if ( ! serverPath.isEmpty()) {
    upsdRestart(serverPath,errorMessages);
   }

  if (! monitoringPath.isEmpty()) {
   upsmonRestart (monitoringPath,errorMessages);
   }

return ActionReply::SuccessReply;
}


KDE4_AUTH_HELPER_MAIN("org.kde.kcontrol.knutsetting", KNSHelper)

