/***************************************************************************
                          knsmonnotify.cpp  -  description
                             -------------------
    begin                : So ?íj 23 2004
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

#include "knsmonnotify.h"

#include <KDE/KLocale>
#include <KDE/KLineEdit>

#include <QtGui/QListWidget>
#include <QtGui/QListWidgetItem>
#include <QtGui/QLayout>
#include <QtGui/QLabel>
#include <QtGui/QFontMetrics>
#include <QtGui/QGroupBox>
#include <QtGui/QCheckBox>



const char* notifyDescText[] =
   {I18N_NOOP("UPS is back online."),
    I18N_NOOP("UPS is on battery."),
    I18N_NOOP("UPS has a low battery (if also on battery, it's critical)."),
    I18N_NOOP("UPS is being shutdown by the master system (FSD = Forced Shutdown)."),
    I18N_NOOP("Communications established with the UPS."),
    I18N_NOOP("Communications lost to the UPS."),
    I18N_NOOP("The system is being shutdown."),
    I18N_NOOP("The UPS battery is bad and needs to be replaced."),
    I18N_NOOP("A UPS is unavailable (can't be contacted for monitoring).")};


KNSMonNotify::KNSMonNotify(KNSCommonData* tCommonData,  QWidget *parent)
             : QWidget(parent) {


  m_monNotifyItemsFile = tCommonData->monitorNotifyDataAddr();           
  QFontMetrics fm =  fontMetrics();

  m_itemsListBox = new QListWidget(this);
  QGroupBox* messageBox = new QGroupBox(this);
  messageBox->setTitle(i18n("Message"));

  m_messageEdit = new KLineEdit (messageBox);
  QGroupBox* flagsBox = new QGroupBox(this);
  flagsBox->setTitle(i18n("Possibilities"));

  m_syslogFlag = new QCheckBox (i18n("Write to syslog"),flagsBox);
  m_wallFlag = new QCheckBox (i18n("Write to all users"),flagsBox);
  m_execFlag = new QCheckBox (i18n("Exec command"),flagsBox);
  m_ignoreFlag = new QCheckBox (i18n("Don't do anything"),flagsBox);
  QGroupBox* descriptionBox = new QGroupBox(this);
  descriptionBox->setTitle(i18n("Description"));

  m_notifyDesc = new KTextEdit(descriptionBox);
  m_notifyDesc->setReadOnly(true);

  m_editButton = new KPushButton(i18n("Edit"),this);
  m_okButton = new KPushButton(i18n("OK"),this);
  m_cancelButton = new KPushButton(i18n("Cancel"),this);
  m_defaultButton = new KPushButton(i18n("Default"),this);
  m_standardButton = new KPushButton(i18n("Default of all notifications"),this);


  connect (m_itemsListBox, SIGNAL (itemActivated ( QListWidgetItem*)),this, SLOT (itemListBoxSelectedSlot(QListWidgetItem*)));

  connect (m_editButton, SIGNAL (clicked()),this, SLOT (editItemSlot()));
  connect (m_defaultButton, SIGNAL (clicked()),this, SLOT (defaultItemSlot()));
  connect (m_okButton, SIGNAL (clicked()),this, SLOT (okItemSlot()));
  connect (m_cancelButton, SIGNAL (clicked()),this, SLOT (cancelItemSlot()));

  connect (m_ignoreFlag, SIGNAL (clicked()),this, SLOT (ignoreFlagSlot()));

  connect (m_syslogFlag, SIGNAL (clicked()),this, SLOT (otherFlagSlot()));
  connect (m_wallFlag, SIGNAL (clicked()),this, SLOT (otherFlagSlot()));
  connect (m_execFlag, SIGNAL (clicked()),this, SLOT (otherFlagSlot()));

  connect( m_standardButton,SIGNAL(clicked()),this,SLOT(setStandardSlot()));

// main layout
  QVBoxLayout* topLayout = new QVBoxLayout(this);
//  topLayout->setMargin(10);
//  topLayout->setSpacing(10);
  topLayout->setSizeConstraint(QLayout::SetMinimumSize);

  QHBoxLayout* topHLayout = new QHBoxLayout();
  topHLayout->setSizeConstraint(QLayout::SetMinimumSize);

  QVBoxLayout* topVLayout = new QVBoxLayout();
  topVLayout->setSizeConstraint(QLayout::SetMinimumSize);

  QVBoxLayout* messageVLayout = new QVBoxLayout(messageBox);
//  messageVLayout->setMargin(0);  
  messageVLayout->setSpacing(0);
  messageVLayout->setSizeConstraint(QLayout::SetMinimumSize);

  QVBoxLayout* flagsVLayout = new QVBoxLayout(flagsBox);
//  flagsVLayout->setMargin(5);
  flagsVLayout->setSpacing(0);
  flagsVLayout->setSizeConstraint(QLayout::SetMinimumSize);

  QGridLayout* flagsGLayout = new QGridLayout();
//  flagsGLayout->setMargin(0);
  flagsGLayout->setSizeConstraint(QLayout::SetMinimumSize);

  QVBoxLayout* descVLayout = new QVBoxLayout(descriptionBox);
//  descVLayout->setMargin(0);
  descVLayout->setSpacing(0);
  descVLayout->setSizeConstraint(QLayout::SetMinimumSize);

  QHBoxLayout* buttonsLayout = new QHBoxLayout();


//layout struct
  topLayout->addLayout(topHLayout);
  topLayout->addLayout(buttonsLayout);

  topHLayout->addWidget(m_itemsListBox);
  topHLayout->addLayout(topVLayout);

  topVLayout->addWidget(messageBox);
  topVLayout->addWidget(flagsBox);
  topVLayout->addWidget(descriptionBox);

  messageVLayout->addWidget(m_messageEdit);

  flagsVLayout->addLayout(flagsGLayout);

  flagsGLayout->addItem(new QSpacerItem(5,0), 0, 0);
  flagsGLayout->addItem(new QSpacerItem(5,0), 0, 2);
  flagsGLayout->addItem(new QSpacerItem(5,0), 0, 4);
  flagsGLayout->addWidget(m_syslogFlag,0,1);
  flagsGLayout->addWidget(m_wallFlag,0,3);
  flagsGLayout->addWidget(m_execFlag,1,1);
  flagsGLayout->addWidget(m_ignoreFlag,1,3);

  descVLayout->addWidget(m_notifyDesc);

  buttonsLayout->addStretch(20);
  buttonsLayout->addWidget(m_editButton);
  buttonsLayout->addWidget(m_okButton);
  buttonsLayout->addWidget(m_cancelButton);
  buttonsLayout->addWidget(m_defaultButton);
  buttonsLayout->addWidget(m_standardButton);

  initItemsListBox();
  setButtons(true);

  setReadOnlyWidgets(false);
  }

KNSMonNotify::~KNSMonNotify(){
  }


void KNSMonNotify::initItemsListBox(){
  m_itemsListBox->addItem(i18n("UPS is on line"));
  m_itemsListBox->addItem(i18n("UPS is on battery"));
  m_itemsListBox->addItem(i18n("UPS has a low battery"));
  m_itemsListBox->addItem(i18n("Forced shutdown"));
  m_itemsListBox->addItem(i18n("Communications OK"));
  m_itemsListBox->addItem(i18n("Communications last"));
  m_itemsListBox->addItem(i18n("Shutdown"));
  m_itemsListBox->addItem(i18n("UPS battery is bad"));
  m_itemsListBox->addItem(i18n("No communications"));
  }



void KNSMonNotify::itemListBoxSelectedSlot(QListWidgetItem* item) {

  int index =  m_itemsListBox->row(item);
  if ((index > -1) && (index < notifyNamesCount)) {

    m_notifyDesc->setText(i18n(notifyDescText[index]));
    m_messageEdit->setText(m_monNotifyItemsFile[index].message);
    m_syslogFlag->setChecked(m_monNotifyItemsFile[index].syslogFlag);
    m_wallFlag->setChecked(m_monNotifyItemsFile[index].wallFlag);
    m_execFlag->setChecked(m_monNotifyItemsFile[index].execFlag);
    m_ignoreFlag->setChecked(m_monNotifyItemsFile[index].ignoreFlag);
    }
  else
    {
    clearItem();
    }
  setButtons(true);  
  }

void KNSMonNotify::clearItem(void) {

  m_notifyDesc->setText(i18n("Description of notification."));
  m_messageEdit->setText("");
  m_syslogFlag->setChecked(false);
  m_wallFlag->setChecked(false);
  m_execFlag->setChecked(false);
  m_ignoreFlag->setChecked(false);
  }


void KNSMonNotify::clear(void) {

  clearItem();
  QList<QListWidgetItem *> selectedItemsList =m_itemsListBox->selectedItems();
  for (int i = 0; i < selectedItemsList.count(); i++) {
    m_itemsListBox->setItemSelected ( selectedItemsList[i],false);  
    }
  m_itemsListBox->setCurrentRow(-1);
  }

void KNSMonNotify::setButtons(bool type) {

  m_standardButton->setEnabled(true);
  if (type) {
    m_editButton->show();
    m_defaultButton->show();
    m_okButton->hide();
    m_cancelButton->hide();
    if (m_itemsListBox->currentRow() == -1) {
      m_editButton->setEnabled(false);
      m_defaultButton->setEnabled(false);
      }
    else {
      m_editButton->setEnabled(true);
      m_defaultButton->setEnabled(true);
      }
    }
  else {
    m_editButton->hide();
    m_defaultButton->hide();
    m_okButton->show();
    m_cancelButton->show();
    }
  }

void KNSMonNotify::setReadOnlyWidgets(bool type) {

  m_messageEdit->setReadOnly(!type);
  m_syslogFlag->setEnabled(type);
  m_wallFlag->setEnabled(type);
  m_execFlag->setEnabled(type);
  m_ignoreFlag->setEnabled(type);
  }


//************** SLOTS ************************

void KNSMonNotify::editItemSlot(void) {

  setReadOnlyWidgets(true);
  m_itemsListBox->setEnabled(false);
  setButtons(false);
  }

void KNSMonNotify::defaultItemSlot(void) {

  m_messageEdit->setText("");
  m_ignoreFlag->setChecked(false);
  switch (m_itemsListBox->currentRow()) {
    case online:
      m_syslogFlag->setChecked(true);
      m_wallFlag->setChecked(false);
      m_execFlag->setChecked(false);
      break;
    case onbatt:
      m_syslogFlag->setChecked(true);
      m_wallFlag->setChecked(true);
      m_execFlag->setChecked(true);
      break;
    case shutdown:
      m_syslogFlag->setChecked(true);
      m_execFlag->setChecked(true);
      m_wallFlag->setChecked(false);
      break;
    default:
      m_syslogFlag->setChecked(false);
      m_wallFlag->setChecked(false);
      m_execFlag->setChecked(false);
      break;
    }

  m_monNotifyItemsFile[m_itemsListBox->currentRow()].message=m_messageEdit->text();
  m_monNotifyItemsFile[m_itemsListBox->currentRow()].syslogFlag=m_syslogFlag->isChecked();
  m_monNotifyItemsFile[m_itemsListBox->currentRow()].wallFlag=m_wallFlag->isChecked();
  m_monNotifyItemsFile[m_itemsListBox->currentRow()].execFlag=m_execFlag->isChecked();
  m_monNotifyItemsFile[m_itemsListBox->currentRow()].ignoreFlag=m_ignoreFlag->isChecked();
  }

void KNSMonNotify::okItemSlot(void) {

  m_monNotifyItemsFile[m_itemsListBox->currentRow()].message=m_messageEdit->text();
  m_monNotifyItemsFile[m_itemsListBox->currentRow()].syslogFlag=m_syslogFlag->isChecked();
  m_monNotifyItemsFile[m_itemsListBox->currentRow()].wallFlag=m_wallFlag->isChecked();
  m_monNotifyItemsFile[m_itemsListBox->currentRow()].execFlag=m_execFlag->isChecked();
  m_monNotifyItemsFile[m_itemsListBox->currentRow()].ignoreFlag=m_ignoreFlag->isChecked();

  setReadOnlyWidgets(false);
  m_itemsListBox->setEnabled(true);
  setButtons(true);
  emit changed();
  }

void KNSMonNotify::cancelItemSlot(void) {

  m_messageEdit->setText(m_monNotifyItemsFile[m_itemsListBox->currentRow()].message);
  m_syslogFlag->setChecked(m_monNotifyItemsFile[m_itemsListBox->currentRow()].syslogFlag);
  m_wallFlag->setChecked(m_monNotifyItemsFile[m_itemsListBox->currentRow()].wallFlag);
  m_execFlag->setChecked(m_monNotifyItemsFile[m_itemsListBox->currentRow()].execFlag);
  m_ignoreFlag->setChecked(m_monNotifyItemsFile[m_itemsListBox->currentRow()].ignoreFlag);

  setReadOnlyWidgets(false);
  m_itemsListBox->setEnabled(true);
  setButtons(true);
  }

void KNSMonNotify::ignoreFlagSlot( void) {

  if (m_ignoreFlag->isChecked()) {
    m_syslogFlag->setChecked(false);
    m_wallFlag->setChecked(false);
    m_execFlag->setChecked(false);
    }
  }


void KNSMonNotify::otherFlagSlot(void) {

  if ((m_syslogFlag->isChecked()) || (m_wallFlag->isChecked()) || (m_execFlag->isChecked()))
    m_ignoreFlag->setChecked(false);
  }

void KNSMonNotify::setStandardSlot (void) {
  int i;
  bool syslogFl;
  bool wallFl;
  bool execFl;

  // if data are changed, must call procedure for push "Cancel" button
  if (!m_cancelButton->isHidden()) cancelItemSlot();

  for (i=0; i< notifyNamesCount; i++) {
    syslogFl=false;
    wallFl=false;
    execFl=false;
    switch (i) {
      case online:
        syslogFl=true;
        break;
      case onbatt:
        syslogFl=true;
        wallFl=true;
        execFl=true;
        break;
      case shutdown:
        syslogFl=true;
        execFl=true;
        break;
      }
    m_monNotifyItemsFile[i].message="";
    m_monNotifyItemsFile[i].syslogFlag=syslogFl;
    m_monNotifyItemsFile[i].wallFlag=wallFl;
    m_monNotifyItemsFile[i].execFlag=execFl;
    m_monNotifyItemsFile[i].ignoreFlag=false;
    }
  QList<QListWidgetItem *> selectedItemsList =m_itemsListBox->selectedItems();
  for (int i = 0; i < selectedItemsList.count(); i++) {
   m_itemsListBox->setItemSelected ( selectedItemsList[i],false);  
   }
  m_itemsListBox->setCurrentRow(-1);
  clearItem();

  emit changed();
  }

#include "knsmonnotify.moc"
