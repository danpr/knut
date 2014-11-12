/***************************************************************************
                          knsmodstdval.cpp  -  description
                             -------------------
    begin                : So b?e 12 2005
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

#include "knsmodstdval.h"

#include <klocale.h>

#include <QtGui/QGroupBox>
#include <QtGui/QFont>
#include <QtGui/QLabel>
#include <QtGui/QCheckBox>
#include <QtCore/QString>
#include <QtGui/QLayout>



#include <iostream>



KNSModStdVal::KNSModStdVal(QWidget *parent)
            : QWidget(parent) {


  QLabel* label1 = new QLabel (i18n("Order of UPS")+" : ",this);// sdorder
  m_sdOrder = new KNSValueEdit(this,VW_NUMBER);
  QLabel* label2 = new QLabel (i18n("No lock")+" : ",this); //nolock
  m_noLockUsed = new QCheckBox (this);
  QLabel* label3 = new QLabel (i18n("Maximum delay of UPS's start")+" : ",this);
  m_maxStartDelay = new KNSValueEdit(this,VW_NUMBER);
  QGroupBox* descGroupBox = new QGroupBox(i18n("Description"),this);
  m_descTextEdit = new KTextEdit(descGroupBox);


  QVBoxLayout* mainLayout = new QVBoxLayout(this);
  mainLayout->setSizeConstraint(QLayout::SetMinimumSize);
  QGridLayout* itemLayout = new QGridLayout();
  itemLayout->setSizeConstraint(QLayout::SetMinimumSize);
  QVBoxLayout* descLayout = new QVBoxLayout(descGroupBox);
  descLayout->setSizeConstraint(QLayout::SetMinimumSize);
  m_noLockLayout = new QHBoxLayout();

  mainLayout->addLayout(itemLayout);
  mainLayout->addWidget(descGroupBox);
  mainLayout->setStretchFactor(descGroupBox,10);

  itemLayout->addWidget(label1,0,0);
  itemLayout->addWidget(m_sdOrder,0,1);
  itemLayout->addWidget(label2,1,0);
  itemLayout->addLayout (m_noLockLayout,1,1);
  itemLayout->addWidget(label3,2,0);
  itemLayout->addWidget(m_maxStartDelay,2,1);
  itemLayout->setColumnStretch(1,5);

  m_noLockLayout->addWidget(m_noLockUsed);

  descLayout->addWidget(m_descTextEdit);


  initValues();
  }


void KNSModStdVal::enabledWidgets(const bool readOnly){

  if  (readOnly){

    m_sdOrder->setReadOnly(true);
    m_noLockUsed->setEnabled(false);
    m_maxStartDelay->setReadOnly(true);
    m_descTextEdit->setReadOnly(true);
    }
  else {
    m_sdOrder->setReadOnly(false);
    m_noLockUsed->setEnabled(true);
    m_maxStartDelay->setReadOnly(false);
    m_descTextEdit->setReadOnly(false);
    }
  }


void KNSModStdVal::clear(void) {

  m_sdOrder->setDefNumValue(sdOrderDef);
  m_sdOrder->setChecked(false);

  m_noLockUsed->setChecked(false);

  m_maxStartDelay->setDefNumValue(sdOrderDef);
  m_maxStartDelay->setChecked(false);

  }


void KNSModStdVal::initValues(void) {

  clear();

  m_sdOrder->setReadOnly(true);
  m_maxStartDelay->setReadOnly(true);
  m_sdOrder->setMinValue(sdOrderMin);
  m_maxStartDelay->setMinValue(maxStartDelayMin);

  enabledWidgets(true);
  }


void KNSModStdVal::getData( modulesList::iterator moduleIt) {

   if (m_sdOrder->isChecked()) (*moduleIt).sdOrderFind=true;
   else (*moduleIt).sdOrderFind=false;
   (*moduleIt).sdOrder=m_sdOrder->value();

   if (m_noLockUsed->isChecked()) (*moduleIt).noLockFind=true;
   else (*moduleIt).noLockFind=false;

   if (m_maxStartDelay->isChecked()) (*moduleIt).maxStartDelayFind=true;
   else (*moduleIt).maxStartDelayFind=false;
   (*moduleIt).maxStartDelay=m_maxStartDelay->value();

   (*moduleIt).desc=m_descTextEdit->toPlainText();

   }

void KNSModStdVal::putData( modulesList::iterator moduleIt) {

  if ((*moduleIt).sdOrderFind) {
    m_sdOrder->setValue((*moduleIt).sdOrder);
    m_sdOrder->setChecked(true);
    }
  else {
    m_sdOrder->setDefNumValue(sdOrderDef);
    m_sdOrder->setChecked(false);
    }

  if ((*moduleIt).noLockFind) {
    m_noLockUsed->setChecked(true);
    }
  else {
    m_noLockUsed->setChecked(false);
    }

  if ((*moduleIt).maxStartDelayFind) {
    m_maxStartDelay->setValue((*moduleIt).maxStartDelay);
    m_maxStartDelay->setChecked(true);
    }
  else {
    m_maxStartDelay->setDefNumValue(maxStartDelayDef);
    m_maxStartDelay->setChecked(false);
    }
  m_descTextEdit->setText((*moduleIt).desc);
  }

void KNSModStdVal::setReadOnly( const bool readOnly ) {

   enabledWidgets(readOnly);
  }

KNSModStdVal::~KNSModStdVal(){

  delete m_noLockLayout;
  }

#include "knsmodstdval.moc"
