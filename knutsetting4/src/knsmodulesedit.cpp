/***************************************************************************
                          knsmodulesedit.cpp  -  description
                             -------------------
    begin                : St ?íj 20 2004
    copyright            : (C) 2004 by Daniel Prynych,,,
    email                : Daniel at prynych@ dot cz
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "knsmodulesedit.h"

#include <klocale.h>
#include <kglobal.h>
#include <KDE/KFileDialog>
#include <KDE/KIcon>
#include <KDE/KUrl>
#include <KDE/KMessageBox>

#include <qlayout.h>
#include <qwidget.h>
#include <qgroupbox.h>
#include <qpixmap.h>
#include <qfontmetrics.h>
#include <QtGui/QScrollArea>
#include <qtabwidget.h>
#include <qcheckbox.h>

//#include <iostream>

KNSModulesEdit::KNSModulesEdit(KNSCommonData* commonData, QWidget *parent )
               :QLabel(parent), m_commonData(commonData) {

  QTabWidget* valueTab;

  KIcon choosePix = KIcon("fileopen");

  QLabel* modelLabel = new QLabel(i18n("Driver")+" : ",this);
  m_modelNameLabel = new QLabel(this);
  QGroupBox* portBox = new QGroupBox(i18n("Port"),this);

  QVBoxLayout* topLayout = new QVBoxLayout(this);
  topLayout->setSizeConstraint(QLayout::SetMinimumSize);
  QHBoxLayout* modelLayout = new QHBoxLayout();
  modelLayout->setSizeConstraint(QLayout::SetMinimumSize);
  QGridLayout* gridPortLayout = new QGridLayout (portBox);
  gridPortLayout->setSizeConstraint(QLayout::SetMinimumSize);
  gridPortLayout->setColumnStretch(0,5);
  QLabel* portLabel = new QLabel (i18n("Type")+" : ",portBox);
  m_portTypeLabel = new QLabel(portBox);
  m_portAuto = new QCheckBox(i18n("Auto"),portBox);
  m_portDevCombo = new KComboBox(portBox);
  m_portDevLabel = new QLabel(portBox);
  m_portAdd = new KPushButton (portBox);
  m_portAdd->setIcon(KIcon(choosePix));


  valueTab = new QTabWidget(this);

  m_standardWidget= new KNSModStdVal(valueTab);
  valueTab->addTab(m_standardWidget,i18n("Standard part of setting"));

  QScrollArea* driverView = new QScrollArea(valueTab);
  m_driverWidget= new KNSModDrivVal(driverView->viewport());
  driverView->setWidget(m_driverWidget);
  driverView->setFrameStyle(QFrame::Panel);
  driverView->setWidgetResizable(true);
  valueTab->addTab(driverView,i18n("Driver part of setting"));

  QScrollArea* userView = new QScrollArea(valueTab);
  m_userWidget= new KNSModUsrVal(userView->viewport());
  userView->setWidget(m_userWidget);
  userView->setFrameStyle(QFrame::Panel);
  userView->setWidgetResizable(true);
  valueTab->addTab(userView,i18n("User part of setting"));


  topLayout->addLayout(modelLayout);
  topLayout->addWidget(portBox);
  topLayout->addWidget(valueTab);
  topLayout->setStretchFactor(valueTab,5);
  modelLayout->addWidget(modelLabel);
  modelLayout->addWidget(m_modelNameLabel);
  modelLayout->addStretch(10);
  QHBoxLayout* hLabelPortLayout = new QHBoxLayout ();
  QHBoxLayout* hDevPortLayout = new QHBoxLayout ();


  gridPortLayout->addItem(new QSpacerItem(0,fontMetrics().height()/2), 0, 0);
  gridPortLayout->addItem(new QSpacerItem(0,fontMetrics().height()/2), 0, 0);
  gridPortLayout->addLayout(hLabelPortLayout,1,0);
  gridPortLayout->addWidget(m_portAuto,1,1);
  gridPortLayout->addLayout(hDevPortLayout,2,0);
  gridPortLayout->addWidget(m_portAdd,2,1);

  hLabelPortLayout->addWidget(portLabel);
  hLabelPortLayout->addWidget(m_portTypeLabel);
  hLabelPortLayout->addStretch(10);

  hDevPortLayout->addWidget(m_portDevLabel);
  hDevPortLayout->addWidget(m_portDevCombo);


  clear();
  connect ( m_portAdd, SIGNAL(released()), this, SLOT(getNewPortSlot()));
  connect ( m_portDevCombo, SIGNAL(highlighted(int)),this, SLOT(setPortLabelSlot(int)));
  connect (m_portAuto,SIGNAL (clicked()),this, SLOT(portAutoUsedSlot()));

  m_autoPort=PORT_AUTO_YES;
  setReadOnly(true);
  }

KNSModulesEdit::~KNSModulesEdit(){
  }


void KNSModulesEdit::setReadOnly (const bool readOnly) {

  if (readOnly) { // show only mode 
    m_portAuto->setEnabled(false);
    m_portDevLabel->show();
    m_portDevCombo->hide();
    m_portAdd->hide();
    }
  else {  // edit mode
    

    if ((m_autoPort == PORT_AUTO_ONLY) || (m_autoPort == PORT_AUTO_NO)) m_portAuto->setEnabled(false);
    else m_portAuto->setEnabled(true);

  
      if ((m_autoPort == PORT_AUTO_ONLY) || (m_autoPort == PORT_AUTO_YES) || (m_portAuto->isChecked())) {
      m_portDevLabel->hide();
      m_portDevCombo->hide();
      m_portAdd->hide();
      }
    else {
      m_portDevLabel->hide();
      m_portDevCombo->show();
      m_portAdd->show();
      }
    }
  m_standardWidget->setReadOnly(readOnly);
  m_driverWidget->setReadOnly(readOnly);
  m_userWidget->setReadOnly(readOnly);
  }

void KNSModulesEdit::getData( modulesList::iterator moduleIt ) {


  (*moduleIt).portFileName=m_portDevLabel->text();
  m_standardWidget->getData(moduleIt);
  m_driverWidget->getData(moduleIt);
  m_userWidget->getData(moduleIt);
  }

void KNSModulesEdit::putData( modulesList::iterator moduleIt ) {

  int i;
  int itemFound;
  modulesDefList::const_iterator modDefIt;
  QStringList::const_iterator itPN;

  m_autoPort=PORT_AUTO_YES;

  modulesDefList* modDefList = m_commonData->modulesDefAddr();

  //find def of module
  modDefIt = findModDefIt((*moduleIt).driver);
  // modDefIt is iterator of structure of modules structure

  if (modDefIt == modDefList->end()) {
    KMessageBox::sorry (0,i18n("Driver %1 for this module isn't defined in list of drivers.").arg((*moduleIt).driver));
    }


  m_modelNameLabel->setText((*moduleIt).driver);
  m_portDevLabel->setText((*moduleIt).portFileName);

//from list of driver
  if (modDefIt != modDefList->end()) {
    switch ((*modDefIt).portType) {
      case PORT_SERIAL :
         m_portTypeLabel->setText(i18n("Serial"));
         break;
      case PORT_USB :
         m_portTypeLabel->setText(i18n("USB"));
         break;
      default:
         m_portTypeLabel->setText(i18n("Unknown"));
      }
    }
  else m_portTypeLabel->setText(i18n("Unknown"));    

  if (modDefIt != modDefList->end()) {
    m_autoPort = (*modDefIt).portAuto; 
    if ((*modDefIt).portAuto == PORT_AUTO_ONLY) {
      m_portAuto->setChecked(true);
      m_portAuto->setEnabled(false);  
      }
    else {
      if ((*modDefIt).portAuto == PORT_AUTO_YES) 
        {
        m_portAuto->setChecked(true);
        }
      else {
        m_portAuto->setChecked(false);
        }
      }
    }

  m_portDevCombo->clear();
  itemFound=-1;
  i=0;

  if (modDefIt != modDefList->end()) {
    for (itPN = (*modDefIt).portNames.begin(); itPN !=(*modDefIt).portNames.end(); itPN++) {
      m_portDevCombo->insertItem(m_portDevCombo->count(),*itPN);
      if ((*itPN) == (*moduleIt).portFileName) {
        itemFound=i;
        }
      i++;
      }
    }

  if (itemFound != -1) m_portDevCombo->setCurrentIndex(itemFound);
  else{
    if (!(*moduleIt).autoPort) {
      m_portDevCombo->insertItem(m_portDevCombo->count(),(*moduleIt).portFileName);
      m_portDevCombo->setCurrentIndex (m_portDevCombo->count()-1);
      }
    }

  m_portAdd->hide();
  m_standardWidget->putData(moduleIt);

  if (modDefIt != modDefList->end()) {
    m_driverWidget->putData(modDefIt,moduleIt);
    m_userWidget->putData(modDefIt,moduleIt);
    }
  else {  // definition of this module doesn't exist
    m_driverWidget->clear();
    m_userWidget->clear();
    }
  }

void KNSModulesEdit::clear( void) {

  m_modelNameLabel->setText("");
  m_portTypeLabel->setText("");
  m_portDevLabel->setText("");
  m_portDevCombo->clear();
  m_portAuto->setChecked(false);

  m_standardWidget->clear();
  m_driverWidget->clear();
  m_userWidget->clear();
  }


void KNSModulesEdit::init( void) {

  m_oldPort="";
  clear();
  }

modulesDefList::const_iterator KNSModulesEdit::findModDefIt (QString driverName) {
  modulesDefList::const_iterator it;

  modulesDefList* modDefList = m_commonData->modulesDefAddr();
  for (it = modDefList->begin(); it != modDefList->end(); it++) {
    if ((*it).name == driverName) return it;
    }
  return modDefList->end();
  }

//--------------------------- SLOTS --------------------
void KNSModulesEdit::getNewPortSlot(void) {
  bool portExist;
  int i;

  QString newPort = KFileDialog::getOpenFileName (KUrl("/dev"),"");
  portExist=false;
  if (!(newPort.isEmpty())) {
    for (i=0 ; i< m_portDevCombo->count(); i++) {
      if (m_portDevCombo->itemText(i) == newPort) {
        portExist=true;
        break;
        }
      }
    if (!portExist) {
       m_portDevCombo->insertItem(m_portDevCombo->count(),newPort);
       m_portDevCombo->setCurrentIndex (m_portDevCombo->count()-1);
      }
    }
  }


void KNSModulesEdit::setPortLabelSlot (int index) {

   m_oldPort=m_portDevLabel->text();
   m_portDevLabel->setText(m_portDevCombo->itemText(index));
   }

void KNSModulesEdit::portAutoUsedSlot(void) {

  if (m_portAuto->isChecked()) {
    m_portDevCombo->hide();
    m_oldPort=m_portDevLabel->text();
    m_portDevLabel->hide();
    m_portAdd->hide();
    }
  else {
    m_portDevLabel->setText(m_portDevCombo->currentText());
    m_portDevCombo->show();
    m_portDevLabel->hide();
    m_portAdd->show();
    }
  }

#include "knsmodulesedit.moc"
