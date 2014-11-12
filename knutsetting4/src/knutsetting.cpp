/***************************************************************************
                          knutsetting.cpp  -  description
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

#include "knutsetting.h"
#include "knssaxhandler.h"


#include <KDE/KGlobal>
#include <KDE/KLocale>
#include <KDE/KIconLoader>

#include <KDE/KPushButton>
#include <KFileDialog>
#include <KDE/KMessageBox>
#include <KDE/KConfig> 
#include <KDE/KStandardDirs>

#include <KDE/KGenericFactory>
#include <KDE/KPluginFactory>

#include <KDE/KAuth/HelperSupport>
#include <KDE/KAuth/Action>
#include <KDE/KAuth/ActionReply>
#include <KDE/KAuth/ActionWatcher>

#include <QtGui/QWidget>
#include <QtGui/QLayout>
#include <QtGui/QPixmap>
#include <QtCore/QString>
#include <QtGui/QTabWidget>
#include <QtGui/QLabel>
#include <QtGui/QFrame>
#include <QtCore/QTextStream> 
#include <QStringList>
#include <QtXml/QtXml>
#include <QtGui/QToolTip>
#include <QCloseEvent>

#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>


#include <QDebug>


//static int distCountOld = 11;

//static QString distPathOld[12][5] = {{I18N_NOOP("None/Unknown"),"","",""},
//                   {"Debian 3.1 Sarge","/etc/nut","/sbin/upsd","/sbin/upsmon","/sbin/upsdrvctl"},
//                   {"Mandrive 2005","/etc/ups","/usr/sbin/upsd","/usr/sbin/upsmon","/usr/bin/upsdrvctl"},
//                   {"Aurox 10.2","/etc/ups","/usr/sbin/upsd","/usr/sbin/upsmon","/sbin/upsdrvctl"},
//                   {"Fedora Core 4","/etc/ups","/usr/sbin/upsd","/usr/sbin/upsmon","/sbin/upsdrvctl"},
//                   {"Fedora Core 3","/etc/ups","/usr/sbin/upsd","/usr/sbin/upsmon","/sbin/upsdrvctl"},
//                   {"Suse Enterprise server 9","/etc/ups","/usr/sbin/upsd","/usr/sbin/upsmon","/usr/lib/ups/driver/upsdrvctl"},
//                   {"RedHat","/etc/ups","/usr/sbin/upsd","/usr/sbin/upsmon","/usr/bin/upsdrvctl"},
//                   {"Mandrake","/etc/ups","/usr/sbin/upsd","/usr/sbin/upsmon","/usr/bin/upsdrvctl"},
//                   {"Compiled - /usr","/etc/ups","/usr/sbin/upsd","/usr/sbin/upsmon","/usr/bin/upsdrvctl"},
//                   {"Compiled - /usr/local","/usr/local/etc","/usr/local/sbin/upsd","/usr/local/sbin/upsmon","/usr/local/bin/upsdrvctl"},
//                   {"Compiled - /usr/local/ups","/usr/local/ups/etc","/usr/local/ups/sbin/upsd","/usr/local/ups/sbin/upsmon","/usr/local/ups/bin/upsdrvctl"}};


// ********** K_PLUGIN_FACTORY_DECLARATION(KcmKNutSettingFactory);


#include <KDE/KGenericFactory>
#include <KDE/KPluginFactory>


 K_PLUGIN_FACTORY(KNutSettingFactory, registerPlugin<KNutSetting>(); )
 K_EXPORT_PLUGIN(KNutSettingFactory("knutsetting","knutsetting") )



//K_PLUGIN_FACTORY(KNutSettingFactory, registerPlugin<KNutSetting>();)
//K_EXPORT_PLUGIN(KNutSettingFactory("knutsetting",knutsetting))


KNutSetting::KNutSetting(QWidget *parent, const QVariantList &)
             : KCModule( KNutSettingFactory::componentData(), parent ) {


   KAboutData *about = new KAboutData(
     "knutsetting", 0, ki18n( "KNutSetting Version 0.2 Hossk" ),
     "0.2", KLocalizedString(ki18n("Program for setting UPS system NUT")), KAboutData::License_GPL,
     ki18n( "Copyright 2011 Daniel Prynych" ) );
   about->addAuthor( ki18n("Daniel Prynych") );
   setAboutData( about );

  setNeedsAuthorization(true);

  m_oldPageIndex=-1;


  commonData = new KNSCommonData();


  //UserIcon don't work, we have to use path to file of icon

  m_connectedPix = KIcon(QIcon(KGlobal::dirs()->findResource("data","knutsetting/pics/kns_conn.png")));
  m_noConnectedPix = KIcon(QIcon(KGlobal::dirs()->findResource("data","knutsetting/pics/kns_error.png")));
  m_upsdPix = KIcon(QIcon(KGlobal::dirs()->findResource("data","knutsetting/pics/kns_main.png")));
  m_upsMonPix = KIcon(QIcon(KGlobal::dirs()->findResource("data","knutsetting/pics/kns_mon.png")));
  m_noProcesPix = KIcon(QIcon(KGlobal::dirs()->findResource("data","knutsetting/pics/kns_error.png"))); 
  m_choosePix =  KIcon(QIcon(KGlobal::dirs()->findResource("data","knutsetting/pics/kns_conn.png ")));



  initVar();
  // places widgets here
  QVBoxLayout* setTopLayout = new QVBoxLayout( this );
  setTopLayout->setMargin(0);
  setTopLayout->setSpacing(0);

  m_mainTabWidget = new QTabWidget(this);

  setTopLayout->addWidget(m_mainTabWidget);
  QHBoxLayout* connectLayout = new QHBoxLayout();
  setTopLayout->addLayout(connectLayout);

  m_modulesTab = new QWidget(m_mainTabWidget);
  m_accessTab = new QWidget(m_mainTabWidget);
  m_serverTab = new QWidget(m_mainTabWidget);
  m_monitorTab = new QWidget(m_mainTabWidget);
  m_pathTab = new QWidget(m_mainTabWidget);

  m_mainTabWidget->addTab(m_modulesTab,i18n("Modules"));
  m_mainTabWidget->addTab(m_accessTab,i18n("Accesses"));
  m_mainTabWidget->addTab(m_serverTab,i18n("Users"));
  m_mainTabWidget->addTab(m_monitorTab,i18n("Monitoring"));
  m_mainTabWidget->addTab(m_pathTab,i18n("Paths"));

  // setting for bottom row
  connectLayout->addSpacing(5);
  m_connectPixLabel = new QLabel(this);
  connectLayout->addWidget(m_connectPixLabel);
  connectLayout->addSpacing(5);
  m_upsdPixLabel = new QLabel(this);
  connectLayout->addWidget(m_upsdPixLabel);
  connectLayout->addSpacing(5);
  m_upsMonPixLabel = new QLabel(this);
  connectLayout->addWidget(m_upsMonPixLabel);
  connectLayout->addSpacing(5);

  m_upsdPixLabel->setToolTip(i18n("UPS daemon"));
  m_upsMonPixLabel->setToolTip(i18n("UPS monitoring"));
  m_connectPixLabel->setToolTip(i18n("UPS connection"));

  m_connectPixLabel->setPixmap(m_noConnectedPix.pixmap(16,16,QIcon::Normal,QIcon::On));
  m_upsdPixLabel->setPixmap(m_noProcesPix.pixmap(16,16,QIcon::Normal,QIcon::On));
  m_upsMonPixLabel->setPixmap(m_noProcesPix.pixmap(16,16,QIcon::Normal,QIcon::On));

  m_restartButton = new KPushButton(i18n("Start/Restart UPSD"),this);
  m_restartButton->setToolTip(i18n("Restarts UPS Daemon"));
  connectLayout->addWidget(m_restartButton);
  connectLayout->addSpacing(5);

  m_restartMonitoringButton = new KPushButton(i18n("Start/Restart UPS monitoring"),this);
  m_restartMonitoringButton->setToolTip(i18n("Restarts UPS Monitoring Daemon"));
  connectLayout->addWidget(m_restartMonitoringButton);


  connectLayout->addStretch(20);


// init parts

  initPath();

  initModules();
  initAccess();
  initServer();
  initMonitoring();

  loadConfig(); //sets m_upsSetting.confPath  // load confih data from knutsettingrc file

// load description of modules from xml file to list
  parseModXMLFile();

  m_procesInfo = new KNSProcesInfo(commonData,this);
  connect (m_procesInfo,SIGNAL(upsdProcesStarted()),this,SLOT(upsdProcesStartedSlot()));   // when prgram upsd is started
  connect (m_procesInfo,SIGNAL(upsdProcesStoped()),this,SLOT(upsdProcesStopedSlot()));     // when prgram upsd is stopped
  connect (m_procesInfo,SIGNAL(upsmonProcesStarted()),this,SLOT(upsmonProcesStartedSlot()));   // when prgram upsd is started
  connect (m_procesInfo,SIGNAL(upsmonProcesStoped()),this,SLOT(upsmonProcesStopedSlot()));     // when prgram upsd is stopped

  
  m_upsdTimer = new QTimer (this);
  m_upsdTimer->setSingleShot( TRUE );
  connect( m_upsdTimer, SIGNAL(timeout()), SLOT(upsdConnectionTimeoutSlot()) );

  m_reconnectTimer = new QTimer(this);
  connect( m_reconnectTimer, SIGNAL(timeout()), SLOT(slotReconnectTimeout()) );

//  upsd connection
  m_nutLocalServer = new KNSNet("localhost","",3493);
  connect (m_nutLocalServer,SIGNAL(firstConnection()),this,SLOT(upsdConectionOKSlot()));
  connect (m_nutLocalServer,SIGNAL(connectionClosed()),this,SLOT(upsdConnectionBrokenSlot()));
  connect (m_nutLocalServer,SIGNAL(connectionError(int)),this, SLOT(upsdConnectionErrorSlot(int)));
  
  m_nutLocalServer->open();

// ups's item is deleted
  connect (m_modulesWidget,SIGNAL(upsDeleted(const QString)),this,SLOT(upsDeletedSlot(const QString)));
// user's item is deleted


  connect (m_serverWidget,SIGNAL(userDeleted(const QString)),this,SLOT(userDeletedSlot(const QString)));
// user's item is added
  connect (m_serverWidget,SIGNAL(userAdded(const QString)),this,SLOT(userAddedSlot(const QString)));
// password for user's item was changed
  connect (m_serverWidget,SIGNAL(passwordChanged(const QString, const QString)),m_monitorWidget,SLOT(changePasswordSlot(const QString,const QString)));
// type of monitoring for user's item was changed
  connect (m_serverWidget,SIGNAL(monitoringChanged(const QString, const typeOfMonitoring)),m_monitorWidget,SLOT(changeMonitoringSlot(const QString,const typeOfMonitoring)));
// type of user for user's item was changed
  connect (m_serverWidget,SIGNAL(typeUserChanged(const QString, const bool)),m_monitorWidget,SLOT(typeUserChangeSlot(const QString,const bool)));


  m_modulesWidget->loadItems();
  m_accessWidget->loadItems();
  m_serverWidget->loadItems();
  m_monitorWidget->loadItems();

  connect (m_restartButton,SIGNAL(clicked()),this,SLOT(upsdRestartSlot()));   // clicks on button 
  connect (m_restartMonitoringButton,SIGNAL(clicked()),this,SLOT(upsmonRestartSlot()));  // clicks on button
};





KNutSetting::~KNutSetting() {

  m_distComboBox->clear();
  //---------- modules -----------
  m_upsConfRem.clear();

  //--------- Access-----------
  m_upsdConfRem.clear();

  //-------- server-----------  
  m_upsdUsersRem.clear();

  //----------- monitoring -------------
  m_upsmonConfRem.clear();

  delete m_nutLocalServer;
  delete commonData;
  m_distPathVector.clear();
  }



void KNutSetting::initModules(void) {

  QVBoxLayout* settingTopLayout = new QVBoxLayout(m_modulesTab);
  m_modulesWidget = new KNSModules(commonData, m_modulesTab);
  settingTopLayout->addWidget(m_modulesWidget);
  connect (m_modulesWidget,SIGNAL(changed()),this,SLOT(changedUpsConfSlot()));
  }



void KNutSetting::initAccess(void) {

  QVBoxLayout* settingTopLayout = new QVBoxLayout(m_accessTab );
  settingTopLayout->setMargin(2);
  settingTopLayout->setSpacing(0);

  m_accessWidget = new KNSAcl(commonData, m_accessTab);
  settingTopLayout->addWidget(m_accessWidget);
  connect (m_accessWidget,SIGNAL(changed()),this,SLOT(changedUpsdConfSlot()));
  }


void KNutSetting::initServer(void) {

  QVBoxLayout* settingTopLayout = new QVBoxLayout(m_serverTab);
  settingTopLayout->setMargin(0);
  settingTopLayout->setSpacing(0);


  m_serverWidget = new KNSUsers(commonData,  m_serverTab);
  settingTopLayout->addWidget(m_serverWidget);
  connect (m_serverWidget,SIGNAL(changed()),this,SLOT(changedUpsdUserSlot()));
  }

void KNutSetting::initMonitoring(void) {

  QVBoxLayout* settingTopLayout = new QVBoxLayout(m_monitorTab);
  settingTopLayout->setMargin(0);
  settingTopLayout->setSpacing(0);

  m_monitorWidget = new KNSMonitoring(commonData, m_monitorTab);
  settingTopLayout->addWidget(m_monitorWidget);
  connect (m_monitorWidget,SIGNAL(changed()),this,SLOT(changedUpsmonConfSlot()));
  }


void KNutSetting::initPath(void) {
  QVector<distPathStruct>::const_iterator it;

  // makeing layout
  QVBoxLayout* pathMainLayout = new QVBoxLayout(m_pathTab);
  pathMainLayout->setMargin(10);
  pathMainLayout->setSpacing(10);

  QGridLayout* pathLayout = new QGridLayout();
  QHBoxLayout *chooseDistLayout = new QHBoxLayout();


  //makeing items
  QLabel* upsConfLabel = new QLabel (i18n ("Config directory") +" :",m_pathTab);
  m_upsConfEdit = new KLineEdit (m_pathTab);
  KPushButton* upsConfChoose = new KPushButton (m_pathTab);
  QLabel* upsServerLabel = new QLabel (i18n ("UPS information server path (upsd)")+" :",m_pathTab);
  m_upsServerEdit = new KLineEdit (m_pathTab);
  KPushButton* upsServerChoose = new KPushButton (m_pathTab);
  QLabel* upsMonitoringLabel = new QLabel (i18n ("UPS monitoring client path (upsmon)")+" :",m_pathTab);
  m_upsMonitoringEdit = new KLineEdit (m_pathTab);
  KPushButton* upsMonitoringChoose = new KPushButton (m_pathTab);
  QLabel* usbDrivCtrlLabel = new QLabel (i18n ("UPS driver controler path (upsdrvctl)")+" :",m_pathTab);
  m_upsDrivCtrlEdit = new KLineEdit (m_pathTab);
  KPushButton* upsDrivCtrlChoose = new KPushButton (m_pathTab);

  QFrame *pathLine = new QFrame(m_pathTab);

  QLabel* chooseDistLabel = new QLabel (i18n("Selected distribution")+" :",m_pathTab);
  m_distComboBox = new KComboBox(m_pathTab);

  // set Layouts
  //pathMainLayout
  pathMainLayout->addLayout(pathLayout);
  pathMainLayout->addWidget(pathLine);
  pathMainLayout->addLayout(chooseDistLayout);
  pathMainLayout->addStretch ( 20 );
  
  pathLayout->addWidget(upsConfLabel,0,0);
  pathLayout->addWidget(m_upsConfEdit,0,1);
  pathLayout->addWidget(upsConfChoose,0,2);
  pathLayout->addWidget(upsServerLabel,1,0);
  pathLayout->addWidget(m_upsServerEdit,1,1);
  pathLayout->addWidget(upsServerChoose,1,2);
  pathLayout->addWidget(upsMonitoringLabel,2,0);
  pathLayout->addWidget(m_upsMonitoringEdit,2,1);
  pathLayout->addWidget(upsMonitoringChoose,2,2);
  pathLayout->addWidget(usbDrivCtrlLabel,3,0);
  pathLayout->addWidget(m_upsDrivCtrlEdit,3,1);
  pathLayout->addWidget(upsDrivCtrlChoose,3,2);

  chooseDistLayout->addWidget(chooseDistLabel);
  chooseDistLayout->addWidget(m_distComboBox);

  //setting items

  upsConfChoose->setIcon (m_choosePix);
  connect ( upsConfChoose, SIGNAL(released()), this, SLOT(getConfigDirSlot()));
  connect ( m_upsConfEdit, SIGNAL (textChanged(const QString &)), SLOT(confChangedSlot(const QString &)));

  upsServerChoose->setIcon (m_choosePix);
  connect ( upsServerChoose, SIGNAL(released()), this, SLOT(getServerFileSlot()));
  connect ( m_upsServerEdit, SIGNAL (textChanged(const QString &)), SLOT(confChangedSlot(const QString &)));

  upsMonitoringChoose->setIcon (m_choosePix);
  connect ( upsMonitoringChoose, SIGNAL(released()), this, SLOT(getMonitoringFileSlot()));
  connect ( m_upsMonitoringEdit, SIGNAL (textChanged(const QString &)), SLOT(confChangedSlot(const QString &)));

  upsDrivCtrlChoose->setIcon (m_choosePix);
  connect ( upsDrivCtrlChoose, SIGNAL(released()), this, SLOT(getDrivCtrlFileSlot()));
  connect ( m_upsDrivCtrlEdit, SIGNAL (textChanged(const QString &)), SLOT(confChangedSlot(const QString &)));

  pathLine->setFrameShape(QFrame::HLine);

  m_distComboBox->clear();

  loadDistConf();

  for (it=m_distPathVector.begin(); it != m_distPathVector.end(); ++it)
    m_distComboBox->addItem((*it).name);
   
  connect (m_distComboBox,SIGNAL(activated(int)),this,SLOT(setSelectedDistSlot(int)));
  connect (m_mainTabWidget,SIGNAL(currentChanged(QWidget*)),this,SLOT(tabIsChangedSlot(QWidget*)));
  }


void KNutSetting::load() {
  // insert your loading code here...
  loadConfig();  
  loadConfFiles(m_upsSetting.confPath);
  }


void KNutSetting::loadConfFiles (const QString configPath) {
  KAuth::Action readAction("org.kde.kcontrol.knutsetting.load");
  readAction.setHelperID("org.kde.kcontrol.knutsetting");

  QVariantMap args;
  args["ups.conf"] =configPath + "ups.conf";
  args["upsd.conf"] =configPath + "upsd.conf";
  args["upsd.users"] = configPath + "upsd.users";
  args["upsmon.conf"] = configPath + "upsmon.conf";

  readAction.setArguments(args);


  KAuth::ActionReply reply = readAction.execute();

  if (reply.failed()) {
    if (reply.type() == KAuth::ActionReply::KAuthError) {
      KMessageBox::error(this, i18n("Unable to authenticate/execute the action: %1, %2", reply.errorCode(), reply.errorDescription()));
      }
    }
  else {
//    testPath();
    QVariantMap argsRet = reply.data();
    QString upsConfString = argsRet["ups.conf"].toString();
    QString upsdConfString = argsRet["upsd.conf"].toString();
    QString upsdUsersString = argsRet["upsd.users"].toString();
    QString upsmonConfString = argsRet["upsmon.conf"].toString();
    loadData(upsConfString,upsdConfString,upsdUsersString,upsmonConfString);
    }

  }



void KNutSetting::defaults() {
  // insert your default settings code here...

  m_accessWidget->setStandardSlot();
  m_serverWidget->setStandardSlot();
  m_monitorWidget->setStandardSlot();
  }



void KNutSetting::save() {

  // save config informations file is knutsettingrc
saveConfig();
  // save config files
saveConfFiles(m_upsSetting.confPath);

}


void KNutSetting::saveConfFiles(const QString configPath) {

  KAuth::Action saveAction("org.kde.kcontrol.knutsetting.save");
  saveAction.setHelperID("org.kde.kcontrol.knutsetting");

  QVariantMap args;

  // sets drivCtrlPath

  QString drivCtrlPathString;
  QString serverPathString;
  QString monitoringPathString;

  if (m_upsSetting.drivCtrlPath.contains("/")) {
    drivCtrlPathString = m_upsSetting.drivCtrlPath.section("/upsdrvctl",0,0);
    }
  else {
    drivCtrlPathString = m_upsSetting.drivCtrlPath;
   }

  modulesList* modulesData=commonData->modulesDataAddr();
  for (modulesList::iterator it = modulesData->begin(); it !=modulesData->end(); it++) {
    if (((*it).driver != "dummycons") && ((*it).driver != "dummyups")) {
      drivCtrlPathString += " ";
      drivCtrlPathString +=  (*it).name;
      }
    }


  if (m_upsSetting.serverPath.contains("/")) {
    serverPathString = m_upsSetting.serverPath.section("/upsd",0,0);
    }
  else {
    serverPathString = m_upsSetting.serverPath;
   }


  if (m_upsSetting.monitoringPath.contains("/")) {
    monitoringPathString = m_upsSetting.monitoringPath.section("/upsmon",0,0);
    }
  else {
    monitoringPathString = m_upsSetting.monitoringPath;
   }



    QString upsConfString;
    QString upsdConfString;
    QString upsdUsersString;
    QString upsmonConfString;

    if (m_upsConfIsChanged) {


      saveUpsConfFile(&upsConfString); // generate new upsConf file into string

     QString configPathString(configPath);
     configPathString += "ups.conf";

      args["drivCtrlPath"] = drivCtrlPathString;
      args["serverPath"]= serverPathString;
      args["monitoringPath"]= monitoringPathString;
      args[configPathString] = upsConfString;

      m_upsConfIsChanged=false;
      }


    if (m_upsdConfIsChanged) {

     saveUpsdConfFile(&upsdConfString);
     QString configPathString (configPath);
     configPathString += "upsd.conf";

      args[configPathString] = upsdConfString;
      args["serverPath"]= serverPathString;

      m_upsdConfIsChanged=false;
      }

    if (m_upsdUserIsChanged) {
      saveUpsdUserFile(&upsdUsersString);
     QString configPathString (configPath);
     configPathString += "upsd.users";

      args[configPathString] = upsdUsersString;
      args["serverPath"]= serverPathString;

      m_upsdUserIsChanged=false;
      }

    if (m_upsmonConfIsChanged) {
      saveUpsmonConfFile(&upsmonConfString);
     QString configPathString (configPath);
     configPathString += "upsmon.conf";

      args[configPathString] = upsmonConfString;
      args["monitoringPath"]= monitoringPathString;

      m_upsmonConfIsChanged=false;
      }

    saveAction.setArguments(args);
    KAuth::ActionReply reply = saveAction.execute();

  if (reply.failed()) {
    if (reply.type() == KAuth::ActionReply::KAuthError) {
      KMessageBox::error(this, i18n("Unable to authenticate/execute the action: %1, %2", reply.errorCode(), reply.errorDescription()));
      }
    }

  }



int KNutSetting::buttons () {

  return KCModule::Default|KCModule::Apply|KCModule::Help;
  }

void KNutSetting::configChanged() {
  // insert your saving code here...
  emit changed(true);
  }

QString KNutSetting::quickHelp() const {

         return i18n("Helpful information about the <h1>knutsetting</h1> module."); 
  }

//----- Private Procs -----------------------


void KNutSetting::saveConfig(void) {

  KConfig *config = new KConfig ("knutsettingrc");
  KConfigGroup configGroup = config->group ("Path");

  // must save from LineEdit file not from m_upsSetting
  configGroup.writeEntry("UpsConfPath",m_upsConfEdit->text());
  configGroup.writeEntry ("UpsServerPath",m_upsServerEdit->text());
  configGroup.writeEntry ("UpsMonitoringPath",m_upsMonitoringEdit->text());
  configGroup.writeEntry ("UpsDrivCtrlPath",m_upsDrivCtrlEdit->text());
  configGroup.sync();
  }

void KNutSetting::loadDistConf(void) {
  QString filename;
  distPathStruct distPathItem;
  char lineBuff[1024];  // buffer for reading line from file

  //adds unknown line
  distPathItem.name=I18N_NOOP("None/Unknown");
  distPathItem.conf="";
  distPathItem.upsd="";
  distPathItem.upsmon="";
  distPathItem.upsdrvctl="";
  m_distPathVector.push_back(distPathItem);

  //reads informations about paths 
  filename = KGlobal::dirs()->findResource("data","knutsetting/dist.conf");
  QFile file(filename);
  if (file.open(QIODevice::ReadOnly)) {

    while (!file.atEnd()) {
      if (file.readLine(lineBuff,sizeof(lineBuff)) != -1) { 
        QString line(lineBuff);
        line = line.trimmed();
        if (line.length() > 0) {
          distPathItem.name=line.section('|',0,0);
          distPathItem.conf=line.section('|',1,1);
          distPathItem.upsd=line.section('|',2,2);
          distPathItem.upsmon=line.section('|',3,3);
          distPathItem.upsdrvctl=line.section('|',4,4);
          m_distPathVector.push_back(distPathItem);
          }
        }
      }
    file.close();
    }
  else {

  }

}

void KNutSetting::loadConfig(void) {

  KConfig *config = new KConfig ("knutsettingrc");

  KConfigGroup configGroup = config->group ("Path");
  m_upsConfEdit->setText(configGroup.readEntry("UpsConfPath",""));
  m_upsServerEdit->setText(configGroup.readEntry("UpsServerPath",""));
  m_upsMonitoringEdit->setText(configGroup.readEntry("UpsMonitoringPath",""));
  m_upsDrivCtrlEdit->setText(configGroup.readEntry("UpsDrivCtrlPath",""));

  if ((!m_upsConfEdit->text().isEmpty()) && (!(m_upsConfEdit->text().right(1) == "/"))) m_upsConfEdit->setText(m_upsConfEdit->text()+"/");
  m_upsSetting.confPath=m_upsConfEdit->text();
  m_upsSetting.serverPath=m_upsServerEdit->text();
  m_upsSetting.monitoringPath=m_upsMonitoringEdit->text();
  m_upsSetting.drivCtrlPath=m_upsDrivCtrlEdit->text();
    
  delete config;

  // set default path for etc directory, upsd and upsdrvctrl
  m_upsConfDefault=m_upsConfEdit->text();
  m_upsDrivCtrlDefault=m_upsDrivCtrlEdit->text();
  m_upsServerDefault=m_upsServerEdit->text();
  m_upsMonitoringDefault=m_upsMonitoringEdit->text();
  }


void KNutSetting::initVar(void) {

    m_upsConfDefault="";
    m_upsDrivCtrlDefault="";
    m_upsServerDefault="";
    m_upsMonitoringDefault="";

    // gets addresses for modul monitoring
    m_modulesData=commonData->modulesDataAddr();
    m_ACData=commonData->ACDataAddr();
    m_usersData=commonData->usersDataAddr();
    m_monitorData=commonData->monitorDataAddr();
    
    commonData->ACDataClean();
    m_upsdConfRem.clear();


//   data are cleaned in constructor of KNUSCommonData
//    commonData->modulesDataClean();
//    commonData->Clean();
//    commonData->m_monitorDataClean();

        
    m_upsConfIsChanged = false;
    m_upsdConfIsChanged = false;
    m_upsdUserIsChanged = false;
    m_upsmonConfIsChanged = false;
}


void KNutSetting::setDistribution(void) {
  int i;
  QString myUpsConf;
  QVector<distPathStruct>::const_iterator it;
  
  myUpsConf = m_upsConfEdit->text();
  if ((!myUpsConf.isEmpty()) && (myUpsConf.right(1) == "/"))
     myUpsConf = myUpsConf.left(myUpsConf.length()-1);
  i=0;
  for (it=m_distPathVector.begin(); it != m_distPathVector.end(); ++it) {
    i++;
    if (it != m_distPathVector.begin()) { // don't test the first item , the first item is for own setting
      if (((*it).conf==myUpsConf) &&  ((*it).upsd==m_upsServerEdit->text()) &&
         ((*it).upsmon==m_upsMonitoringEdit->text()) && ((*it).upsdrvctl==m_upsDrivCtrlEdit->text())) {
	 
        m_distComboBox->setCurrentIndex(i);
        return;
        }    
      }
    }



//  for (i = 1; i != distCount; i++) { // don't test the first item , the first item is for own setting
//    if ((distPath[i][1]==myUpsConf) &&  (distPath[i][2]==upsServerEdit->text()) &&
//       (distPath[i][3]==upsMonitoringEdit->text()) && (distPath[i][4]==upsDrivCtrlEdit->text())) {
//      m_distComboBox->setCurrentItem(i);
//      return;
//     }
//   }
  m_distComboBox->setCurrentItem(0);
  }
            

//-------- SLOTS ------------------------


void KNutSetting::changedUpsConfSlot(void) {

  m_upsConfIsChanged = true;
  emit changed(true);
  }


void KNutSetting::changedUpsdConfSlot(void) {

  m_upsdConfIsChanged = true;
  emit changed(true);
  }

void KNutSetting::changedUpsdUserSlot(void) {

  m_upsdUserIsChanged = true;
  emit changed(true);
  }


void KNutSetting::changedUpsmonConfSlot(void) {

  m_upsmonConfIsChanged = true;
  emit changed(true);
  }

      
void KNutSetting::getConfigDirSlot(void) {

  QString dataDir = KFileDialog::getExistingDirectory (m_upsConfEdit->text());
  if (!(dataDir.isEmpty())) m_upsConfEdit->setText(dataDir);
  }

void KNutSetting::getServerFileSlot(void) {

  QString dataDir = KFileDialog::getOpenFileName (m_upsServerEdit->text(),"upsd");
  if (!(dataDir.isEmpty())) m_upsServerEdit->setText(dataDir);
  }


void KNutSetting::getMonitoringFileSlot(void) {

  QString dataDir = KFileDialog::getOpenFileName (m_upsMonitoringEdit->text(),"upsmon");
  if (!(dataDir.isEmpty())) m_upsMonitoringEdit->setText(dataDir);
  }
  

void KNutSetting::getDrivCtrlFileSlot(void) {

  QString dataDir = KFileDialog::getOpenFileName (m_upsDrivCtrlEdit->text(),"upsdrvctl");
  if (!(dataDir.isEmpty()))  m_upsDrivCtrlEdit->setText(dataDir);
  }

void KNutSetting::setSelectedDistSlot(int index) {

  m_upsConfEdit->setText(m_distPathVector[index].conf);
  m_upsServerEdit->setText(m_distPathVector[index].upsd);
  m_upsMonitoringEdit->setText(m_distPathVector[index].upsmon);
  m_upsDrivCtrlEdit->setText(m_distPathVector[index].upsdrvctl);
  
  
//  upsConfEdit->setText(distPath[index][1]);
//  upsServerEdit->setText(distPath[index][2]);
//  upsMonitoringEdit->setText(distPath[index][3]);
//  upsDrivCtrlEdit->setText(distPath[index][4]);
  }

void KNutSetting::tabIsChangedSlot(QWidget* activeSlot) {

  if ((activeSlot != m_pathTab) && (m_oldPageIndex==4)) {   // jump from the m_pathTab to other tab
//  UPS CONF PATH
    m_upsSetting.serverPath=m_upsServerEdit->text();
    m_upsSetting.monitoringPath=m_upsMonitoringEdit->text();
    m_upsSetting.drivCtrlPath=m_upsDrivCtrlEdit->text();

// when upsConf is changed it must test path and parse conf files (loadData)
    if ((!m_upsConfEdit->text().isEmpty()) && (!(m_upsConfEdit->text().right(1) == "/"))) m_upsConfEdit->setText(m_upsConfEdit->text()+"/");
    if ( m_upsSetting.confPath != m_upsConfEdit->text()) {
      m_upsSetting.confPath = m_upsConfEdit->text();
      loadConfFiles( m_upsSetting.confPath );
      }
    }
  m_oldPageIndex=m_mainTabWidget->currentIndex();  
  }


void KNutSetting::loadData (QString& upsConfString, QString& upsdConfString, QString& upsdUsersString, QString& upsmonConfString) {
  commonData->modulesDataClean();
  parseUpsConfFile(&upsConfString);
  m_modulesWidget->loadItems();

  commonData->ACDataClean();
  parseUpsdConfFile(&upsdConfString);
  m_accessWidget->loadItems();

  commonData->usersDataClean();
  parseUpsdUsersFile(&upsdUsersString);
  m_serverWidget->loadItems();

  commonData->monitorDataClean();
  parseUpsmonConfFile(&upsmonConfString);
  m_monitorWidget->loadItems();
  }
      
void KNutSetting::testPath(void) {
   FILE*  myFile;
   struct stat myStat;
   QString message = "";

   if (m_upsSetting.confPath.isEmpty()) message = message + i18n("The configure path is empty.") +"\n";
   else {
     if ((myFile = fopen(m_upsSetting.confPath.toLocal8Bit(),"r")) == NULL)
       message = message + i18n("The configure path isn't selected.")+"\n"; 
     else {
       stat (m_upsSetting.confPath.toLocal8Bit(),&myStat);
       if (!(S_ISDIR(myStat.st_mode)))
         message = message + i18n("The configure path is not directory.")+"\n";
       }
     }

   if (m_upsSetting.serverPath.isEmpty())
     message = message +i18n("The path for information server (upsd) is empty.")+"\n";
   else {
     if ((myFile = fopen(m_upsSetting.serverPath.toLocal8Bit(),"r")) == NULL)
       message = message + i18n("The path for information server (upsd) isn't selected.")+ "\n";
     else {
       stat (m_upsSetting.serverPath.toLocal8Bit(),&myStat);
       if (!(S_ISREG(myStat.st_mode)))
//         message = message + i18n("\"%1\" isn't file. (information server)",upsSetting.serverPath)+"\n";
         message = message + i18n("\"%1\" must be file. Information server (upsd) cannot be run.")+"\n";
//         message = message + i18n("The path for information server (upsd) is not file.",upsSetting.serverPath)+"\n";
       }
     }

   if (m_upsSetting.monitoringPath.isEmpty())
     message = message + i18n("The path for monitoring client (upsmon) is empty.")+"\n";
   else {
     if ((myFile = fopen(m_upsSetting.monitoringPath.toLocal8Bit(),"r")) == NULL)
       message = message + i18n("The path for monitoring client (upsmon) isn't selected.")+"\n";
     else {
       stat (m_upsSetting.monitoringPath.toLocal8Bit(),&myStat);
       if (!(S_ISREG(myStat.st_mode)))
//         message = message + i18n("The path for monitoring client (upsmon) is not file.")+"\n";
         message = message + i18n("\"%1\" must be file. Monitoring client (upsmon) cannot be run.",m_upsSetting.monitoringPath)+"\n";
       }
     }
     
   if (m_upsSetting.drivCtrlPath.isEmpty())
     message = message + i18n("The path for driver controler (upsdrvctl) is empty.")+"\n";
   else {
     if ((myFile = fopen(m_upsSetting.drivCtrlPath.toLocal8Bit(),"r")) == NULL)
       message = message + i18n("The path for driver controler (upsdrvctl) isn't selected.")+"\n";
     else {
       stat (m_upsSetting.drivCtrlPath.toLocal8Bit(),&myStat);
       if (!(S_ISREG(myStat.st_mode)))
//         message = message + i18n("The path for driver controler (usbdrvctl) is not file.")+"\n";
         message = message + i18n("\"%1\" must be file. Driver controler (usbdrvctl) cannot be run.",m_upsSetting.monitoringPath)+"\n";
       }
     }

   if (!(message.isEmpty())) {
      message += i18n("You must set path.");
//      message += i18n("You must decide path.");
      KMessageBox::sorry (0,message);
      // jump on m_pathTab
      m_mainTabWidget->setCurrentIndex(4);
      }
  }


void KNutSetting::saveUpsConfFile(QString* upsConfString) {
  modulesList::const_iterator itModules;
  moduleItems::const_iterator itItem;
  QString name, port, driver;


  QTextStream upsdStream( upsConfString );

  for (QStringList::Iterator it=m_upsConfRem.begin();it != m_upsConfRem.end(); it++) {
    upsdStream << *it << endl;
    }
  upsdStream << "" << endl;
  upsdStream << "#KNutSetting Modules" << endl;

  for (itModules = m_modulesData->begin(); itModules != m_modulesData->end();itModules++) {
    port = (*itModules).portFileName.trimmed();
    driver = (*itModules).driver.trimmed();
    name = (*itModules).name.trimmed();

    if ((!name.isEmpty()) && (!driver.isEmpty()) && (!port.isEmpty())) {
      upsdStream << " [" << name << "]" << endl;
      upsdStream << "\tdriver = " << driver << endl;
      upsdStream << "\tport = " << port << endl;
      if ((*itModules).sdOrderFind) upsdStream << "\tsdorder = " << (*itModules).sdOrder << endl;
      if ((*itModules).noLockFind) upsdStream << "\tnolock" << endl;
      if ((*itModules).maxStartDelayFind) upsdStream << "\tmaxstartdelay = " << (*itModules).maxStartDelay << endl;
      if (!(*itModules).desc.isEmpty()) upsdStream << "\tdesc = \"" << (*itModules).desc << "\"" << endl;
      // write nostandart items
      for (itItem = (*itModules).items.begin(); itItem != (*itModules).items.end(); itItem++) {
        if ((*itItem).itemFound)
        upsdStream << "\t" << (*itItem).itemName << " = " << (*itItem).value << endl;
        }
      }
    upsdStream << "" << endl;
    }
 
  upsdStream.flush();
  }
  
void KNutSetting::saveUpsdUserFile(QString* upsdUserString) {
  usersPermList::const_iterator itUser;
  KNSString word;
  KNSString line;
    

  QTextStream upsdStream( upsdUserString );

  for (QStringList::Iterator it=m_upsdUsersRem.begin();it != m_upsdUsersRem.end(); it++) {
    upsdStream << *it << endl;
    }

  upsdStream << "" << endl;
  upsdStream << "#KNutSetting Users" << endl;
  upsdStream << "" << endl;
  for (itUser = m_usersData->begin(); itUser != m_usersData->end(); itUser++) {
    upsdStream << "\t[" << (*itUser).name << "]" << endl;
    if (!(*itUser).password.isEmpty())
      upsdStream << "\t\tpassword = " << ((*itUser).password).envelop() << endl;
    if ((*itUser).isUser) {
      if (!(*itUser).actions.isEmpty()) {
        upsdStream << "\t\tactions =";
        line = (*itUser).actions;
        while (!line.isEmpty()) {
          upsdStream << " " << line.extractWord();
          line.trimmedOwn();
          }
        upsdStream << endl;
        }
      if (!(*itUser).instcmds.isEmpty()) {
        upsdStream << "\t\tinstcmds =";
        for (QStringList::const_iterator it = (*itUser).instcmds.begin(); it != (*itUser).instcmds.end(); it++) {
          word = (*it);
          upsdStream << " " << word.envelop();
          }
        upsdStream << endl;
        }
      }
    else {
      if ((*itUser).upsmon)
        upsdStream << "\t\tupsmon master" << endl;
      else
        upsdStream << "\t\tupsmon slave" << endl;
      }      

      
    
    upsdStream << "" << endl;
    }
  upsdStream.flush(); 
  }  

void KNutSetting::saveUpsdConfFile(QString* upsdConfString) {

  QTextStream upsdStream( upsdConfString );

  for (QStringList::Iterator it=m_upsdConfRem.begin();it != m_upsdConfRem.end(); it++) {
    upsdStream << *it << endl;
    }

  upsdStream << "" << endl;
  upsdStream << "#KNutSetting Access Control Data" << endl;


   for (listenAddressPortListDef::Iterator itListenAddress = m_ACData->listenAddressPortList.begin(); itListenAddress != m_ACData->listenAddressPortList.end(); itListenAddress++) {
     upsdStream << "LISTEN "<< (*itListenAddress).address << " " << (*itListenAddress).port << endl;
   } 
  upsdStream << "" << endl;


  if (!m_ACData->maxAgeFound) {
     upsdStream << "MAXAGE " << m_ACData->maxAge <<endl;
    }

  upsdStream << "" << endl;

  if (!m_ACData->maxConnFound) {
    upsdStream << "MAXCONN " << m_ACData->maxConn <<endl;
    }
  upsdStream.flush();
  }


void KNutSetting::saveUpsmonConfFile(QString* upsmonConfString) {
  bool otherFlag;
  monitorUpsList::const_iterator monIt;


  QTextStream upsmonStream( upsmonConfString );
  
  for (QStringList::Iterator it=m_upsmonConfRem.begin();it != m_upsmonConfRem.end(); it++) {
    upsmonStream << *it << endl;
    }

  upsmonStream << endl;
  upsmonStream << "#KNutSetting Monitoring" << endl;

  if (m_monitorData->runAsUserFind)
    upsmonStream << "RUN_AS_USER " << m_monitorData->runAsUser << endl;

   // here is place for monitoring data

  upsmonStream << endl;
  for (monIt=m_monitorData->nutMonitorUps.begin(); monIt != m_monitorData->nutMonitorUps.end(); monIt++ ) {
    upsmonStream << "MONITOR " << (*monIt).upsName << "@" << (*monIt).hostName << " ";
    upsmonStream << QString::number((*monIt).powerValue) << " " << (*monIt).userName << " " << (*monIt).password;
    if ((*monIt).upsmon == MON_MASTER) upsmonStream << " master" << endl;
    else upsmonStream << " slave" << endl;
    }
  upsmonStream << endl;     

  upsmonStream << "MINSUPPLIES " << m_monitorData->monitorValues.minSupplies << endl << endl;
  upsmonStream << "SHUTDOWNCMD \"" << m_monitorData->monitorValues.shutDownCmd << "\"" << endl << endl;

  if (m_monitorData->monitorValues.notifyCmdFound)
    upsmonStream << "NOTIFYCMD " << m_monitorData->monitorValues.notifyCmd << endl << endl;

  if (m_monitorData->monitorValues.pollFreqFound)
    upsmonStream << "POLLFREQ " << m_monitorData->monitorValues.pollFreq << endl << endl;

  if (m_monitorData->monitorValues.pollAlertFreqFound)
    upsmonStream << "POLLFREQALERT " << m_monitorData->monitorValues.pollAlertFreq << endl << endl;

  upsmonStream << "HOSTSYNC " << m_monitorData->monitorValues.hotSync << endl << endl;
  upsmonStream << "DEADTIME " << m_monitorData->monitorValues.deadTime << endl << endl;
  upsmonStream << "POWERDOWNFLAG " << m_monitorData->monitorValues.powerDown << endl << endl;

                                        
  for (int i = 0 ; i < notifyNamesCount; i++) {
    if (!m_monitorData->monNotifyItems[i].message.isEmpty())
      upsmonStream << "NOTIFYMSG " << notifyName[i] << " \"" << m_monitorData->monNotifyItems[i].message << "\"" << endl;
    }
  upsmonStream << endl;


  for (int i = 0 ; i < notifyNamesCount; i++) {

    if ((m_monitorData->monNotifyItems[i].syslogFlag) ||
        (m_monitorData->monNotifyItems[i].wallFlag) ||
        (m_monitorData->monNotifyItems[i].execFlag) ||
        (m_monitorData->monNotifyItems[i].ignoreFlag)) {

      upsmonStream << "NOTIFYFLAG " << notifyName[i] << " ";
      if (m_monitorData->monNotifyItems[i].ignoreFlag) upsmonStream << "IGNORE" << endl;
      else {
        otherFlag = false;
        if (m_monitorData->monNotifyItems[i].syslogFlag){
          upsmonStream << "SYSLOG";
          otherFlag = true;
          }
        if (m_monitorData->monNotifyItems[i].wallFlag){
          if (otherFlag) upsmonStream << "+";
          else otherFlag = true;
          upsmonStream << "WALL";
          }
        if (m_monitorData->monNotifyItems[i].execFlag){
          if (otherFlag) upsmonStream << "+";
          else otherFlag = true;
          upsmonStream << "EXEC";
          }
        }

      upsmonStream << endl;
      }
    }
  if (m_monitorData->monitorValues.rbWarnTimeFound)
    upsmonStream << "RBWARNTIME " << m_monitorData->monitorValues.rbWarnTime << endl << endl;
  if (m_monitorData->monitorValues.noCommWarnTimeFound)
    upsmonStream << "NOCOMMWARNTIME " << m_monitorData->monitorValues.noCommWarnTime << endl << endl;
  upsmonStream << "FINALDELAY " << m_monitorData->monitorValues.finalDelay << endl << endl;

  upsmonStream << endl;
  upsmonStream.flush();
  }


void KNutSetting::parseModXMLFile (void) { // parse modules bestfcom,  blazer_usb ...
  QFile* xmlFile;
  QXmlSimpleReader reader;
  QString filename;
  //parses XML file with modules informations

  filename = KGlobal::dirs()->findResource("data","knutsetting/modules_def.xml");
  
  xmlFile = new QFile (filename);
  if (xmlFile->open(QIODevice::ReadOnly)) {

    
    KNSSaxHandler* handler = new KNSSaxHandler(commonData);
    reader.setContentHandler(handler);
    reader.setErrorHandler(handler);
    reader.parse(xmlFile);

    xmlFile->close();
    delete handler;
    }
  delete xmlFile;
  }


bool KNutSetting::findModParItemExist (const QString name, const moduleItems* items)  {
  moduleItems::const_iterator it;

  if (items->count() == 0) return false;
  for (it = items->begin(); it != items->end(); it++) {
    if ((*it).itemName==name) return true;
    }
  return false;
  }


void KNutSetting::parseUpsConfFile(QString* upsConfString) {
  QString word2, word3;
  KNSString line, word1, endWord;
  bool emptyLine=false;
  QString nameSection;
  bool existSection;
  KNSString lineWithoutComment;
  struct moduleStruct newModule;
  moduleItemStruct newItem;


  QTextStream* upsdStream= new QTextStream (upsConfString, QIODevice::ReadOnly);
  existSection=false;

  while (readLine(line, upsdStream)) {
    line.trimmedOwn();
    if (line.left(12) == "#KNutSetting") continue;
    if ((line.left(1) == "#")  || (line.length() == 0)){
      if ((line.length() != 0) || (emptyLine == false))
        { // only remarks
        m_upsConfRem.push_back(line);
        if (line.length() != 0) emptyLine=false;
        else emptyLine=true;
        }
      }
    else {
      line.stripCommentOwn();
      nameSection=findBrackets(line);
      if (!nameSection.isEmpty()) {
        if (existSection){
          m_modulesData->push_back(newModule);
           }
        // standard settings for new section
        existSection=true;
        newModule.name=nameSection;
        newModule.driver="";
        newModule.portFileName="";
        newModule.desc="";
        newModule.sdOrder=sdOrderDef;
        newModule.sdOrderFind=false;
        newModule.noLockFind=false;
        newModule.maxStartDelay=maxStartDelayDef;
        newModule.maxStartDelayFind=false;
        newModule.items.clear();
        }
      else {
        if (existSection) {
          word1 = line.extractWord();
          word2 = line.extractWord();
   
          word1.toLowerOwn();
          if (word2 == "=") {
            word3 = line.extractWord();
            if ((word1 == "driver") && (newModule.driver.isEmpty())) {
              if (!word3.isEmpty()) newModule.driver=word3;
              }
            else {  
              if ((word1 == "port") && (newModule.portFileName.isEmpty())) {
                if (!word3.isEmpty()) newModule.portFileName=word3;
                }
              else {  
                if ((word1 == "sdorder") && (!newModule.sdOrderFind)) {
                  if (!word3.isEmpty()) {
                    newModule.sdOrder=word3.toInt();
                    newModule.sdOrderFind=true;
                    }
                  }
                else {  
                  if ((word1 == "maxstartdelay") && (!newModule.maxStartDelayFind)) {
                    if (!word3.isEmpty()) {
                      newModule.maxStartDelay=word3.toInt();
                      newModule.maxStartDelayFind=true;
                      }
                    }
                  else {
                    if ((word1 == "desc") && (newModule.desc.isEmpty())) {
                      if (!word3.isEmpty()) newModule.desc=word3;
                      }
                    else {
                      // no standart items
                      if (!findModParItemExist(word1,&newModule.items)) {
                        newItem.itemName=word1;
                        newItem.value=word3;
                        newItem.itemFound=true;
                        newModule.items.push_back(newItem);
                        }
                      }
                    }
                  }  
                }
              }
            }
          else { // word2 != "="
            if ((word1 == "nolock") && (!newModule.noLockFind)) {
              newModule.noLockFind=true;
              }
            else {
              // no standart items
              if (!findModParItemExist(word1,&newModule.items)) {
                newItem.itemName=word1;
                newItem.value="";
                newItem.itemFound=true;
                newModule.items.push_back(newItem);
                }
              }
            }
          }
        }
      }
    }
  if (existSection) {
    m_modulesData->push_back(newModule);
    }
  }


 


void KNutSetting::parseUpsdUsersFile(QString* upsdUsersString) {
  QString word2, word3;
  KNSString line, word1, endWord;
  bool emptyLine=false;
  QString nameSection;
  bool existSection;
  KNSString lineWithoutComment;
  struct userPermStruct newUserPerm;
  
    {
  QTextStream* upsdStream= new QTextStream (upsdUsersString, QIODevice::ReadOnly);
    existSection=false;
    while (readLine(line, upsdStream)) {
      line.trimmedOwn();
      if (line.left(12) == "#KNutSetting") continue;
      if ((line.left(1) == "#")  || (line.length() == 0)){
        if ((line.length() != 0) || (emptyLine == false))
          { // only remarks
          m_upsdUsersRem.push_back(line);
          if (line.length() != 0) emptyLine=false;
          else emptyLine=true;
          }
        }
      else {
        line.stripCommentOwn();
        nameSection=findBrackets(line);
        if (!nameSection.isEmpty()) {
          if (existSection){
             m_usersData->push_back(newUserPerm);
             }
          // standard settings for new section
          existSection=true;
          newUserPerm.name=nameSection;
          newUserPerm.isUser=true;
          newUserPerm.password="";
          newUserPerm.actions="";
          newUserPerm.instcmds.clear();
          newUserPerm.upsmon=MON_SLAVE;
          }
        else {
          if (existSection) {
            word1 = line.extractWord();
            word2 = line.extractWord();
            word1.toLowerOwn();
            if ((word1 == "upsmon") && (newUserPerm.isUser)) {
              // if newUserPerm.isUser is false, upsmon was found yet
              newUserPerm.isUser=false;
              if (word2 == "master") newUserPerm.upsmon=MON_MASTER;
              }
            else {
              if (word2 == "=") {
                // adds item to structure
                if ((word1 == "password") && (newUserPerm.password.isEmpty())) {
                  word3 = line.extractWord();
                  if (!word3.isEmpty()) newUserPerm.password=word3;
                  }
                if (word1 == "actions"){
                  if (newUserPerm.actions.isEmpty())
                    newUserPerm.actions=line.extractWord();
                  else {
                    endWord = line.extractWord();
                    if (newUserPerm.actions.indexOf(endWord,0,Qt::CaseInsensitive) != -1)  // no case sensitive
                      newUserPerm.actions += " " + endWord;
                    }  
                  }
                if (word1 == "instcmds"){
                  lineWithoutComment=line.stripComment();
                  lineWithoutComment.trimmedOwn();
                  while (!lineWithoutComment.isEmpty()) {
                    endWord = lineWithoutComment.extractWord().toLower();
                    if (newUserPerm.instcmds.indexOf(endWord) == -1) {
//                    if (newUserPerm.instcmds.find(endWord) == newUserPerm.instcmds.end()) {
                      newUserPerm.instcmds.push_back(endWord);
                      lineWithoutComment.trimmedOwn();
                      }
                    }
                  }
                }
              }
            }  
          }
        }
      }
    if (existSection) {
      m_usersData->push_back(newUserPerm);
      }
    }
  }



 
      
void KNutSetting::parseUpsdConfFile(QString* upsdConfString) {
  QString word1, word2, word3;
  KNSString line;
  

  listenAddressPortStruct newListenAddressPort; 
 
  bool emptyLine=false;
  QTextStream* upsdStream= new QTextStream (upsdConfString, QIODevice::ReadOnly);


  while (readLine(line, upsdStream)) {

    line.trimmedOwn();
    if (line.left(12) == "#KNutSetting") continue;

    if ((line.left(1) == "#")  || (line.length() == 0)){
      // save to upsdConfRem
      if ((line.length() != 0) || (emptyLine == false))
        {
        m_upsdConfRem.push_back(line);
        if (line.length() != 0) emptyLine=false;
        else emptyLine=true;
        }
      }
    else {
      line.stripCommentOwn();
      word1=line.extractWord();
      if (word1=="MAXCONN") {
        word2=line.extractWord();
        if (!word2.isEmpty())  {
          m_ACData->maxConn=word2.toInt();
          m_ACData->maxConnFound=true;
          }
        }
      else {
        if (word1=="LISTEN") {
          word2=line.extractWord();
          word3=line.extractWord();
          if ((!word2.isEmpty()) && (!word3.isEmpty())) {
            newListenAddressPort.address=word2;
            newListenAddressPort.port=word3; 
            m_ACData->listenAddressPortList.push_back(newListenAddressPort); 
            }
          }
        else {
          if (word1 == "MAXAGE")
            word2=line.extractWord();
            if (!word2.isEmpty())  {
              m_ACData->maxAge=word2.toInt();
              m_ACData->maxAgeFound=true;
            }
          }  
        }  
      }
    }
  }


void KNutSetting::parseUpsmonConfFile(QString* upsmonConfString) {
  QString word2, word3, word4, word5, word6, wordFlag, upsName, hostName;
  KNSString line, word1, endWord;
  struct monitorUpsStruct newMonitorItem;
  bool emptyLine=false;
  QString nameSection;
  KNSString lineWithoutComment;

    QTextStream* upsmonStream = new QTextStream (upsmonConfString, QIODevice::ReadOnly);

    while (readLine(line, upsmonStream)) {
      line.trimmedOwn();
      if (line.left(12) == "#KNutSetting") continue;
      if ((line.left(1) == "#")  || (line.length() == 0)){
        if ((line.length() != 0) || (emptyLine == false))
          { // only remarks
          m_upsmonConfRem.push_back(line);
          if (line.length() != 0) emptyLine=false;
          else emptyLine=true;
          }
        }
      else {
        line.stripCommentOwn();
        word1=line.extractWord();
        word2=line.extractWord();
        if (word1 == "MONITOR") {
          word3=line.extractWord(); // powerValue
          word4=line.extractWord(); // userName
          word5=line.extractWord(); // pasword
          word6=line.extractWord(); // master/slave
          if ((!word2.isEmpty()) && (!word3.isEmpty()) && (!word4.isEmpty()) &&
             (!word5.isEmpty()) && (!word5.isEmpty()) && (!word6.isEmpty())) {
            upsName = word2.section('@',0,0);
            hostName = word2.section('@',1);
            if (upsName.isEmpty()) hostName="myups";
            if (hostName.isEmpty()) hostName="localhost";
            newMonitorItem.hostName=hostName.toLower();            
            newMonitorItem.upsName=upsName;
            newMonitorItem.password=word5;
            newMonitorItem.powerValue=word3.toInt();
            if (newMonitorItem.powerValue < 1) newMonitorItem.powerValue=1;
            newMonitorItem.userName=word4;
            if (word6.toLower() == "slave") newMonitorItem.upsmon=MON_SLAVE;
            else newMonitorItem.upsmon=MON_MASTER;

            if (!commonData->monitorUpsItemExist(newMonitorItem.upsName, newMonitorItem.hostName))
              m_monitorData->nutMonitorUps.push_back(newMonitorItem);
            }
          }
        if ((word1 == "RUN_AS_USER") && (!m_monitorData->runAsUserFind)) {
          m_monitorData->runAsUserFind=true;
          m_monitorData->runAsUser=word2;
          }
        if ((word1 == "MINSUPPLIES") && (!m_monitorData->monitorValues.minSuppliesFound)) {
          m_monitorData->monitorValues.minSuppliesFound=true;
          m_monitorData->monitorValues.minSupplies=word2.toInt();
          }
        if ((word1 == "SHUTDOWNCMD") && (!m_monitorData->monitorValues.shutDownCmdFound)) {
          m_monitorData->monitorValues.shutDownCmdFound=true;
          m_monitorData->monitorValues.shutDownCmd=word2;
          }
        if ((word1 == "NOTIFYCMD") && (!m_monitorData->monitorValues.notifyCmdFound)) {
          m_monitorData->monitorValues.notifyCmdFound=true;
          m_monitorData->monitorValues.notifyCmd=word2;
          }
        if ((word1 == "POLLFREQ") && (!m_monitorData->monitorValues.pollFreqFound)) {
          m_monitorData->monitorValues.pollFreqFound=true;
          m_monitorData->monitorValues.pollFreq=word2.toInt();
          }
        if ((word1 == "POLLFREQALERT") && (!m_monitorData->monitorValues.pollAlertFreqFound)) {
          m_monitorData->monitorValues.pollAlertFreqFound=true;
          m_monitorData->monitorValues.pollAlertFreq=word2.toInt();
          }
        if ((word1 == "HOSTSYNC") && (!m_monitorData->monitorValues.hotSyncFound)) {
          m_monitorData->monitorValues.hotSyncFound=true;
          m_monitorData->monitorValues.hotSync=word2.toInt();
          }
        if ((word1 == "DEADTIME") && (!m_monitorData->monitorValues.deadTimeFound)) {
          m_monitorData->monitorValues.deadTimeFound=true;
          m_monitorData->monitorValues.deadTime=word2.toInt();
          }
        if ((word1 == "POWERDOWNFLAG") && (!m_monitorData->monitorValues.powerDownFound)) {
          m_monitorData->monitorValues.powerDownFound=true;
         m_monitorData->monitorValues.powerDown=word2;
          }
        if ((word1 == "RBWARNTIME") && (!m_monitorData->monitorValues.rbWarnTimeFound)) {
          m_monitorData->monitorValues.rbWarnTimeFound=true;
          m_monitorData->monitorValues.rbWarnTime=word2.toInt();
          }
        if ((word1 == "NOCOMMWARNTIME") && (!m_monitorData->monitorValues.noCommWarnTimeFound)) {
          m_monitorData->monitorValues.noCommWarnTimeFound=true;
          m_monitorData->monitorValues.noCommWarnTime=word2.toInt();
          }
        if ((word1 == "FINALDELAY") && (!m_monitorData->monitorValues.finalDelayFound)) {
          m_monitorData->monitorValues.finalDelayFound=true;
          m_monitorData->monitorValues.finalDelay=word2.toInt();
          }
        // extract the third word NOTIFYMSG ONLINE something
        // ALERT other line with same word2 (ONLINE ONBATT ... ) rewrites previus (older) setting
        // NOTIFYMSG ONLINE "something one"
        // NOTIFYMSG ONLINE "something two"
        // the second line rewrites setting message for ONLINE on samething two
        // 
        
        word3=line.extractWord();

        if ((word1 == "NOTIFYMSG") && (!word3.isEmpty())){
          for (int i =0; i < notifyNamesCount; i++) {
            if (word2 == notifyName[i]) {
              m_monitorData->monNotifyItems[i].message=word3;
              }
            }
          }


        if ((word1 == "NOTIFYFLAG") && (!word3.isEmpty())){
          for (int i =0; i < notifyNamesCount; i++) {
            if (word2 == notifyName[i]) {
              m_monitorData->monNotifyItems[i].syslogFlag=false;
              m_monitorData->monNotifyItems[i].wallFlag=false;
              m_monitorData->monNotifyItems[i].execFlag=false;
              m_monitorData->monNotifyItems[i].ignoreFlag=false;
              while (!word3.isEmpty()) {

                wordFlag = word3.section('+',0,0);
                word3=word3.section('+',1);
                word3=word3.trimmed();
                if (wordFlag == "SYSLOG") m_monitorData->monNotifyItems[i].syslogFlag=true;
                  else {
                  if (wordFlag == "WALL") m_monitorData->monNotifyItems[i].wallFlag=true;
                    else {
                    if (wordFlag == "EXEC") m_monitorData->monNotifyItems[i].execFlag=true;
                      else {
                        if (wordFlag == "IGNORE") m_monitorData->monNotifyItems[i].ignoreFlag=true;
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



KNSString KNutSetting::findBrackets(KNSString line) {
  int leftBracket;
  int rightBracket;

  if ((leftBracket =line.indexOf('[')) == -1) return "";
  if ((rightBracket =line.indexOf(']')) == -1) return "";
  if ((rightBracket - leftBracket) > 1) return line.mid(leftBracket+1,rightBracket - leftBracket-1);
  else return "";
  };


bool KNutSetting::readLine(KNSString& line, QTextStream* upsdStream) {
bool addOtherLine;

  if (upsdStream->atEnd()) {
    line = "";
    return false;
    }


  line = upsdStream->readLine();
  addOtherLine = true;
  while (addOtherLine) {
    addOtherLine = false;
    if (!upsdStream->atEnd()) {
      if (!line.isEmpty()) {
        if (line[line.length()-1] == '\\' ) {
          line = line.left(line.length()-1) + upsdStream->readLine();
          addOtherLine = true;
          }
        }
      }
    }
  return true;  
  }


//------------------------SLOTS------------------------


void KNutSetting::confChangedSlot(const QString &) {

  setDistribution();
  emit changed(true);
  }


void KNutSetting::upsdConnectionTimeoutSlot (void) {
  m_nutLocalServer->getUpsValues();
  m_upsdTimer->start( 5000);
}

void KNutSetting::upsdConectionOKSlot(void) {

  m_connectPixLabel->setPixmap(m_connectedPix.pixmap(16,16,QIcon::Normal,QIcon::On));
  m_connectPixLabel->setToolTip(QString());
  m_connectPixLabel->setToolTip(i18n("UPS connection is up"));
  m_nutLocalServer->getUpsVars();

  m_reconnectTimer->stop();
  m_upsdTimer->start( 5000);
  }

void KNutSetting::upsdConnectionBrokenSlot(void) {

  m_connectPixLabel->setPixmap(m_noConnectedPix.pixmap(16,16,QIcon::Normal,QIcon::On));
  m_connectPixLabel->setToolTip(QString());
  m_connectPixLabel->setToolTip(i18n("UPS connection is broken"));

  m_upsdTimer->stop();
  m_reconnectTimer->setSingleShot( TRUE );
  m_reconnectTimer->start( 2000 ); // start timer for reconnect delay

  }

void KNutSetting::upsdProcesStartedSlot(void) {

  m_restartButton->setText(i18n("Restart ups"));
  m_upsdPixLabel->setPixmap(m_upsdPix.pixmap(16));

  m_upsdPixLabel->setToolTip(QString());
  m_upsdPixLabel->setToolTip(i18n("UPS daemon is running"));
  }

void KNutSetting::upsdProcesStopedSlot(void) {

  m_restartButton->setText(i18n("Start  ups"));
  m_upsdPixLabel->setPixmap(m_noProcesPix.pixmap(16));

  m_upsdPixLabel->setToolTip(QString());
  m_upsdPixLabel->setToolTip(i18n("UPS daemon isn't running."));
  }


void KNutSetting::upsmonProcesStartedSlot(void) {

  m_restartMonitoringButton->setText(i18n("Restart UPS monitoring"));
  m_upsMonPixLabel->setPixmap(m_upsMonPix.pixmap(16));

  m_upsMonPixLabel->setToolTip(QString());
  m_upsMonPixLabel->setToolTip(i18n("UPS monitoring is running."));
  }

void KNutSetting::upsmonProcesStopedSlot(void) {

  m_restartMonitoringButton->setText(i18n("Start UPS monitoring"));
  m_upsMonPixLabel->setPixmap(m_noProcesPix.pixmap(16));

  m_upsMonPixLabel->setToolTip(QString());
  m_upsMonPixLabel->setToolTip(i18n("UPS monitoring isn't running."));
  }


void KNutSetting::upsdRestartSlot(void) {

  KAuth::Action execAction("org.kde.kcontrol.knutsetting.exec");
  execAction.setHelperID("org.kde.kcontrol.knutsetting");

  QVariantMap args;

  if (m_upsSetting.serverPath.contains("/")) {
    args["serverPath"] = m_upsSetting.serverPath.section("/upsd",0,0);
    }
  else {
    args["serverPath"] = m_upsSetting.serverPath;
   }


  execAction.setArguments(args);
  KAuth::ActionReply reply = execAction.execute();

  if (reply.failed()) {
    if (reply.type() == KAuth::ActionReply::KAuthError) {
      KMessageBox::error(this, i18n("Unable to authenticate/execute the action: %1, %2", reply.errorCode(), reply.errorDescription()));
      }
    }
  }




void KNutSetting::upsmonRestartSlot(void) {

  KAuth::Action execAction("org.kde.kcontrol.knutsetting.exec");
  execAction.setHelperID("org.kde.kcontrol.knutsetting");

  QVariantMap args;


  if (m_upsSetting.monitoringPath.contains("/")) {
    args["monitoringPath"] = m_upsSetting.monitoringPath.section("/upsmon",0,0);
    }
  else {
    args["monitoringPath"] = m_upsSetting.monitoringPath;
   }


  execAction.setArguments(args);
  KAuth::ActionReply reply = execAction.execute();

  if (reply.failed()) {
    if (reply.type() == KAuth::ActionReply::KAuthError) {
      KMessageBox::error(this, i18n("Unable to authenticate/execute the action: %1, %2", reply.errorCode(), reply.errorDescription()));
      }
    }
  }





void KNutSetting::upsDeletedSlot (const QString upsName) {
  // we must say to monitoring's part that module's item was deleted

  m_monitorWidget->deleteUpsMonitoring(upsName);
  }


void KNutSetting::userDeletedSlot (const QString userName) {
  // we must say to monitoring's part that user's item was deleted
  m_monitorWidget->deleteUserMonitoring(userName);
  }

void KNutSetting::userAddedSlot (const QString userName) {
  // we have to say to monitoring's part that user's item was deleted
  m_monitorWidget->addUserMonitoring(userName);
  }


void KNutSetting::slotReconnectTimeout (void) {

  m_nutLocalServer->open();
  }

void KNutSetting::upsdConnectionErrorSlot (int ) {

  m_reconnectTimer->setSingleShot( TRUE );
  m_reconnectTimer->start( 2000 ); // starts reconnect timer again
  }


#include "knutsetting.moc"
