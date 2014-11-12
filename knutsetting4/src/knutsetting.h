/***************************************************************************
                          knutsetting.h  -  description
                             -------------------
    begin                : Ne dub  4 20:26:08 CEST 2010
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
#ifndef KNUTSETTING_H_
#define KNUTSETTING_H_

#include "knscommondata.h"
#include "knsacl.h"

#include "knsmodules.h"
#include "knsusers.h"
#include "knsmonitoring.h"
#include "knsnet.h"
#include "knsprocesinfo.h"
#include "knsstring.h"


#include <KCModule>
#include <KAboutData>
#include <KLineEdit>
#include <KComboBox>
#include <KApplication>
#include <KPushButton> 


#include <QtCore/QList>
#include <QtCore/QVector>
#include <QtCore/QTimer>

class QWidget;
class QPixmap;
class QString;
class QTabWidget;
class QLabel;

struct distPathStruct {
  QString name;
  QString conf;
  QString upsd;
  QString upsmon;
  QString upsdrvctl;
  };

/**
  * Hlavni trida
  * Main class
  *
  *@author Daniel Prynych,Zajecov,,
  *since 0.2
  */
class KNutSetting: public KCModule {
  Q_OBJECT
public:
  KNutSetting(QWidget *parent = 0, const QVariantList &args = QVariantList() );


//  explicit KNutSetting(QWidget *parent =0, const QVariantList &args = QVariantList() ); 


  virtual ~KNutSetting();

  void load();
  void save();
  void defaults();
  int buttons();
  QString quickHelp() const;

public slots:
  void configChanged();

  void tabIsChangedSlot(QWidget* activeSlot);


private:

  //structure includes information of paths
  //this structure is completed when user switched from path page to other page

  struct typeOfUpsSetting {
    QString confPath;
    QString serverPath;
    QString monitoringPath;
    QString drivCtrlPath;
     } m_upsSetting;


  // common data for all classes
  KNSCommonData* commonData;

  KAboutData *m_myAboutData;

  QWidget* m_pathTab;
  QWidget* m_accessTab;
  QWidget* m_modulesTab;
  QWidget* m_serverTab;
  QWidget* m_monitorTab;


  KLineEdit* m_upsConfEdit;
  KLineEdit* m_upsDrivCtrlEdit;
  KLineEdit* m_upsServerEdit;
  KLineEdit* m_upsMonitoringEdit;

  QString m_upsConfDefault;
  QString m_upsDrivCtrlDefault;
  QString m_upsServerDefault;
  QString m_upsMonitoringDefault;

  KIcon m_choosePix;
  KIcon m_connectedPix;
  KIcon m_noConnectedPix;
  KIcon m_upsdPix;
  KIcon m_upsMonPix;
  KIcon m_noProcesPix;


  KPushButton* m_restartButton;
  KPushButton* m_restartMonitoringButton;
  QLabel* m_connectPixLabel;
  QLabel* m_upsdPixLabel;
  QLabel* m_upsMonPixLabel;

  KComboBox* m_distComboBox;

  QTabWidget* m_mainTabWidget;

  int m_oldPageIndex;

  KNSNet* m_nutLocalServer;

  QTimer *m_upsdTimer;
  QTimer *m_reconnectTimer;



  KNSProcesInfo* m_procesInfo;

  QVector<distPathStruct> m_distPathVector;

//------------- UPSD.CONF--------------------
  ACDataStruct* m_ACData;
  QStringList m_upsdConfRem;
  bool m_upsdConfIsChanged;
  KNSAcl* m_accessWidget;

//------------- UPSD.USERS--------------------
  usersPermList* m_usersData;
  QStringList m_upsdUsersRem;
  bool m_upsdUserIsChanged;
  KNSUsers* m_serverWidget;

//------------- UPSMON.CONF--------------------
  QStringList m_upsmonConfRem;
  struct monitorDataStruct* m_monitorData;
  bool m_upsmonConfIsChanged;
  KNSMonitoring* m_monitorWidget;

//------------- UPS.CONF--------------------
  KNSModules* m_modulesWidget;
  QStringList m_upsConfRem;
  modulesList* m_modulesData;
  bool m_upsConfIsChanged;

//--------------------------------------


//----------FUNCTIONS----------------------

/**
 * Vytvori zalozku pro nastaveni cest
 * Makes tab for seting paths
 *
 * @since  0.1
 **/
  void initPath(void);

  void testPath(void);


/**
 * Vytvori zalozku pro nastaveni modulu
 * Makes tab for setting modules
 *
 * @since  0.1
 **/
void initModules(void);

  
/**
 * Vytvori zalozku pro nastaveni pristupu a ACL adres
 * Makes tab for setting access and ACL address
 *
 * @since  0.1
 **/
  void initAccess(void);

/**
 * Vytvori zalozku pro nastaveni uzivatelu
 * Makes tab for setting users
 *
 * @since  0.1
 **/
  void initServer(void);

/**
 * Vytvori zalozku pro nastaveni sledovani
 * Makes tab for setting monitoring
 *
 * @since  0.1
 **/
  void initMonitoring(void);


  void saveConfig(void);
  void loadConfig(void);

/**
 * Nahraje konfiguracni soubory do retezcu.
 * Load configuration files to strings.
 *
 **/
  void loadConfFiles (const QString configPath);


/**
 * Ulozi konfiguracni soubory z retezcu.
 * Save configuration files from strings.
 *
 **/
  void saveConfFiles (const QString configPath);
 

 

/**
 * Nahrava data o distribucich (jejich cesty) ze souboru dist.conf
 * Loads data about distributions (their paths) from file dist.conf
 *
 * @since 0.1
 **/
  void loadDistConf(void);


  void initVar(void);

  void parseModXMLFile (void);

  bool findModParItemExist (const QString name, const moduleItems* items);


  void loadData (QString& upsConfString, QString& upsdConfString, QString& upsdUsersString, QString& upsmonConfString);

  void parseUpsConfFile(QString* upsConfString);
  void parseUpsdConfFile(QString* upsdConfString);
  void parseUpsdUsersFile(QString* upsdUsersString);
  void parseUpsmonConfFile(QString* upsmonConfString);


  KNSString findBrackets(KNSString line);

  bool readLine(KNSString& line, QTextStream* upsdStream);


  void saveUpsConfFile(QString* upsConfString);
  void saveUpsdConfFile(QString* upsdConfString);
  void saveUpsdUserFile(QString* upsdUserString);
  void saveUpsmonConfFile(QString* upsmonConfString);

  void setDistribution(void);


private slots:
  void confChangedSlot(const QString &);


  void changedUpsConfSlot(void);
  void changedUpsdConfSlot(void);
  void changedUpsdUserSlot(void);
  void changedUpsmonConfSlot(void);


  void getConfigDirSlot(void);
  void getServerFileSlot(void);
  void getDrivCtrlFileSlot(void);
  void getMonitoringFileSlot(void);

  void setSelectedDistSlot(int index);

  void upsdConectionOKSlot(void);
  void upsdConnectionBrokenSlot(void);
  void upsdConnectionErrorSlot (int );


  void upsdConnectionTimeoutSlot (void);
  void slotReconnectTimeout (void);

  void upsdProcesStartedSlot(void);
  void upsdProcesStopedSlot(void);

  void upsmonProcesStartedSlot(void);
  void upsmonProcesStopedSlot(void);

  void upsdRestartSlot(void);
  void upsmonRestartSlot(void);

  void upsDeletedSlot (const QString upsName);

  void userDeletedSlot (const QString userName);

  void userAddedSlot (const QString userName);

};

#endif
