/***************************************************************************
                          knsmoddrivval.cpp  -  description
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

#include "knsmoddrivval.h"

#include <KDE/KLocale>

#include <QtGui/QLayout>
#include <QtCore/QString>
#include <QtGui/QLabel>

#include <QDebug>



KNSModDrivVal::KNSModDrivVal( QWidget *parent)
              : QWidget(parent) {


  m_itemLayout=0;
  m_valueInfoList.clear();
  m_valueInfoList.reserve(4);
  
  QVBoxLayout* mainLayout = new QVBoxLayout();
  mainLayout->setSizeConstraint(QLayout::SetMinimumSize);
  this->setLayout(mainLayout);

  m_gridEnvLayout = new QHBoxLayout();
  m_gridEnvLayout->setSizeConstraint(QLayout::SetMinimumSize);
  mainLayout->addLayout (m_gridEnvLayout);

  mainLayout->addStretch(10);
  }

KNSModDrivVal::~KNSModDrivVal(){
  }

void KNSModDrivVal::clear(void) {
  QVector<valueInfoDef>::iterator it;

 //deletes all rows
  if (m_itemLayout) {
    m_gridEnvLayout->removeItem(m_itemLayout);
    delete m_itemLayout;
    m_itemLayout=0L;
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

void KNSModDrivVal::init(modulesDefList::const_iterator moduleDefIt) {
  paramDefList::const_iterator paramIt;
  paramEnumItemList::const_iterator enumIt;
  valueInfoDef newValueInfo;
  KNSValueEdit* valueEdit;
  clear();

  m_itemLayout = new QGridLayout();
  m_itemLayout->setSizeConstraint(QLayout::SetMinimumSize);
  m_gridEnvLayout->addLayout(m_itemLayout);


  for ( paramIt=(*moduleDefIt).param.begin(); paramIt!=(*moduleDefIt).param.end(); paramIt++) {


    QLabel* valueLabel = new QLabel((*paramIt).name+" : ",this);

    valueLabel->show();
    m_itemLayout->addWidget(valueLabel,m_itemLayout->rowCount(),0);
    newValueInfo.label= valueLabel;
    newValueInfo.name= ((*paramIt).name);
    valueEdit=0L;
    switch ((*paramIt).type) {
      case PARAM_NUMBER:
        valueEdit = new KNSValueEdit(this,VW_NUMBER,(*paramIt).force);
        valueEdit->setMinValue((*paramIt).min);
        valueEdit->setMaxValue((*paramIt).max);
        valueEdit->setDefNumValue((*paramIt).parDefNum);
        valueEdit->setReadOnly(true);
      break;
      case PARAM_STRING:
        valueEdit = new KNSValueEdit(this,VW_STRING,(*paramIt).force,(*paramIt).variable);
        valueEdit->setDefStringValue((*paramIt).parDef);
        valueEdit->setReadOnly(true);
      break;
      case PARAM_ENUM:
        valueEdit = new KNSValueEdit(this,VW_COMBO,(*paramIt).force);
        //adds items of enum
        for (enumIt=(*paramIt).paramEnumItems.begin();enumIt!=(*paramIt).paramEnumItems.end();enumIt++) {
          valueEdit->addItem((*enumIt).value,(*enumIt).text);
          }
        valueEdit->setReadOnly(true);
        valueEdit->setDefComboValue((*paramIt).parDef);  
      break;
      }
    if (valueEdit !=0) {
      valueEdit->show();
      //must decrease numRows because row is added
      m_itemLayout->addWidget(valueEdit,m_itemLayout->rowCount()-1,1);
      newValueInfo.value= valueEdit;
      m_valueInfoList.push_back(newValueInfo);
      }
    }
  m_itemLayout->setColumnStretch(1,5);
  }

void KNSModDrivVal::putData(modulesDefList::const_iterator moduleDefIt, modulesList::iterator moduleIt) {
  moduleItems::const_iterator it;
  QVector<valueInfoDef>::iterator valIt;


  init(moduleDefIt);

  for (it = (*moduleIt).items.begin();it != (*moduleIt).items.end();it++) {  // date from file  // it is iterator on moduleItemStruct
    for (valIt = m_valueInfoList.begin();valIt != m_valueInfoList.end();valIt++) {


      if ((*valIt).name == ((*it).itemName)) {
        (*valIt).value->setText((*it).value);
        (*valIt).value->setChecked(true);
        //put data to (*valIt).value->setValue() valueEdit
        break;
        }
      }
    }
  }

void KNSModDrivVal::getData( modulesList::iterator moduleIt) {
  moduleItems::iterator it;
  QVector<valueInfoDef>::const_iterator valIt;
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
    // item is changed and one don't exist in list of values
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


void KNSModDrivVal::setReadOnly( const bool readOnly ) {
  QVector<valueInfoDef>::iterator it;

  for (it=m_valueInfoList.begin();it!=m_valueInfoList.end();it++) {
    if ((*it).value != 0) (*it).value->setReadOnly(readOnly);
    }
  }


#include "knsmoddrivval.moc"
