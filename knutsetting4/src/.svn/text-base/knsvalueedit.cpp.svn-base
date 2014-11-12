/***************************************************************************
                          knsvalueedit.cpp  -  description
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

#include "knsvalueedit.h"


#include <QtGui/QWidget>
#include <QtCore/QString>
#include <QtGui/QCheckBox>
#include <QtGui/QHBoxLayout>



KNSValueEdit::KNSValueEdit(QWidget *parent, const typeOfValueWidget type, const bool force, const bool variable )
            : QLabel(parent), m_type(type), m_force(force), m_variable(variable) {

  m_minNum=0;
  m_maxNum=99;

  m_defNum=0;
  m_defString="";

  m_comboList=0;

  QHBoxLayout* mainLayout = new QHBoxLayout(this);
  mainLayout->setSizeConstraint(QLayout::SetMinimumSize);

  m_checkBox = new QCheckBox(this);

  switch (m_type) {
    case VW_STRING:
      m_valueStringWidget = new KLineEdit(this);
      m_valueStringWidget->setMinimumSize(150,m_valueStringWidget->minimumSize().height());
      mainLayout->addWidget(m_valueStringWidget);
      mainLayout->setStretchFactor(m_valueStringWidget,5);
      if (!m_variable) m_valueStringWidget->setReadOnly(true);
    break;
    case VW_NUMBER:
      m_valueIntWidget = new KIntNumInput(this);
      mainLayout->addWidget(m_valueIntWidget);
      mainLayout->setStretchFactor(m_valueIntWidget,5);
      m_valueIntWidget->setMinimum(m_minNum);
      m_valueIntWidget->setMaximum(m_maxNum);
    break;
    case VW_COMBO:
      m_valueComboWidget = new KComboBox(this);
      QHBoxLayout* comboLayout = new QHBoxLayout();
      comboLayout->setSizeConstraint(QLayout::SetMinimumSize);
      mainLayout->addLayout(comboLayout);
      comboLayout->addWidget(m_valueComboWidget);
      mainLayout->setStretchFactor(comboLayout,5);
      m_comboList = new QList<QString>;
      connect (m_valueComboWidget, SIGNAL(activated(const QString&)),this,SLOT(comboHighlightedSlot (const QString&)));
      m_comboLabel = new QLabel(this);
      comboLayout->addWidget(m_comboLabel);
    break;
    }
  mainLayout->addWidget(m_checkBox);

  if (m_force) {
    m_checkBox->setChecked(true);
    m_checkBox->setEnabled(false);
    }
  else m_checkBox->setChecked(false);
  setSizePolicy (QSizePolicy::Minimum, QSizePolicy::Minimum);

  connect (m_checkBox,SIGNAL(clicked()),this,SLOT(valueUsedSlot()));
  }

KNSValueEdit::~KNSValueEdit(){
  if (m_type==VW_COMBO) delete m_comboList;
  }

void KNSValueEdit::setReadOnly (bool readOnly) {

  m_checkBox->setEnabled(!readOnly);
  switch (m_type) {
    case VW_STRING:
      if ((!m_checkBox->isChecked()) || (readOnly) || (!m_variable)) m_valueStringWidget->setReadOnly(true);
      else m_valueStringWidget->setReadOnly(false);
    break;
    case VW_NUMBER:
      if ((!m_checkBox->isChecked()) || readOnly)  m_valueIntWidget->setEnabled(false);
      else m_valueIntWidget->setEnabled(true);
    break;
    case VW_COMBO:
      if ((!m_checkBox->isChecked()) || readOnly) {
        m_valueComboWidget->setEnabled(false);
        m_valueComboWidget->hide();
        m_comboLabel->show();
        }
      else {
        m_valueComboWidget->setEnabled(true);
        m_valueComboWidget->show();
        m_comboLabel->hide();
        }
    break;
    }
  }


QSize KNSValueEdit::sizeHint() const {

  return minimumSizeHint();
  }


QSize KNSValueEdit::minimumSizeHint() const {
  int w ;
  int h = m_checkBox->height();

  switch (m_type) {
    case VW_STRING:
    w = (m_valueStringWidget->sizeHint()).width();
    if (h < m_valueStringWidget->height())
       h = m_valueStringWidget->height();
    break;
    case VW_NUMBER:
    w = (m_valueIntWidget->minimumSizeHint()).width();
    if (h < m_valueIntWidget->height())
       h = m_valueIntWidget->height();
    break;
    case VW_COMBO:
    if (m_checkBox->isChecked()) {
      w = m_valueComboWidget->width();
      h= qMax(h,m_valueComboWidget->height()); 
//      if (h < m_valueComboWidget->height())
//         h = m_valueComboWidget->height();
      }
    else {
      w = m_comboLabel->width();
      h= qMax(h,m_comboLabel->height()); 
//      if (h < m_comboLabel->height())
//         h = m_comboLabel->height();
      }
    break;
    }
 
  w += this->layout()->spacing();
  w += m_checkBox->width();

  w = 150;
  return QSize(w, h);
  }


void KNSValueEdit::setText (QString value) {

  switch (m_type) {
    case VW_STRING:
    if (m_variable) m_valueStringWidget->setText(value);
    break;
    case VW_NUMBER:
    if (m_variable) m_valueIntWidget->setValue(value.toInt());
    break;
    case VW_COMBO:
    int i = 0;
    QList<QString>::const_iterator CLIt;
    for (CLIt=m_comboList->begin(); CLIt!=m_comboList->end();CLIt++) {
      if ((*CLIt) == value) {
        m_valueComboWidget->setCurrentIndex(i);
        m_comboLabel->setText(m_valueComboWidget->currentText());
        break;
        }
      i++;
      }
    break;
    }
  }


void KNSValueEdit::setValue (int value) {

  if (m_type == VW_NUMBER) {
    if (m_variable) m_valueIntWidget->setValue(value);
    }
  }


bool KNSValueEdit::isChecked (void) {

  return m_checkBox->isChecked();
  }


int KNSValueEdit::value (void) {

  if (m_type==VW_NUMBER) {
      return m_valueIntWidget->value();  
    }
  return 0;
  }


QString KNSValueEdit::text (void) {
  QString retString;

  switch (m_type) {
    case VW_STRING:
      return m_valueStringWidget->text();
    break;
    case VW_NUMBER:
      retString.setNum(m_valueIntWidget->value());
      return retString;
    break;
    case VW_COMBO:
      if (m_valueComboWidget->currentIndex()== -1) return "";
      else {
        retString = (*(m_comboList->begin()+m_valueComboWidget->currentIndex()));
        return retString;
        }
    break;
    }
  return "";
  }



void KNSValueEdit::addItem(const QString value, const QString notify) {

  if (m_type == VW_COMBO) {
    m_valueComboWidget->insertItem(m_valueComboWidget->count(),value+" : ("+notify+")");
    m_comboList->push_back(value);
    if (m_valueComboWidget->count()==1) {
      m_comboLabel->setText(value+" : ("+notify+")");
      }
    }
  }

void KNSValueEdit::setMinValue (int min) {

  if (m_type == VW_NUMBER) {
    m_minNum=min;
    m_valueIntWidget->setMinimum(m_minNum);
    m_valueIntWidget->setMaximum(m_maxNum);
    }
  }

void KNSValueEdit::setMaxValue (int max) {

  if (m_type == VW_NUMBER) {
    m_maxNum=max;
    m_valueIntWidget->setMinimum(m_minNum);
    m_valueIntWidget->setMaximum(m_maxNum);
    }
  }

void KNSValueEdit::setDefNumValue (int def) {

  if (m_type == VW_NUMBER) {
    m_defNum=def;
    m_valueIntWidget->setValue(def);
    }
  }

void KNSValueEdit::setDefStringValue (QString def) {

  if (m_type == VW_STRING) {
    m_defString=def;
    m_valueStringWidget->setText(def);  
    }
  }


void KNSValueEdit::setDefComboValue (QString def) {
  int i;
  QList<QString>::const_iterator it;

  if (m_type == VW_COMBO) { //typeOfValueWidget
    i=0;
    for (it = m_comboList->begin(); it != m_comboList->end(); it++) {
    // tests if def is in combo list
      if ((*it) == def) {
        m_valueComboWidget->setCurrentIndex(i); //KComboBox
        m_comboLabel->setText(m_valueComboWidget->currentText());
        m_defString=def;
        break;
        }
      i++;
      }
    }
  }

void KNSValueEdit::setChecked (bool check) {

  if (check) m_checkBox->setChecked(true);
  else {
    if (!m_force) m_checkBox->setChecked(false); 
    }
  }


void KNSValueEdit::valueUsedSlot (void) {

  if (m_checkBox->isChecked()) {
    switch (m_type) {
      case VW_STRING:
      if (m_variable) m_valueStringWidget->setReadOnly(false);
      break;
      case VW_NUMBER:
      m_valueIntWidget->setEnabled(true);
      break;
      case VW_COMBO:
      m_valueComboWidget->setEnabled(true);
      m_valueComboWidget->show();
      m_comboLabel->hide();
      break;
      }
    }
  else {
    switch (m_type) {
      case VW_STRING:
      m_valueStringWidget->setReadOnly(true);
      break;
      case VW_NUMBER:
      m_valueIntWidget->setEnabled(false);
      break;
      case VW_COMBO:
      m_valueComboWidget->setEnabled(false);
      m_valueComboWidget->hide();
      m_comboLabel->show();
      break;
      }
    }
  }


void KNSValueEdit::comboHighlightedSlot (const QString & text) {

  m_comboLabel->setText(text);
  }

#include "knsvalueedit.moc"
