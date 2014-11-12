/***************************************************************************
                          knutrwvar.cpp  -  description
                             -------------------
    begin                : So ríj 26 2002
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

 
#include "knutrwvar.h"
#include "knutvardata.h"

#include <KDE/KComboBox>
#include <KDE/KLocale>

#include <QtGui/QFrame>
#include <QtCore/QString>

#include <QtGui/QLabel>
#include <QtGui/QLayout>


//#include <iostream>

KNutRWVar::KNutRWVar(QString* oldUserName, QString* oldPassword, const QString userName, const QString password, KNutNet* const upsNet, QWidget* parent) : KDialog(parent), m_upsNet(upsNet), m_oldUserName(oldUserName), m_oldPassword(oldPassword) {
  upsVarDef upsVar;

qDebug ("KNutRWVar::KNutRWVar");
  if (!m_upsNet) return;

  setCaption( i18n("UPS variables") );
  setButtons( KDialog::Ok | KDialog::Cancel | KDialog::Default );
  setButtonText(KDialog::User1,i18n("Reload Vars"));

  m_upsConnectOk = true;
 
  QWidget *mainWidget = new QWidget (this);
  setMainWidget( mainWidget);
     
  QVBoxLayout *topLayout = new QVBoxLayout( mainWidget );
  QLabel *label1 = new QLabel (i18n("SET RW VARIABLE"),mainWidget);
  label1->setAlignment(Qt::AlignHCenter);
  topLayout->addWidget(label1);


  QLabel *label2 = new QLabel (i18n("Variable:"),mainWidget);
  m_rWVarBox = new KComboBox(mainWidget);

  QLabel *label3 = new QLabel (i18n("Value:"),mainWidget);
  m_valueVarBox = new KComboBox(mainWidget);
  m_valueVarLine = new KLineEdit(mainWidget);
  m_valueVarLine->hide();

  m_passLayout = new QGridLayout();
  QLabel *labelName = new QLabel (i18n("User name:"),mainWidget);
  QLabel *labelPassword = new QLabel (i18n("Password:"),mainWidget);
  m_lineEditName = new KLineEdit( mainWidget );
  m_lineEditPassword = new KLineEdit( mainWidget );


  if (((*m_oldUserName) == "") && ((*m_oldPassword) == "")) {
    m_lineEditName->setText(userName);
    m_lineEditPassword->setText(password);
    }
  else {
    m_lineEditName->setText(*m_oldUserName);
    m_lineEditPassword->setText(*m_oldPassword);
    }

  if (!((*m_oldUserName) == "") || !((*m_oldPassword) == "")) {
    m_lineEditName->setDisabled(true);
    m_lineEditPassword->setDisabled(true);
    }

  m_lineEditPassword->setEchoMode(QLineEdit::Password);

  topLayout->addLayout(m_passLayout);

  m_passLayout->addWidget(label2,0,0);
  m_passLayout->addWidget(m_rWVarBox,0,1);
  m_passLayout->addWidget(label3,1,0);
  m_passLayout->addWidget(labelName,2,0);
  m_passLayout->addWidget(labelPassword,3,0);
  m_passLayout->addWidget(m_lineEditName,2,1);
  m_passLayout->addWidget(m_lineEditPassword,3,1);

  topLayout->addStretch(10);

  //loads ComboBox
  int n = m_upsNet->readNumberVars( KNutNet::RWVars );
  if ( n != -1) {
    for (int i =0; i < n ; i++) {
      if (!m_upsNet->readVars(i, upsVar,KNutNet::RWVars))
      m_rWVarBox->addItem(upsVar.upsVarName);
      }
    slotChangeVar(0);
    }
  else { // error
    m_passLayout->addWidget(m_valueVarLine,1,1);
    m_valueVarLine->setMaxLength(upsVar.upsVarMax);
    m_valueVarBox->hide();
    m_valueVarLine->show();
    m_upsValueType=true;
    }
  connect (m_rWVarBox,SIGNAL(activated(int)),this,SLOT(slotChangeVar(int)));

  connect( this, SIGNAL( defaultClicked() ), this, SLOT( slotDefault() ) ); 
  connect( this, SIGNAL( okClicked() ), this, SLOT( slotOk() ) );
  }


void KNutRWVar::slotDefault () {
  upsVarDef upsVar;
  int error;
qDebug("KNutRWVar::slotDefault");
  QString varName = m_rWVarBox->currentText();
  if (!( error = m_upsNet->readVars(varName,upsVar))) {
    if (upsVar.upsValueType) m_valueVarLine->setText(upsVar.upsValue);
    else m_valueVarBox->setCurrentItem(upsVar.upsValue);
    }
  else emit signalShowMessage(KNutVarData::errorToText (error),"rwerrors_event");
  }


void KNutRWVar::slotChangeVar(int item) {
  upsVarDef upsVar;

  QString varName = m_rWVarBox->itemText(item);
  int error = m_upsNet->readVars(varName,upsVar);
  m_upsValueType=upsVar.upsValueType;

  if (error || (upsVar.upsValueType)) {
    // char
    m_valueVarBox->hide();
    m_valueVarLine->show();
    m_passLayout->addWidget(m_valueVarLine,1,1); // add char box into layout
    m_valueVarLine->setText(upsVar.upsValue);
    }
  else {
    //enum
    m_valueVarLine->hide();
    m_valueVarBox->show();
    m_passLayout->addWidget(m_valueVarBox,1,1);  // add ValueVarBox into layout
    m_valueVarBox->clear();
    if (upsVar.upsVarMax) {
      for (int i = 0 ; i < upsVar.upsVarMax; i++)
        m_valueVarBox->addItem(m_upsNet->readEnumValueVar(upsVar.upsVarName,i+1));
      }
    m_valueVarBox->setCurrentItem(upsVar.upsValue);
    }
  }



KNutRWVar::~KNutRWVar(){
}

//bool KNutRWVar::isUpsOk (void) { return m_upsConnectOk; }


void KNutRWVar::slotOk() {
  QString value;
  int error =0;

  if (m_upsValueType) value=m_valueVarLine->text();
  else value=m_valueVarBox->currentText();

  if (((*m_oldUserName) == "") && ((*m_oldPassword) == "")) {
    //the first connection sets name and password
    //prvni propojeni nastavime jmeno a heslo
    if (!(error = m_upsNet->setVariable(m_rWVarBox->currentText(), value, m_lineEditName->text(),m_lineEditPassword->text(),false))) {
      // vzhledem k asynchronimu spracovani asi zbytecne
//      emit signalChangeRWVars(m_rWVarBox->currentText());//emits command for loading of variable and repaint of panel

      (*m_oldUserName) = m_lineEditName->text();
      (*m_oldPassword) = m_lineEditPassword->text();
      accept();
      return;
      }
    }
  else {
qDebug("KNutRWVar::slotOk() - second connection");
    if (((*m_oldUserName) == m_lineEditName->text()) && ((*m_oldPassword) == m_lineEditPassword->text())) {
      if (!(error = m_upsNet->setVariable(m_rWVarBox->currentText(), value, m_lineEditName->text(),m_lineEditPassword->text(),true))) {
      // next line isn't needed, maybe
      // vzhledem k asynchronimu spracovani asi zbytecne
//        emit signalChangeRWVars(m_rWVarBox->currentText()); //emits command for loading of variable and repaint of panel
        accept();
        return;
        }
      }
    }
  //Nahlasime chybu
  // sends information about error
  emit signalShowMessage(KNutVarData::errorToText (error),"rwerrors_event");
  }

#include "knutrwvar.moc"
