/***************************************************************************
                          knsdoublelistbox.cpp  -  description
                             -------------------
    begin                : Út srp 31 2004
    copyright            : (C) 2004 by Daniel Prynych,,,
    email                : Daniel Prynych@alo.cz
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "knsdoublelistbox.h"
#include "knsstring.h"

#include <QtGui/QListWidgetItem>
#include <QtGui/QListWidget>

#include <QtGui/QLayout>
#include <QtGui/QFontMetrics>
#include <QStringList>
#include <QtGui/QWidget>



KNSDoubleListBox::KNSDoubleListBox(QWidget *parent, const QString title ) : QGroupBox(parent) {

  QFontMetrics fm =  fontMetrics();
  setTitle(title);
  QVBoxLayout* mainLayout = new QVBoxLayout(this);
  mainLayout->setMargin(5);
  mainLayout->setSpacing(5);
  mainLayout->addSpacing ( fm.height());
  QHBoxLayout* lineLayout = new QHBoxLayout();
  mainLayout->addLayout(lineLayout);

  m_leftBox= new QListWidget(this);
  m_rightBox= new QListWidget(this);

  lineLayout->addWidget(m_leftBox);
  QVBoxLayout* buttonLayout = new QVBoxLayout();
  lineLayout->addLayout(buttonLayout);

  m_moveRightButton = new KPushButton(">",this);
  m_moveLeftButton = new KPushButton("<",this);
  buttonLayout->addWidget(m_moveRightButton);
  buttonLayout->addWidget(m_moveLeftButton);
  buttonLayout->addStretch(20);

  lineLayout->addWidget(m_rightBox);

  m_selectedItems = new QStringList();

  connect (m_moveLeftButton,SIGNAL(clicked()),this,SLOT(moveToLeftSlot()));
  connect (m_moveRightButton,SIGNAL(clicked()),this,SLOT(moveToRightSlot()));
  }


KNSDoubleListBox::~KNSDoubleListBox(){
  m_selectedItems->clear();
  delete m_selectedItems;
  }

void KNSDoubleListBox::clear (void) {

  m_leftBox->clear();
  m_rightBox->clear();
  }

void KNSDoubleListBox::setReadOnly (const bool readOnly) {

  m_moveRightButton->setEnabled(!readOnly);
  m_moveLeftButton->setEnabled(!readOnly);
  }


void KNSDoubleListBox::setMainList (const QStringList list) {

  m_leftBox->clear();
  for ( QStringList::const_iterator it = list.begin(); it != list.end(); ++it ) {
    m_leftBox->addItem((*it));
    }
  }



void KNSDoubleListBox::setSelectList (const QStringList list) {
  QList<QListWidgetItem *> listItems;

  m_selectedItems->clear();
  m_rightBox->clear();
  for ( QStringList::const_iterator it = list.begin(); it != list.end(); ++it ) {
    listItems = m_leftBox->findItems((*it),Qt::MatchExactly); // returns list of pointers of item
    for (int i = 0 ; i < listItems.count(); i++) {
      m_rightBox->addItem(listItems[i]->text());
      m_leftBox->takeItem(m_leftBox->row(listItems[i]));
      }   
    m_selectedItems->push_back((*it));
    }
  }


void KNSDoubleListBox::reviewSelectList (void ) {
  QList<QListWidgetItem *> listItems;
  QStringList::const_iterator it;

  m_rightBox->clear();
  for (QStringList::const_iterator it=m_selectedItems->begin(); it !=m_selectedItems->end(); it++) {
    listItems = m_leftBox->findItems((*it),Qt::MatchExactly); // returns list of pointers of item
    for (int i = 0 ; i < listItems.count(); i++) {
      m_rightBox->addItem(listItems[i]->text());
      m_leftBox->takeItem(m_leftBox->row(listItems[i]));
      }
    }
  }


void KNSDoubleListBox::renameSeletedItems(const QString oldName, const QString newName) {
  QStringList::iterator it;

  for (it=m_selectedItems->begin(); it !=m_selectedItems->end(); it++) {
    if ((*it) == oldName) (*it)=newName;
    }

  for (int i=0; i < m_rightBox->count(); i++) {
    if (m_rightBox->item(i)->text()==oldName) m_rightBox->item(i)->setText(newName);
    }
  for (int i=0; i < m_leftBox->count(); i++) {
    if (m_leftBox->item(i)->text()==oldName) m_leftBox->item(i)->setText(newName);
    }
  }

QStringList KNSDoubleListBox::getSelectList (void) {
  return *m_selectedItems;
  }


void KNSDoubleListBox::moveToLeftSlot(void) {
//  QListBoxItem* QListWidgetItem;
  QString name;
  QStringList::Iterator it;
  QList<QListWidgetItem *> listItems;
  
  listItems = m_rightBox->selectedItems();
  for (int i = 0; i < listItems.count(); i++) {
    name = listItems[i]->text();
    m_leftBox->addItem(name);
    m_rightBox->takeItem(m_rightBox->row(listItems[i]));
    for (it=m_selectedItems->begin(); it !=m_selectedItems->end(); it++) {
      if ((*it) == name) {
        m_selectedItems->erase(it);
        break;
        }
      }
    } 
  }


void KNSDoubleListBox::moveToRightSlot(void) {
  QList<QListWidgetItem *> listItems;


  listItems = m_leftBox->selectedItems();
  for (int i = 0; i < listItems.count(); i++) {
    m_rightBox->addItem(listItems[i]->text());
    // we must add item to inner list 
    m_selectedItems->push_back(listItems[i]->text());
    m_leftBox->takeItem(m_leftBox->row(listItems[i]));
    }
  }

#include "knsdoublelistbox.moc"
