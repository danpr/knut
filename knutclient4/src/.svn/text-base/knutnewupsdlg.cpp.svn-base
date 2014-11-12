/***************************************************************************
                          knutnewupsdlg.cpp  -  description
                             -------------------
    begin                : Tue Aug 21 2001
    copyright            : (C) 2001 by Daniel Prynych
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

#include "knutnewupsdlg.h"
 
#include <KDE/KLocale>
#include <KDE/KMessageBox>

#include <QtCore/QString>
#include <QtGui/QLayout>
#include <QtGui/QFrame>
#include <QtGui/QLabel>
#include <QtGui/QGroupBox>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>




//#include <iostream>


KNutNewUpsDlg::KNutNewUpsDlg (KNutUpsData* const upsData, KNutVarData* const analogData, const bool add, QWidget* parent)
  : KDialog( parent ), addMember(add), m_myUpsData(upsData) {

  setButtons( KDialog::Ok | KDialog::Cancel | KDialog::Help );
  setModal(true);
  setInitialSize(QSize(450,600));

  QWidget *page = new QWidget(this);
  setMainWidget (page);
  QVBoxLayout *mainLayout = new QVBoxLayout( page );

  QGroupBox *hostBox = new QGroupBox (page);
  QGridLayout *hostLayout = new QGridLayout (hostBox);
  mainLayout->addWidget(hostBox);

  QGroupBox* GroupBox2 = new QGroupBox(page);
  QVBoxLayout *vActionSelectorLayout = new QVBoxLayout( GroupBox2 );

  QLabel* textLabel1 = new QLabel( i18n("Name")+":",hostBox);
  m_lineEdit1 = new KLineEdit( hostBox );
  textLabel1->setBuddy(m_lineEdit1);
  m_lineEdit1->setEnabled( add ); 
  // pokud se nejedna o pridani (add) neni jmeno aktivovano
  // when makes adding name isn't activated
  hostLayout->addWidget (textLabel1,1,0);
  hostLayout->addWidget (m_lineEdit1,1,1,1,3);

  QLabel* textLabel2 = new QLabel(i18n("UPS address")+":", hostBox );
  m_lineEdit2 = new KLineEdit( hostBox );
  textLabel2->setBuddy(m_lineEdit2);
  hostLayout->addWidget (textLabel2,2,0);
  hostLayout->addWidget (m_lineEdit2,2,1,1,3);

// Name of UPS
  QLabel* textLabel7 = new QLabel(i18n("UPS name")+":", hostBox );
  m_lineEdit7 = new KLineEdit( hostBox);
  textLabel7->setBuddy(m_lineEdit7);
  hostLayout->addWidget (textLabel7,3,0);
  hostLayout->addWidget (m_lineEdit7,3,1,1,3);


  QLabel* textLabel3 = new QLabel(i18n("Delay (ms):"), hostBox );
  m_lineEdit3 = new KLineEdit( hostBox );
  m_lineEdit3->setText("5000");
  textLabel3->setBuddy(m_lineEdit3);
  hostLayout->addWidget (textLabel3,4,0);
  hostLayout->addWidget(m_lineEdit3,4,1);

  QLabel* textLabel4 = new QLabel(i18n("Port:"), hostBox );
  m_lineEdit4 = new KLineEdit( hostBox );
  m_lineEdit4->setText(QString("%1").arg(knc::DefaultPort));
  textLabel4->setBuddy(m_lineEdit4);
  hostLayout->addWidget(textLabel4,4,2);
  hostLayout->addWidget(m_lineEdit4,4,3);

  QLabel* textLabel5 = new QLabel(i18n("User name:"), hostBox );
  m_lineEdit5 = new KLineEdit( hostBox );
  m_lineEdit5->setText("");
  textLabel5->setBuddy(m_lineEdit5);
  hostLayout->addWidget (textLabel5,5,0);
  hostLayout->addWidget(m_lineEdit5,5,1);

  QLabel* textLabel6 = new QLabel(i18n("Password:"), hostBox );
  m_lineEdit6 = new KLineEdit( hostBox );
  m_lineEdit6->setEchoMode(QLineEdit::Password);
  m_lineEdit6->setText("");
  textLabel6->setBuddy(m_lineEdit6);
  hostLayout->addWidget(textLabel6,5,2);
  hostLayout->addWidget(m_lineEdit6,5,3);

  m_savePassword = new QCheckBox (i18n("Store NUT password in configuration file"),page);
  mainLayout->addWidget(m_savePassword);
  mainLayout->addSpacing(10);
  mainLayout->addWidget(GroupBox2);


  m_upsValueSelector = new KActionSelector(GroupBox2);

  m_upsValueSelector->setAvailableLabel(i18n("Availabled UPS values"));
  m_upsValueSelector->setSelectedLabel(i18n("Selected UPS values"));
  m_upsValueSelector->setAvailableInsertionPolicy(KActionSelector::Sorted);
  m_upsValueSelector->setSelectedInsertionPolicy(KActionSelector::AtTop);

  vActionSelectorLayout->addWidget(m_upsValueSelector);

  mainLayout->addStretch(10);

  GroupBox2->setTitle( i18n( "UPS Variables" ));

  QListWidget* aListWidget = m_upsValueSelector->availableListWidget();
  for (int i=1; i < knc::NumberOfTypes; i++) {  //values 0 is None, this values isn't showing
    new KNutUpsWidgetItem (aListWidget, analogData->getNameAnalogData(i),i);
    }
  aListWidget->sortItems(Qt::AscendingOrder);
  }



KNutNewUpsDlg::~KNutNewUpsDlg() {
  }


void KNutNewUpsDlg::moveToSelectedList (const int codeUpsVar) {

if (codeUpsVar == 0) return;
if (isExistSelectedItem(codeUpsVar)) return;  //Value is moved yet

KNutUpsWidgetItem *availableItem = 0;

for (int i = 0; i< m_upsValueSelector->availableListWidget()->count(); i++) {
  availableItem =(KNutUpsWidgetItem*) m_upsValueSelector->availableListWidget()->item(i);
  if (availableItem->getVarCode() == codeUpsVar) {

    QListWidget* sListWidget = m_upsValueSelector->selectedListWidget();
    new KNutUpsWidgetItem (sListWidget,availableItem->text(),codeUpsVar);
    m_upsValueSelector->availableListWidget()->takeItem( m_upsValueSelector->availableListWidget()->row(availableItem));
    break;
    }
  }
}


bool KNutNewUpsDlg::isExistSelectedItem (const int codeUpsVar) const {

KNutUpsWidgetItem *selectedItem = 0;

for ( int i = 0; i < m_upsValueSelector->selectedListWidget()->count(); i++) {
  selectedItem = (KNutUpsWidgetItem*) m_upsValueSelector->selectedListWidget()->item(i);
  if (selectedItem->getVarCode()== codeUpsVar) return true;
  }

return false;
}


void KNutNewUpsDlg::putData (const KNutUpsData::upsRecordDef upsRecord ) {
  m_lineEdit1->setText(upsRecord.name);
  m_lineEdit2->setText(upsRecord.upsAddress);
  m_lineEdit7->setText(upsRecord.upsName);
  QString delay, port;
  delay.setNum(upsRecord.delay);
  m_lineEdit3->setText(delay);
  port.setNum(upsRecord.port);
  m_lineEdit4->setText(port);
  m_lineEdit5->setText(upsRecord.userName);
  m_lineEdit6->setText(upsRecord.password);
  m_savePassword->setChecked(m_savePassword);

  for (int j=0; j <knc::NumberOfVars; j++) {
     moveToSelectedList (upsRecord.upsVarCode[j]);
     }
  }


KNutUpsData::upsRecordDef KNutNewUpsDlg::getData (void ) const {
  KNutUpsData::upsRecordDef upsRecord;
  int j;

  upsRecord.name=m_lineEdit1->text();
  upsRecord.upsAddress=m_lineEdit2->text();
  upsRecord.upsName= m_lineEdit7->text();
  upsRecord.delay=m_lineEdit3->text().toLong();
  upsRecord.port=(short unsigned)m_lineEdit4->text().toUInt();
  upsRecord.userName=m_lineEdit5->text();
  upsRecord.password=m_lineEdit6->text();
  upsRecord.savePassword=m_savePassword->isChecked();

  KNutUpsWidgetItem *selectedItem;
  for (j = 0; j < m_upsValueSelector->selectedListWidget()->count(); j++) {
    selectedItem =(KNutUpsWidgetItem*)m_upsValueSelector->selectedListWidget()->item(j);
    upsRecord.upsVarCode[j] =  selectedItem->getVarCode();
    }

  for (int i=j; i < knc::NumberOfVars; i++) { 
     upsRecord.upsVarCode[i]=0;
    }

  return upsRecord;
 }


void KNutNewUpsDlg::makeOk() {
  bool ok;

  setResult(QDialog::Rejected);
  // closes window
  m_lineEdit1->setText(m_lineEdit1->text().trimmed());
  m_lineEdit2->setText(m_lineEdit2->text().trimmed());

  if ((m_lineEdit1->text().length() <1 ) || (m_lineEdit2->text().length() <1 )) {
    KMessageBox::sorry (0,i18n("No Name or UPS address"));
    return;
    }

  if (addMember) {
    // when UpsName is existed we cannot make new record
    // pokud UpsName jiz existuje nemuzeme vytvorit novy zaznam
    if ( m_myUpsData->findName(m_lineEdit1->text()) != 0L ) {
    KMessageBox::sorry (0,i18n("This Name exist"));
    return;
    }
  }

  long delay = m_lineEdit3->text().toLong(&ok);
  if (! ok) {
    KMessageBox::sorry (0,i18n("Delay must be number"));
    return;
    }

  long port = m_lineEdit4->text().toLong(&ok);
  if (! ok) {
    KMessageBox::sorry (0,i18n("Port must be number"));
    return;
    }

  if ((port < knc::StartPort) || (port > knc::StopPort)) {
    KMessageBox::sorry (0,i18n("Port must be from 1025 until 65535"));
    return;
    }

  if (delay < knc::MinDelay) {
    KMessageBox::sorry (0,i18n("Delay is too small"));
    return;
    }
  accept ();
  }


/*********************************************************************************************/
/*                                                                                           */
/*                         PROTECTED SLOTS                                                   */
/*                                                                                           */
/*********************************************************************************************/
void KNutNewUpsDlg::slotButtonClicked(int button) {
  if (button == KDialog::Ok)
    makeOk();
  else
     KDialog::slotButtonClicked(button);
}


#include "knutnewupsdlg.moc"
