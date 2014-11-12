/***************************************************************************
                          knsmodusrval.cpp  -  description
                             -------------------
    begin                : Po b?e 14 2005
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

#include "knsmodusrval.h"
#include "knsaddparam.h"


#include <klocale.h>

#include <qstring.h>
#include <qstringlist.h>
#include <qwidget.h>
#include <qlayout.h>
#include <qlabel.h>


//#include <iostream>


KNSModUsrVal::KNSModUsrVal(QWidget *parent )
             : QWidget(parent) {

  m_itemLayout=0;
  //lost of input widgets
  m_valueInfoList.clear();
  m_valueInfoList.reserve(4);



  m_addButton = new KPushButton(i18n("Add"),this);


//  modulesDefList* modDefList = commonData->modulesDefAddr();

  QVBoxLayout* mainLayout = new QVBoxLayout(this);
  mainLayout->setSizeConstraint(QLayout::SetMinimumSize);

  m_gridEnvLayout = new QHBoxLayout();
  m_gridEnvLayout->setSizeConstraint(QLayout::SetMinimumSize);
  mainLayout->addLayout(m_gridEnvLayout);
  QHBoxLayout* hLayout = new QHBoxLayout();
  hLayout->setSizeConstraint(QLayout::SetMinimumSize);
  mainLayout->addLayout(hLayout);
  hLayout->addStretch(10);
  hLayout->addWidget(m_addButton);
//  mainLayout->addSpacing(5);
  mainLayout->addStretch(10);


  connect (m_addButton,SIGNAL(clicked()),this, SLOT( addParamSlot()));
  }

KNSModUsrVal::~KNSModUsrVal(){

  }


void KNSModUsrVal::clear(void) {
  QVector<valueInfoStruct>::iterator it;

 //deletes all rows
  if (m_itemLayout) {
    m_gridEnvLayout->removeItem(m_itemLayout);
    delete m_itemLayout;
    m_itemLayout=0;
    }

  for (it=m_valueInfoList.begin();it!=m_valueInfoList.end();it++) {
    if ((*it).label != 0) {
      (*it).label->hide();
      delete (*it).label;
      }
    if ((*it).value != 0) delete (*it).value;
    (*it).name="";
    }
  m_valueInfoList.clear();
  }


void KNSModUsrVal::init(modulesDefList::const_iterator moduleDefIt, modulesList::iterator moduleIt) {
  valueInfoStruct newValueInfo;
  KNSValueEdit* valueEdit;
  moduleItems::const_iterator it;
  paramDefList::const_iterator modDefParmIt;
  bool itemFound;

  clear();

  m_itemLayout = new QGridLayout();
  m_itemLayout->setSizeConstraint(QLayout::SetMinimumSize);
  m_gridEnvLayout->addLayout(m_itemLayout);

  for (it = (*moduleIt).items.begin();it != (*moduleIt).items.end();it++) {
    itemFound=false;
    for (modDefParmIt = (*moduleDefIt).param.begin();modDefParmIt != (*moduleDefIt).param.end(); modDefParmIt++) {
      if ((*modDefParmIt).name == (*it).itemName) {
        itemFound=true;
        break;
        }
      }

    if (!itemFound) {
      QLabel* valueLabel = new QLabel((*it).itemName+" : ",this);
      valueLabel->show();
      m_itemLayout->addWidget(valueLabel,m_itemLayout->rowCount(),0);
      newValueInfo.label= valueLabel;
      newValueInfo.name= ((*it).itemName);
      valueEdit = new KNSValueEdit(this,VW_STRING,true,true);
      valueEdit->setText((*it).value);
      valueEdit->setChecked(true);
      valueEdit->setReadOnly(true);
      valueEdit->show();
      //must decrease numRows because row is added
      m_itemLayout->addWidget(valueEdit,m_itemLayout->rowCount()-1,1);
      newValueInfo.value= valueEdit;

      m_valueInfoList.push_back(newValueInfo);
      //put data to (*valIt).value->setValue() valueEdit
      }

    }
  m_itemLayout->setColumnStretch(1,5);
  }


void KNSModUsrVal::putData(modulesDefList::const_iterator moduleDefIt, modulesList::iterator moduleIt) {
  moduleItems::const_iterator it;
//  QValueVector<valueInfoStruct>::iterator valIt;
//  bool itemFound;

//  selectedModuleIt = moduleIt;
  m_nameParamsList.clear();
  for (it = (*moduleIt).items.begin();it != (*moduleIt).items.end();it++) m_nameParamsList.push_back((*it).itemName);

  // activate init
//  if ( myModuleDefIt != moduleDefIt) {
    init(moduleDefIt,moduleIt);
//    myModuleDefIt = moduleDefIt;
//    }


//  for (it = (*moduleIt).items.begin();it != (*moduleIt).items.end();it++) {
//    for (valIt = valueInfoList.begin();valIt != valueInfoList.end();valIt++) {
//      if ((*valIt).name == ((*it).itemName)) {
//        (*valIt).value->setText((*it).value);
//        (*valIt).value->setChecked(true);
//        //put data to (*valIt).value->setValue() valueEdit
//        break;
//        }
//      }
//    }
  }


void KNSModUsrVal::getData( modulesList::iterator moduleIt) {
  moduleItems::iterator it;
  QVector<valueInfoStruct>::const_iterator valIt;
  bool itemFound;

 for (valIt = m_valueInfoList.begin();valIt != m_valueInfoList.end();valIt++) {
   itemFound=false;
   for (it = (*moduleIt).items.begin();it != (*moduleIt).items.end();it++) {
      if ((*valIt).name == ((*it).itemName)) {
        (*it).value=(*valIt).value->text();
        (*it).itemFound=(*valIt).value->isChecked();
        itemFound=true;
        }
      }
    // item is checked and one don't exist in list of values
    // adds item into list of values
    if ((!itemFound) && (*valIt).value->isChecked()) {
      moduleItemStruct newValue;
      newValue.itemName=(*valIt).name;
      newValue.value=(*valIt).value->text();
      newValue.itemFound=true;
      (*moduleIt).items.push_back(newValue);
      }
    }
  }

void KNSModUsrVal::setReadOnly( const bool readOnly ) {
  QVector<valueInfoStruct>::iterator it;

  for (it=m_valueInfoList.begin();it!=m_valueInfoList.end();it++) {
    if ((*it).value != 0) (*it).value->setReadOnly(readOnly);
    }
  m_addButton->setDisabled(readOnly);  
  }


//-------------SLOTS-------------------

void KNSModUsrVal::addParamSlot( void ) {
  moduleItems::const_iterator it;

  KNSAddParam* addNewParam = new KNSAddParam (&m_nameParamsList,this);
  connect (addNewParam,SIGNAL(paramAdded (const QString, const QString)),this,SLOT (addedNewParamSlot(const QString,const QString)));
  addNewParam->exec();
  disconnect (addNewParam,SIGNAL(paramAdded (const QString, const QString)),this,SLOT (addedNewParamSlot(const QString,const QString)));
  delete addNewParam;
  }

void KNSModUsrVal::addedNewParamSlot(const QString name,const QString value) {
  valueInfoStruct newValueInfo;
  KNSValueEdit* valueEdit;


  QLabel* valueLabel = new QLabel(name+" : ",this);
  valueLabel->show();
  m_itemLayout->addWidget(valueLabel,m_itemLayout->rowCount(),0);
  newValueInfo.label= valueLabel;
  newValueInfo.name= (name);
  valueEdit = new KNSValueEdit(this,VW_STRING,true,true);
  valueEdit->setText(value);
  valueEdit->setChecked(true);
  valueEdit->setReadOnly(false);
  valueEdit->show();
  //must decrease numRows because row is added
  m_itemLayout->addWidget(valueEdit,m_itemLayout->rowCount()-1,1);
  newValueInfo.value= valueEdit;

  m_valueInfoList.push_back(newValueInfo);
  // added param's name into list of params
  m_nameParamsList.push_back(name);
  }


#include "knsmodusrval.moc"
