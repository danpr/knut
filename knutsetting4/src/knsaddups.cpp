/***************************************************************************
                          knsaddups.cpp  -  description
                             -------------------
    begin                : ?t b?e 31 2005
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

#include "knsaddups.h"
#include "knsupssaxhandler.h"

#include <KDE/KLocale>
#include <KDE/KGlobal>
#include <KDE/KStandardDirs>

#include <KDE/KFileDialog>
#include <KDE/KIcon>
#include <KDE/KUrl>



#include <QtGui/QListWidget>
#include <QtGui/QGroupBox>
#include <QtGui/QFontMetrics>
#include <QtGui/QLayout>
#include <QtGui/QSplitter>
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QFile>
//#include <qxml.h>
#include <QtGui/QFrame>


#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QCheckBox>
#include <QtGui/QTreeWidgetItem>

#include <QtDebug>


KNSAddUPS::KNSAddUPS(KNSCommonData* commonData, QWidget *parent )
          : KDialog(parent), m_commonData(commonData) {

  setButtons( KDialog::Ok | KDialog::Cancel);
  setModal(true);
  setInitialSize(QSize(450,600));


  m_modulesData = m_commonData->modulesDataAddr();
  m_selectedManufactItem=0;


  KIcon choosePix = KIcon("fileopen");

  QFontMetrics fm =  fontMetrics();

  QWidget* mWidget = new QWidget(this);
  setMainWidget(mWidget);
  //mWidget->setBaseSize(500,500);
  //mWidget->resize(500,500);



  QLabel *label1 = new QLabel(i18n("Select manufacturer and model of UPS or driver."),mWidget);

  QFrame* topLine = new QFrame(mWidget);
  topLine->setFrameStyle(QFrame::HLine);
  topLine->setLineWidth(1);
  topLine->setMidLineWidth(0);
  topLine->setFrameShadow(QFrame::Sunken);


  QSplitter* vSplitter = new QSplitter(Qt::Vertical,mWidget);
  vSplitter->setOpaqueResize ( TRUE );
  QWidget *hWidget = new QWidget(vSplitter);

  QSplitter* hSplitter = new QSplitter(Qt::Horizontal,hWidget);
  hSplitter->setOpaqueResize ( TRUE );

  QGroupBox* manufacGroupBox = new QGroupBox(i18n("Manufacturer"),hSplitter);

  m_manufactBox = new QListWidget(manufacGroupBox);
  m_manufactBox->setMinimumSize(150,100);

  QGroupBox* modelGroupBox = new QGroupBox(i18n("Model"),hSplitter);

  m_modelBox = new QListWidget (modelGroupBox);
  m_modelBox->setMinimumSize(150,100);

  QWidget* downWidget = new QWidget(vSplitter);
  QLabel* nameLabel = new QLabel (i18n("Name") +" :",downWidget);
  m_nameLineEdit = new KLineEdit (downWidget);

  QGroupBox* driverGroupBox = new QGroupBox(i18n("Driver"),downWidget);
  QWidget* driverWidget = new QWidget(driverGroupBox);

  m_driverCombo = new KComboBox(driverWidget);
  QGroupBox* portGroupBox = new QGroupBox(i18n("Port"),driverWidget);

  QLabel* label2 = new QLabel(i18n("Type")+ " : ",portGroupBox);
  m_connectLabel = new QLabel(portGroupBox);

  m_portAuto = new QCheckBox(i18n("Auto"),portGroupBox);

  m_portDevCombo = new KComboBox(portGroupBox);
  m_portAdd = new KPushButton (portGroupBox);
  m_portAdd->setIcon(choosePix);

  m_desc = new KTextEdit(driverWidget);
  m_desc->setReadOnly(true);

  QFrame* bottomLine = new QFrame(mWidget);
  bottomLine->setFrameStyle(QFrame::HLine);
  bottomLine->setLineWidth(1);
  bottomLine->setMidLineWidth(0);
  bottomLine->setFrameShadow(QFrame::Sunken);


  QVBoxLayout* topLayout = new QVBoxLayout(mWidget);
  QHBoxLayout* hLayout = new QHBoxLayout(hWidget);
  QVBoxLayout* manufacLayout = new QVBoxLayout( manufacGroupBox );
  QVBoxLayout* modelLayout = new QVBoxLayout( modelGroupBox );
  QVBoxLayout* downLayout = new QVBoxLayout(downWidget);
  QHBoxLayout* nameLayout = new QHBoxLayout();
  QVBoxLayout* driverGroupLayout = new QVBoxLayout(driverGroupBox);
  QVBoxLayout* driverLayout = new QVBoxLayout(driverWidget);
  QGridLayout* portGroupLayout = new QGridLayout(portGroupBox);
  portGroupLayout->setColumnStretch(0,5);
  QHBoxLayout* connectLayout = new QHBoxLayout();
  portGroupLayout->addWidget(m_portDevCombo,2,0);


  topLayout->addWidget(label1);
  topLayout->addWidget(topLine);
  topLayout->addWidget(vSplitter);
  topLayout->addWidget(bottomLine);

  hLayout->addWidget(hSplitter);

  manufacLayout->addWidget(m_manufactBox);

  modelLayout->addWidget(m_modelBox);

  downLayout->addLayout(nameLayout);
  downLayout->addWidget(driverGroupBox);

  nameLayout->addWidget(nameLabel);
  nameLayout->addWidget(m_nameLineEdit);
  driverGroupLayout->addWidget(driverWidget);

  driverLayout->addWidget(m_driverCombo);
  driverLayout->addWidget(portGroupBox);
  driverLayout->addWidget(m_desc);

  portGroupLayout->addItem(new QSpacerItem(0,fm.height()-5), 0, 0);
  portGroupLayout->addLayout(connectLayout,1,0);
  portGroupLayout->addWidget(m_portAuto,1,1);
  portGroupLayout->addWidget(m_portAdd,2,1);

  connectLayout->addWidget(label2);
  connectLayout->addWidget(m_connectLabel);
  connectLayout->addStretch(10);

  QList<int> vList;
  vList.push_back(300);
  vList.push_back(150);
  vSplitter->setSizes(vList);

  QList<int> hList;
  hList.push_back(150);
  hList.push_back(200);
  hSplitter->setSizes(hList);


  //load informatsions about UPSes to UPSDataBase
  m_driverSeletedList=0;
  initUPSData();
  showManufact();
  disableDriverPart();

  connect (m_manufactBox,SIGNAL( itemSelectionChanged ( void )),this,SLOT( manufactChangedSlot (void) ));

//  connect (m_modelBox,SIGNAL( itemActivated ( QListWidgetItem* )),this,SLOT( modelSelectedSlot ( QListWidgetItem*)));
  connect (m_modelBox,SIGNAL( itemSelectionChanged ( void )),this,SLOT( modelChangedSlot (void)));

  connect (m_driverCombo,SIGNAL( activated(int)),this,SLOT(driverSelectedSlot (int)));
  connect (m_portAdd, SIGNAL(released()), this, SLOT(getNewPortSlot()));
  connect (m_portAuto, SIGNAL(toggled(bool)), this, SLOT(autoToggledSlot(bool)));
  connect (m_nameLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(nameChangedSlot(const QString&)));
  connect (this, SIGNAL(okClicked()), this, SLOT(addModuleSlot()));
  }



KNSAddUPS::~KNSAddUPS(){
  }


void KNSAddUPS::clearDriverPart (void) {
  m_nameLineEdit->clear();
  enableButtonOk(false);
  m_driverCombo->clear();
  m_portDevCombo->clear();
  m_connectLabel->setText("");
  m_desc->setText("");
  m_portAuto->setChecked(false);
  m_portAuto->setEnabled(true);
  m_portAutoOnly = false;
  m_driverCurrentId=-1;
//  paramsList=0;
  }

void KNSAddUPS::disableDriverPart (void) {

  clearDriverPart();
  m_portAuto->setEnabled(false);
  m_driverCombo->setEnabled(false);
  m_portDevCombo->setEnabled(false);
  m_portAdd->setEnabled(false);
  m_nameLineEdit->setEnabled(false);
  enableButtonOk(false);

  }

void KNSAddUPS::initUPSData (void){
  modulesDefList::const_iterator modDefIt;
  modulesDefList* modDefList;
  manufactList::iterator manufactIt;
  upsModelDriverStruct newModelDriver;


  if (m_commonData->manufactCount() == 0) {
    // parse description of UPSes
    parseUPSXMLFile();

    // add ups from list of modules
    //returns addres of list of modulles  // KNSCommonData
    modDefList=m_commonData->modulesDefAddr();

    for (modDefIt = modDefList->begin(); modDefIt != modDefList->end(); modDefIt++) {
      newModelDriver.paramsList.clear();
      newModelDriver.driverName=(*modDefIt).name;
      newModelDriver.distName=(*modDefIt).distName;
      newModelDriver.portType=(*modDefIt).portType;
      // description
      newModelDriver.description=(*modDefIt).description;
      m_commonData->addManufactModelDriver (i18n("Common"),(*modDefIt).name, newModelDriver);

      }
    manufactIt = m_commonData->findManufact(i18n("Common"));

    m_commonData->sortModels(&(*manufactIt).models);
    }
  }


void KNSAddUPS::showManufact (void) {

  m_manufactBox->clear();
  QStringList  manufactList = m_commonData->getManufactList();

  for (int i = 0; i < manufactList.count(); i++) {
    m_manufactBox->addItem(manufactList.at(i));
    }
  }


void KNSAddUPS::parseUPSXMLFile (void) {
  QFile* xmlFile;
  QXmlSimpleReader reader;
  QString filename;
  //parses XML file with informations about modules


  filename = KGlobal::dirs()->findResource("data","knutsetting/upses_def.xml");

  xmlFile = new QFile (filename);
  if (xmlFile->open(QIODevice::ReadOnly)) {
    KNSUPSSaxHandler* handler = new KNSUPSSaxHandler(m_commonData);
    reader.setContentHandler(handler);
    reader.setErrorHandler(handler);
    reader.parse(xmlFile);

    xmlFile->close();
    delete handler;
    }
  delete xmlFile;
  m_commonData->sortManufacts();
  }

void KNSAddUPS::setButtonOK (void) {
    modulesList::const_iterator moduleIt;

  if ((m_nameLineEdit->text().trimmed()).isEmpty()) {
    enableButtonOk(false);
    return;
    }

  for (moduleIt=m_modulesData->begin(); moduleIt !=m_modulesData->end();++moduleIt ) {
    if ((*moduleIt).name == m_nameLineEdit->text()) {
      enableButtonOk(false);
      return;
      }
    }
  if ((m_portAuto->isEnabled() && m_portAuto->isChecked()) || (m_portDevCombo->count() > 0)) enableButtonOk(true);
  else enableButtonOk(true);  
  }


//----------- SLOTS --------------------------


void KNSAddUPS::manufactChangedSlot (void) {

  if (m_manufactBox->selectedItems().count() > 0) {
    QListWidgetItem *firstSelectedItem =  m_manufactBox->selectedItems().first();
    if (firstSelectedItem != NULL) { 
      if (firstSelectedItem != m_selectedManufactItem) {
        m_modelBox->clear();
        manufactList::iterator manufactIt = m_commonData->findManufact(firstSelectedItem->text());
        for (modelsList::const_iterator modelIt = (*manufactIt).models.begin(); modelIt != (*manufactIt).models.end(); modelIt++) {
          m_modelBox->addItem((*modelIt).modelName);
          }
        m_selectedManufactItem=firstSelectedItem;
        m_driverSeletedList=0;
        disableDriverPart();
        }
      }
    }
  }



void KNSAddUPS::modelChangedSlot (void) {

  if (m_modelBox->selectedItems().count() > 0) {
    QListWidgetItem *firstSelectedItem =  m_modelBox->selectedItems().first();

      if (firstSelectedItem != NULL) {
      clearDriverPart();
      m_driverCombo->setEnabled(true);
      m_nameLineEdit->setEnabled(true);

      m_driverSeletedList = m_commonData->findDriverList(m_manufactBox->currentItem()->text(),firstSelectedItem->text());
      if (m_driverSeletedList != 0) {


        for (driversListId::iterator  driverListId = m_driverSeletedList->begin(); driverListId != m_driverSeletedList->end(); driverListId++) {
          int driverId = (*driverListId);  // id of drivers
          upsModelDriversList* modelDrivers = m_commonData->driverDataAddr();
          upsModelDriverStruct modelDr;
          modelDr = modelDrivers->at(driverId);
          m_driverCombo->addItem(modelDr.driverName);
          }
        if (m_driverCombo->count()> 0) driverSelectedSlot (m_driverCombo->currentIndex());
        }
      }
    }
  }

void KNSAddUPS::driverSelectedSlot (int index) {
  modulesDefList::iterator modulesListIt;
  modulesDefList* modulesList;


  if (m_driverSeletedList == 0) return;
  if (index >= m_driverSeletedList->count()) return;

  modulesList = m_commonData->modulesDefAddr();
  m_connectLabel->setText("");
  m_portDevCombo->clear();
  m_desc->setPlainText("");
  m_portDevCombo->setEnabled(true);
  m_portAdd->setEnabled(true);
  m_portAuto->setChecked(false);

  int driverListId = (m_driverSeletedList->at(index));

  m_driverCurrentId = driverListId;
 
  QString modelDesc(m_commonData->getModelDescription(m_manufactBox->currentItem()->text(), m_modelBox->currentItem()->text(), index));
 

 
 if (!modelDesc.isEmpty()) {
   modelDesc = i18n("Model information :\n") + modelDesc;
//   m_desc->setPlainText(m_commonData->getModelDescription(m_manufactBox->currentItem()->text(), m_modelBox->currentItem()->text(), index));
   m_desc->setPlainText( modelDesc);
   }
    //save list of params to pointer

  if (m_commonData->modulesDefExist(m_driverCombo->itemText(index))) {

  modulesListIt = m_commonData->modulesDefFind(m_driverCombo->itemText(index));

    if (!((*modulesListIt).description.isEmpty())) {
      QString allText(m_desc->toPlainText());
      if (!(allText.isEmpty())) {
        allText.append("\n");
        allText.append(i18n("Driver information : \n"));
        }
      allText.append((*modulesListIt).description);
      m_desc->setPlainText(allText);
      }
    
     if ((*modulesListIt).portAuto == PORT_AUTO_ONLY)  m_portAutoOnly = true;
     else m_portAutoOnly = false;


     if (((*modulesListIt).portAuto == PORT_AUTO_YES) || ((*modulesListIt).portAuto == PORT_AUTO_ONLY))
        m_portAuto->setChecked(true);
     else
       m_portAuto->setChecked(false);

     if  (((*modulesListIt).portAuto == PORT_AUTO_ONLY) || ((*modulesListIt).portAuto == PORT_AUTO_NO))
       m_portAuto->setEnabled(false);
     else
       m_portAuto->setEnabled(true);


     if ((*modulesListIt).portType==PORT_SERIAL) m_connectLabel->setText(i18n("Serial"));
     else {
       if ((*modulesListIt).portType==PORT_USB) m_connectLabel->setText(i18n("USB"));
       else m_connectLabel->setText("");
       }

      if (modulesListIt != modulesList->end())  {
      m_portDevCombo->addItems((*modulesListIt).portNames);
      }
    }
  else {

      m_desc->setPlainText("Driver for this model of UPS  is not in list of drivers.");
    }
  }


void KNSAddUPS::getNewPortSlot(void) {
  bool portExist;
  int i;

  QString newPort = KFileDialog::getOpenFileName (KUrl("/dev"));
  portExist=false;
  if (!(newPort.isEmpty())) {
    for (i=0 ; i< m_portDevCombo->count(); i++) {
      if (m_portDevCombo->itemText(i) == newPort) {
        portExist=true;
        break;
        }
      }
    if (!portExist) {
       m_portDevCombo->addItem(newPort);
       m_portDevCombo->setCurrentIndex (m_portDevCombo->count()-1);
       setButtonOK();
      }
    }
  }

void KNSAddUPS::autoToggledSlot (bool on) {

  m_portDevCombo->setDisabled(on);
  m_portAdd->setDisabled(on);
  setButtonOK();
  }


void KNSAddUPS::nameChangedSlot (const QString&) {

  setButtonOK();
  }


void KNSAddUPS::addModuleSlot (void) {
  moduleStruct newModule;
  moduleItemStruct newItem;
  ParamsDriverList::const_iterator paramIt;

  newModule.name=m_nameLineEdit->text();

//  if (m_portAuto->isEnabled() && m_portAuto->isChecked())
//    newModule.port="auto";
//  else newModule.port = m_portDevCombo->currentText();

  if (m_portAuto->isChecked()) newModule.portFileName="";
  else newModule.portFileName = m_portDevCombo->currentText();

  if (m_portAuto->isChecked()) newModule.autoPort = true;
  else newModule.autoPort = false;

//  newModule.autoPortOnly=m_portAutoOnly;

  newModule.driver=m_driverCombo->currentText();
  //set default value
  newModule.sdOrder=sdOrderDef;
  newModule.sdOrderFind=false;
  
  upsModelDriversList* modelDriverListAddr = m_commonData->driverDataAddr();

  newModule.noLockFind =  (modelDriverListAddr->at(m_driverCurrentId)).driverNoLockFound;
  newModule.maxStartDelay= (modelDriverListAddr->at(m_driverCurrentId)).driverMaxStartDelay;
  newModule.maxStartDelayFind=(modelDriverListAddr->at(m_driverCurrentId)).driverMaxStartDelayFound;

  newModule.desc=m_desc->toPlainText();
  newModule.items.clear();
if (m_driverCurrentId != - 1) {
    for (paramIt=(modelDriverListAddr->at(m_driverCurrentId)).paramsList.begin();paramIt!=(modelDriverListAddr->at(m_driverCurrentId)).paramsList.end(); paramIt++) {
      newItem.itemName=(*paramIt).paramName;
      newItem.value=(*paramIt).value;
      newItem.itemFound=true;
      newModule.items.push_back(newItem);
      }
    }
  m_modulesData->append(newModule);
  emit upsAdded(newModule.name);
  }

#include "knsaddups.moc"
