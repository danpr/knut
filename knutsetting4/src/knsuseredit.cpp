/***************************************************************************
                          knsuseredit.cpp  -  description
                             -------------------
    begin                : So srp 28 2004
    copyright            : (C) 2004 by Daniel Prynych,,,
    email                : Daniel at Prynych dot cz
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "knsuseredit.h"
#include "knsstring.h"

#include <KDE/KLocale>

#include <QtGui/QGroupBox>

#include <QtGui/QLayout>
#include <QtGui/QLabel>

#include <QtGui/QWidget>
#include <QtGui/QRadioButton>
#include <QtGui/QCheckBox>
#include <QtCore/QStringList>
#include <QtGui/QButtonGroup>
#include <QtCore/QStringList>

#include <QDebug>




KNSUserEdit::KNSUserEdit( QWidget *parent ) : QGroupBox(parent){

  if (instcmdStringDef.isEmpty()) m_instcmdDef = QStringList();
  else  {
    m_instcmdDef = instcmdStringDef.split(" ",QString::SkipEmptyParts);
    }

  setTitle(i18n("User Setting"));



  QLabel* nameLabel = new QLabel (i18n("Name")+" :",this);
  QLabel* passwordLabel = new QLabel (i18n("Password")+" :",this);
  m_nameEdit = new KLineEdit (this);
  m_passwordEdit = new KLineEdit (this); // password
  QGroupBox*  typeFrameGroupBox = new QGroupBox(this);
  typeFrameGroupBox->setTitle(i18n("Type of section"));
  m_userRadio = new QRadioButton (i18n("User"),typeFrameGroupBox);
  m_monitorRadio =new QRadioButton ( i18n("Monitoring"),typeFrameGroupBox);
  m_commandWidget = new KNSDoubleListBox(this,i18n("Instant Commands"));
  m_actionsGroup = new QGroupBox(i18n("Actions"),this);
  m_SETCheckBox = new QCheckBox(i18n("Allow the change variables")+" (SET)",m_actionsGroup);
  m_FSBCheckBox = new QCheckBox(i18n("Set forced shutdown flag")+" (FSD)",m_actionsGroup);
  m_monitorGroupBox = new QGroupBox(this);
  m_monitorGroupBox->setTitle(i18n("UPS monitoring"));
  m_masterRadio = new QRadioButton(i18n("Master"),m_monitorGroupBox);
  m_slaveRadio = new QRadioButton(i18n("Slave"),m_monitorGroupBox);


  QVBoxLayout* mainLayout = new QVBoxLayout(this);
  mainLayout->setSizeConstraint(QLayout::SetMinimumSize);
  QHBoxLayout* firstLayout = new QHBoxLayout();
  firstLayout->setSizeConstraint(QLayout::SetMinimumSize);
  QGridLayout* namePassLayout = new QGridLayout();
  namePassLayout->setSizeConstraint(QLayout::SetMinimumSize);

  QVBoxLayout* typeLayout = new QVBoxLayout(typeFrameGroupBox);
  
  typeLayout->setSizeConstraint(QLayout::SetMinimumSize);

  QHBoxLayout* secondLayout = new QHBoxLayout();
  secondLayout->setSizeConstraint(QLayout::SetMinimumSize);

  QVBoxLayout* radioLayout = new QVBoxLayout();
  radioLayout->setSizeConstraint(QLayout::SetMinimumSize);


  QVBoxLayout* actionsLayout = new QVBoxLayout(m_actionsGroup);
  actionsLayout->setSizeConstraint(QLayout::SetMinimumSize);
  QVBoxLayout* monitorLayout = new QVBoxLayout(m_monitorGroupBox);
  monitorLayout->setSizeConstraint(QLayout::SetMinimumSize);


  mainLayout->addLayout(firstLayout);
  mainLayout->addLayout(secondLayout);

  firstLayout->addLayout(namePassLayout);
  firstLayout->addWidget(typeFrameGroupBox);

  namePassLayout->addWidget (nameLabel,0,0);
  namePassLayout->addWidget (passwordLabel,1,0);
  namePassLayout->addWidget (m_nameEdit,0,1);
  namePassLayout->addWidget (m_passwordEdit,1,1);

  secondLayout->addWidget(m_commandWidget);
  secondLayout->addLayout(radioLayout);
 
  radioLayout->addWidget(m_actionsGroup);
  radioLayout->addWidget(m_monitorGroupBox);


  typeLayout->addWidget ( m_userRadio );
  typeLayout->addWidget ( m_monitorRadio );

  actionsLayout->addWidget(m_SETCheckBox);
  actionsLayout->addWidget(m_FSBCheckBox);
  monitorLayout->addWidget(m_masterRadio);
  monitorLayout->addWidget(m_slaveRadio);
  mainLayout->setStretchFactor(m_actionsGroup,10);

  clear();

//  namePasswordChangedSlot("");

  connect (m_userRadio,SIGNAL(toggled ( bool )),this,SLOT (userClickedSlot( bool )));

  connect (m_nameEdit,SIGNAL(textChanged(const QString &)),SLOT(namePasswordChangedSlot(const QString &)));
  connect (m_passwordEdit,SIGNAL(textChanged(const QString &)),SLOT(namePasswordChangedSlot(const QString &)));
  }

KNSUserEdit::~KNSUserEdit(){

  m_masterRadio->setChecked(true);
}

void KNSUserEdit::clear(void) {

  m_nameEdit->setText("");
  m_passwordEdit->setText("");
  m_userRadio->setChecked(true);
  m_monitorRadio->setChecked(false);
  m_SETCheckBox->setChecked(false);
  m_FSBCheckBox->setChecked(false);
  m_commandWidget->clear();
  m_masterRadio->setChecked(true);
  m_slaveRadio->setChecked(false);

  m_actionsGroup->hide();
  m_commandWidget->hide();
  m_monitorGroupBox->hide();
  }



void KNSUserEdit::putData (const QString name,const QString password, const bool isUser, const QString actions, const QStringList instCmds, const typeOfMonitoring typeMon) {
  bool SETCheck = false;
  bool FSBCheck = false;
  KNSString wordString;
  KNSString word;

  m_nameEdit->setText(name);
  m_passwordEdit->setText(password);

//  namePasswordChangedSlot("");

  m_userRadio->setChecked(isUser);
  m_monitorRadio->setChecked(!isUser);

  userClickedSlot (isUser); // sets widgets

  wordString=actions;
  wordString.trimmedOwn();
  wordString.toLowerOwn();

  word = wordString.extractWord();
  while (!word.isEmpty()) {
    if (word == "set") SETCheck = true;
    if (word == "fsb") FSBCheck = true;
    word = wordString.extractWord();
    }
  m_SETCheckBox->setChecked(SETCheck);
  m_FSBCheckBox->setChecked(FSBCheck);

  m_commandWidget->setMainList(m_instcmdDef);
  m_commandWidget->setSelectList(instCmds);

  m_masterRadio->setChecked((bool)typeMon);
  m_slaveRadio->setChecked(!(bool)typeMon);
  }


void KNSUserEdit::getData (QString& name, QString& password, bool& isUser,  QString& actions,QStringList& instCmds, typeOfMonitoring& typeMon) {

  name = m_nameEdit->text();
  password = m_passwordEdit->text();
  isUser = m_userRadio->isChecked();
  actions ="";
  if (m_SETCheckBox->isChecked()) actions += "SET";
  if (m_FSBCheckBox->isChecked()) {
    if (!actions.isEmpty()) actions += " ";
    actions += "FSB";
    }
  instCmds = m_commandWidget->getSelectList();
  if (m_masterRadio->isChecked()) typeMon=MON_MASTER; else typeMon=MON_SLAVE;
  }

QString KNSUserEdit::getPassword (void) {

  return m_passwordEdit->text();
  }


typeOfMonitoring KNSUserEdit::getTypeMon (void) {

  if (m_masterRadio->isChecked()) return MON_MASTER; else return MON_SLAVE;
  }

bool KNSUserEdit::getIsUser (void) {

  return m_userRadio->isChecked();
  }



void KNSUserEdit::setReadOnlyName (const bool readOnly) {

//  nameEdit->setEnabled(enable);
  m_nameEdit->setReadOnly(readOnly);
  }


void KNSUserEdit::setReadOnly (const bool readOnly) {

  setReadOnlyName (readOnly);
  m_passwordEdit->setReadOnly(readOnly);
  m_userRadio->setEnabled(!readOnly);
  m_monitorRadio->setEnabled(!readOnly);
  m_masterRadio->setEnabled(!readOnly);
  m_slaveRadio->setEnabled(!readOnly);
  m_SETCheckBox->setEnabled(!readOnly);
  m_FSBCheckBox->setEnabled(!readOnly);
  m_commandWidget->setReadOnly(readOnly);

  }


void KNSUserEdit::init (void) {

  clear();
  m_commandWidget->setMainList(m_instcmdDef);
  }

void KNSUserEdit::testAreNamePasswordEmpty (void) {

namePasswordChangedSlot("");
}

//---------------SLOTS----------------  

void KNSUserEdit::namePasswordChangedSlot( const QString & ) {

if (m_nameEdit->text().isEmpty() ||  m_passwordEdit->text().isEmpty())
  emit nameAndPasswordAreEmpty (true);
else 
  emit nameAndPasswordAreEmpty (false);
}


void KNSUserEdit::userClickedSlot(bool on) {

  if (on) {
    m_actionsGroup->show();
    m_commandWidget->show();
    m_monitorGroupBox->hide();
    }
  else {
    m_actionsGroup->hide();
    m_commandWidget->hide();
    m_monitorGroupBox->show();
    }
  }

#include "knsuseredit.moc"
