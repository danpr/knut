/***************************************************************************
                          knutprintupsvar.cpp  -  description
                             -------------------
    begin                : St cec 3 2002
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

#include "knutprintupsvar.h"
#include "knutvardata.h"

#include <KNotification>
#include <KLocalizedString>

#include <QLayout>
#include <QFrame>
#include <QDialogButtonBox>
#include <QString>
#include <QPalette>
#include <QPushButton>

// od verze 0.6.0  nebudeme vytvaret vlastni spojeni

KNutPrintUpsVar::KNutPrintUpsVar(KNutNet* const upsNet, const bool useDescription, const bool allowNotification, QWidget* parent)
: QDialog(parent), m_upsNet(upsNet), m_allowNotification(allowNotification) {

  if (!upsNet) return;

  upsVarDef upsVar;
  upsICommDef upsIComm;
  dataVarDef dataVar;

  m_dataVarList.clear();

  setWindowTitle( i18n("UPS variables") );

  QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Cancel);

  QPushButton *retryButton = new QPushButton(i18n("Reload Vars"));
  retryButton->setDefault(true);
  buttonBox->addButton(retryButton, QDialogButtonBox::AcceptRole);

  connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
  connect(buttonBox, SIGNAL(accepted()), this, SLOT(slotReloadVars()));

  m_treeWidget = new QTreeWidget(this); //mainWidget

  QVBoxLayout *topDialogLayout = new QVBoxLayout( this);
  topDialogLayout->addWidget(m_treeWidget);
  topDialogLayout->addWidget(buttonBox);


//  m_treeWidget = new QTreeWidget(this); //mainWidget

//  setMainWidget( m_treeWidget );

  QStringList headersList;
  headersList << i18n("Name") << i18n("Value");
  
  if (useDescription) {
    m_treeWidget->setColumnCount(3);
    headersList << i18n("Description");
    }
  else {
    m_treeWidget->setColumnCount(2);
    }
  
  m_treeWidget->setHeaderLabels (headersList);  

    int countVars=m_upsNet->readNumberVars(KNutNet::AllVars);
    int countRWVars=m_upsNet->readNumberVars(KNutNet::RWVars);
    int countIComms=m_upsNet->readNumberComms();

    m_iconDir = QIcon("folder-open");   

   m_treeWidget->setRootIsDecorated(true);
   m_treeWidget->setAllColumnsShowFocus( true );
   m_treeWidget->setUniformRowHeights(true);
   m_treeWidget->setSortingEnabled(false); 
   m_treeWidget->setIndentation(20);
 
    QTreeWidgetItem* widgetItem;
    if (countIComms) {

      QTreeWidgetItem* ICommMainWidget = new QTreeWidgetItem( m_treeWidget);
      ICommMainWidget->setText (0,i18n("Instant Commands"));
      ICommMainWidget->setIcon(0,m_iconDir);
      m_treeWidget->expandItem(ICommMainWidget);
      for (int i=0; i<countIComms; i++) {
        m_upsNet->readIComm(i+1,upsIComm);
        widgetItem = addTreeWIdgetItem (ICommMainWidget, upsIComm.upsCommName, "");
        if ((!upsVar.upsDescription.isEmpty()) && useDescription) {
          widgetItem ->setText (2,upsIComm.upsDescription);
          }
        }
      }

    if (countRWVars) {
      QTreeWidgetItem* RWVarsMainWidget = new QTreeWidgetItem( m_treeWidget);
      RWVarsMainWidget->setText (0,i18n("Read/Write Variables"));
      RWVarsMainWidget->setIcon(0,m_iconDir);
      m_treeWidget->expandItem(RWVarsMainWidget);

      for (int i=0; i<countVars; i++) {
        m_upsNet->readVars(i+1,upsVar);

        if (!upsVar.upsVarType) {
          dataVar.name=upsVar.upsVarName;
          widgetItem = addTreeWIdgetItem ( RWVarsMainWidget, upsVar.upsVarName, upsVar.upsValue);
          dataVar.value = widgetItem;

        if ((!upsVar.upsDescription.isEmpty()) && useDescription) {
          widgetItem->setText(2,upsVar.upsDescription);
	  }

          m_dataVarList.push_back(dataVar);
         }
       }
     }


    QTreeWidgetItem* ROVarsMainWidget = new QTreeWidgetItem( m_treeWidget);
    ROVarsMainWidget->setText(0,i18n("Read Only Variables"));
    ROVarsMainWidget->setIcon(0,m_iconDir);
    m_treeWidget->expandItem(ROVarsMainWidget);
    for (int i=0; i<countVars; i++) {
      m_upsNet->readVars(i+1,upsVar);

      if (upsVar.upsVarType) {
        dataVar.name=upsVar.upsVarName;
        widgetItem = addTreeWIdgetItem( ROVarsMainWidget, upsVar.upsVarName, upsVar.upsValue);
        dataVar.value = widgetItem;

        if ((!upsVar.upsDescription.isEmpty()) && useDescription)
          widgetItem->setText(2,upsVar.upsDescription);

      m_dataVarList.push_back(dataVar);
      }
    }

  m_treeWidget->resizeColumnToContents ( 0 );
  m_treeWidget->resizeColumnToContents ( 1 );

  colorRow();

//  connect (this,SIGNAL(user1Clicked()),this,SLOT(slotReloadVars()));
  connect (m_treeWidget,SIGNAL(itemCollapsed ( QTreeWidgetItem*)),this,SLOT(slotTreeCollapsedExpanded (QTreeWidgetItem *)));
  connect (m_treeWidget,SIGNAL(itemExpanded ( QTreeWidgetItem*)),this,SLOT(slotTreeCollapsedExpanded (QTreeWidgetItem *)));

  setLayout(topDialogLayout);

  }



KNutPrintUpsVar::~KNutPrintUpsVar(){

  m_dataVarList.clear();
  if (m_treeWidget) delete m_treeWidget;
  }


void KNutPrintUpsVar::colorRow (void) {

  bool rowBackground = true;

  //void QTreeWidgetItem::setBackground ( int column, const QBrush & brush );
  for (int i=0; i < m_treeWidget->topLevelItemCount(); i++) {
    if (rowBackground){
      for (int j=0; j < m_treeWidget->columnCount (); j++) {
        m_treeWidget->topLevelItem(i)->setBackground (j,palette().base());
        }
      } 
    else {
      for (int j=0; j < m_treeWidget->columnCount (); j++) {
        m_treeWidget->topLevelItem(i)->setBackground (j,palette().alternateBase());
        }
      }
    rowBackground = !(rowBackground);
    
    if (m_treeWidget->topLevelItem(i)->isExpanded ()) {
      colorRowChildren (m_treeWidget->topLevelItem(i), rowBackground); 
      }
    }
  }


void KNutPrintUpsVar::colorRowChildren (const QTreeWidgetItem *item, bool &rowBackground ) {

  int count = item->childCount();
  for (int i = 0; i < count; i++) {
    if (!(item->child(i)->isHidden())) {
      if (rowBackground) {
        for (int j=0; j < m_treeWidget->columnCount (); j++) {
          item->child(i)->setBackground (j,palette().base());
          }
        }
      else {
        for (int j=0; j < m_treeWidget->columnCount (); j++) {
          item->child(i)->setBackground (j,palette().alternateBase() );
          }
        }
      rowBackground = !(rowBackground);
      }
    if (item->child(i)->isExpanded ()) {
      colorRowChildren (item->child(i),rowBackground); 
      }
    } 
  }




void KNutPrintUpsVar::slotReloadVars (void) {
  upsVarDef upsVar;

  if (m_upsNet->getUpsValues(true) !=0) {
    if (m_allowNotification) KNotification::event ("errorShowVars_event",KNutVarData::errorToText(m_upsNet->getError()).toLocal8Bit());
    }
  else {
    QVector<dataVarDef>::const_iterator it;
    for (it = m_dataVarList.begin(); it != m_dataVarList.end(); it++) {
      m_upsNet->readVars((*it).name, upsVar);
      (*it).value->setText (1, upsVar.upsValue);
      }
    }
  }

QTreeWidgetItem* KNutPrintUpsVar::addTreeWIdgetItem ( QTreeWidgetItem *rootWidget, QString varName, QString value) {
  QTreeWidgetItem* widgetItem;
  QTreeWidgetItem* helpWidgetItem;
  QString left;
  int n;

widgetItem = rootWidget;
  do {
    n = varName.indexOf(".");
    if (n > 0) {
      left = varName.left(n);
      varName = varName.mid(n+1);
      if (!(helpWidgetItem = existTreeWidgetItem( widgetItem,left))) {
        widgetItem = new QTreeWidgetItem( widgetItem);
        widgetItem->setText(0,left);
        m_treeWidget->expandItem(widgetItem);
        }
      else widgetItem = helpWidgetItem;
      widgetItem->setIcon(0,m_iconDir);
     }
    else { // end part
      helpWidgetItem = new QTreeWidgetItem( widgetItem);
      helpWidgetItem->setText(0, varName);
      helpWidgetItem->setText(1, value);
      m_treeWidget->expandItem(helpWidgetItem);
      return helpWidgetItem;
      }
   } while (n > -1);
  return NULL; // for compiler only  
 }


QTreeWidgetItem* KNutPrintUpsVar::existTreeWidgetItem (const QTreeWidgetItem* widgetItem, const QString name) {
  int itemCount = widgetItem->childCount();

  if ( itemCount == 0 ) return NULL;
  for (int i = 0; i < itemCount; i++) {
    if (widgetItem->child(i)->text(0) == name) return widgetItem->child(i);
    }
  return NULL;
  }

void KNutPrintUpsVar::slotTreeCollapsedExpanded (QTreeWidgetItem *) {

  colorRow();
  }

#include "moc_knutprintupsvar.cpp"

