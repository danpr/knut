/***************************************************************************
                          knsaddparam.cpp  -  description
                             -------------------
    begin                : So ?en 25 2005
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

#include "knsaddparam.h"

#include <KDE/KLocale>

#include <QtGui/QGroupBox>
#include <QtGui/QFontMetrics>
#include <QtGui/QLayout>
#include <QtCore/QString>
#include <QtGui/QLabel>
#include <QtGui/QFrame>

#include <QtGui/QWidget>
#include <QtCore/QStringList>




KNSAddParam::KNSAddParam(QStringList* nameList, QWidget *parent )
          : KDialog(parent), m_nameList(nameList) {

  setButtons( KDialog::Ok | KDialog::Cancel);
  setModal(true);
  setInitialSize(QSize(450,600));


  QWidget* mWidget = new QWidget(this);
  setMainWidget(mWidget);

  QVBoxLayout* topLayout = new QVBoxLayout(mWidget);
  

  QLabel *label1 = new QLabel(i18n("Add other parameter."),mWidget);
  topLayout->addWidget(label1);

  QFrame* topLine = new QFrame(mWidget);
  topLine->setFrameStyle(QFrame::HLine);
  topLine->setLineWidth(1);
  topLine->setMidLineWidth(0);
  topLine->setFrameShadow(QFrame::Sunken);
  topLayout->addWidget(topLine);

  QGridLayout* gridLayout = new QGridLayout();
  gridLayout->setMargin(5);
  topLayout->addLayout(gridLayout);


  QLabel* nameLabel = new QLabel(i18n("Name")+" :",mWidget);
  m_nameEdit = new KLineEdit(mWidget);
  gridLayout->addWidget(nameLabel,0,0);
  gridLayout->addWidget(m_nameEdit,0,1);

  QLabel* valueLabel = new QLabel(i18n("Value")+" :",mWidget);
  m_valueEdit = new KLineEdit(mWidget);
  gridLayout->addWidget(valueLabel,1,0);
  gridLayout->addWidget(m_valueEdit,1,1);

  QFrame* bottomLine = new QFrame(mWidget);
  bottomLine->setFrameStyle(QFrame::HLine);
  bottomLine->setLineWidth(1);
  bottomLine->setMidLineWidth(0);
  bottomLine->setFrameShadow(QFrame::Sunken);

  topLayout->addStretch(10);
  topLayout->addSpacing (5);
  topLayout->addWidget(bottomLine);

  enableButtonOk(false);

  connect (this, SIGNAL(okClicked()), this, SLOT(addParamSlot()));
  connect (m_nameEdit, SIGNAL(textChanged(const QString&)), this, SLOT(nameChangedSlot(const QString&)));
  }


KNSAddParam::~KNSAddParam(){
  }

//----------------SLOTS----------------------


void KNSAddParam::addParamSlot (void) {

  emit paramAdded(m_nameEdit->text(),m_valueEdit->text());
  }

void KNSAddParam::nameChangedSlot (const QString&) {
    QStringList::const_iterator it;

    if (m_nameEdit->text().isEmpty()) {
    enableButtonOk(false);
    return;
    }

  for (it = m_nameList->begin(); it !=m_nameList->end();++it ) {
    if ((*it) == m_nameEdit->text()) {
      enableButtonOk(false);
      return;
      }
    }
  enableButtonOk(true);
  }

#include "knsaddparam.moc"
