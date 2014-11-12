/***************************************************************************
                          knutinstcomms.cpp  -  description
                             -------------------
    begin                : ct ríj 24 2002
    copyright            : (C) 2002 by Daniel Prynych
    email                : Daniel@prynych.cz
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "knutinstcomms.h"
#include "knutprintupsvar.h"
#include "knutvardata.h"


#include <QtGui/QFrame>
#include <QtCore/QString>

#include <QtGui/QLayout> 
#include <QtGui/QLabel>


KNutInstComms::KNutInstComms(QString* oldUserName, QString* oldPassword, const QString uName, const QString password, KNutNet* const upsNet, QWidget* parent) : KDialog( parent ), m_upsNet(upsNet), m_oldUserName(oldUserName), m_oldPassword(oldPassword)  {
  upsICommDef upsIComm;

  if (!m_upsNet) return;

  setCaption( i18n("Instant commands") );
  setButtons( KDialog::Ok | KDialog::Cancel );

  m_upsConnectOk=true;

  QWidget *mainWidget = new QWidget (this);
  setMainWidget( mainWidget);


  QVBoxLayout *topLayout = new QVBoxLayout( mainWidget );
  QLabel *label1 = new QLabel (i18n("RUN INSTANT COMMAND"),mainWidget);
  label1->setAlignment(Qt::AlignHCenter);
  topLayout->addWidget(label1);

  QLabel *label2 = new QLabel (i18n("Command:"),mainWidget);
  m_instCommBox = new KComboBox(mainWidget);

  QGridLayout *passLayout = new QGridLayout ();
  QLabel *labelName = new QLabel (i18n("User name:"),mainWidget);
  QLabel *labelPassword = new QLabel (i18n("Password:"),mainWidget);
  m_lineEditName = new KLineEdit( mainWidget );
  m_lineEditPassword = new KLineEdit( mainWidget );
  if (((*m_oldUserName) == "") && ((*m_oldPassword) == "")) {
    m_lineEditName->setText(uName);
    m_lineEditPassword->setText(password);
    }
  else {
    m_lineEditName->setText(*m_oldUserName);
    m_lineEditPassword->setText(*m_oldPassword);
    m_lineEditName->setDisabled(true);
    m_lineEditPassword->setDisabled(true);
    }

  m_lineEditPassword->setEchoMode(QLineEdit::Password);

  topLayout->addLayout(passLayout);

  passLayout->addWidget(label2,0,0);
  passLayout->addWidget(m_instCommBox,0,1);

  passLayout->addWidget(labelName,1,0);
  passLayout->addWidget(labelPassword,2,0);
  passLayout->addWidget(m_lineEditName,1,1);
  passLayout->addWidget(m_lineEditPassword,2,1);

  topLayout->addStretch(10);

  //loads ComboBox
  int n = m_upsNet->readNumberComms();
  for (int i =0; i < n ; i++) {
    if (!m_upsNet->readIComm(i+1, upsIComm))
    m_instCommBox->addItem(upsIComm.upsCommName);
    }

  connect( this, SIGNAL( okClicked() ), this, SLOT( slotOk() ) );
  }

KNutInstComms::~KNutInstComms(){
 }


void KNutInstComms::slotOk() {
  int error =0;

  if (((*m_oldUserName) == "") && ((*m_oldPassword) == "")) {
    // prvni propojeni nastavime jmeno a heslo
    //for the first connection we have to set name and password
    if (!( error = m_upsNet->instantCommand(m_instCommBox->currentText(),m_lineEditName->text(),m_lineEditPassword->text(),false))) {
      (*m_oldUserName) = m_lineEditName->text();
      (*m_oldPassword) = m_lineEditPassword->text();
      accept();
      return;
      }
    }
  else {
    // for other connection name and password are not setted
    if (((*m_oldUserName) == m_lineEditName->text()) && ((*m_oldPassword) == m_lineEditPassword->text())) {
      // jen posleme prikaz // sends command only
      if (!( error = m_upsNet->instantCommand(m_instCommBox->currentText(),m_lineEditName->text(),m_lineEditPassword->text(),true))) {
        accept();
        return;
        }
      }
    }
  emit signalShowMessage(KNutVarData::errorToText (error),"icerrors_event");
  }

#include "knutinstcomms.moc"
