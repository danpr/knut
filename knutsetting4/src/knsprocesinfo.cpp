/***************************************************************************
                          knsprocesinfo.cpp  -  description
                             -------------------
    begin                : Ne ?ec 18 2004
    copyright            : (C) 2004 by Daniel Prynych,,,
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

#include "knsprocesinfo.h"

#include <KDE/KMessageBox>
#include <KDE/KLocale>
 

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QRegExp>
#include <QtCore/QTextStream>
#include <QtCore/QDir>
#include <QtCore/QTimer>
#include <QtGui/QWidget>
#include <QtDebug>
#include <QDebug>


#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>



KNSProcesInfo::KNSProcesInfo(KNSCommonData* commonData, QWidget *parent)
             : QObject(parent), m_commonData(commonData) {


  cleanProcesInformations(&upsdProces);
  cleanProcesInformations(&upsmonProces);

  m_procesTimer = new QTimer( this );
  connect( m_procesTimer, SIGNAL(timeout()), this, SLOT(timerProcesSlot()) );


  m_procDir = new QDir ("/proc",QString::null,QDir::Name,QDir::Dirs);

//  parseDir();
  // signals not run, when then emited from constructor or procedures which run from constructor
  //???  WHY ???
  m_procesTimer->setSingleShot (true);
  m_procesTimer->start(5);

  }

KNSProcesInfo::~KNSProcesInfo(){
  }

void KNSProcesInfo::parseDir(void){
  bool upsdFind=false;
  bool upsmonFind=false;



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
//        if (!cmdLine.isEmpty() && cmdLine.contains("/")) {
        if (!cmdLine.isEmpty()) {
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

            upsdFind=true;
            if (!upsdProces.running) {
              upsdProces.running=true;
              upsdProces.pid=(*it).toInt();
              upsdProces.path=cmdPath;
              emit upsdProcesStarted();
              }
            }
          else
            if (cmdName=="upsmon") {
              upsmonFind=true;
             // upsd is found
              if (!upsmonProces.running) {
                upsmonProces.running=true;
                upsmonProces.path=cmdPath;
                upsmonProces.pid=(*it).toInt();
                upsmonProces.childPid = -1;
                emit upsmonProcesStarted();
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
  if ((!upsdFind) && (upsdProces.running)) {
    cleanProcesInformations(&upsdProces);
    emit upsdProcesStoped();
    }
  if ((!upsmonFind) && (upsmonProces.running)) {
    cleanProcesInformations(&upsmonProces);
    emit upsmonProcesStoped();
    }
  }


int KNSProcesInfo::findParentPid (const QString name) {
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

void KNSProcesInfo::cleanProcesInformations (procesInformationsDef* myProcesInfo) {

  myProcesInfo->path="";
  myProcesInfo->running=false;
  myProcesInfo->uid=0;
  myProcesInfo->pid=0;
  myProcesInfo->childPid=-1;
  }



void KNSProcesInfo::timerProcesSlot() {

    m_procesTimer->stop();
    parseDir();
    m_procesTimer->start(5000);
    }

#include "knsprocesinfo.moc"
