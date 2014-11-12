/***************************************************************************
                          knutprintupsvar.h  -  description
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

#ifndef KNUTPRINTUPSVAR_H
#define KNUTPRINTUPSVAR_H

#include "knutupsdata.h"
#include "knutnet.h"


#include <KDE/KApplication>
#include <KDE/KDialog>
#include <KDE/KListWidget>
#include <KDE/KIcon>

#include <QtCore/QVector>
#include <QTreeWidget>
#include <QTreeWidgetItem>


class QLabel;
class QPixmap;
class QFramel;
class QString;

/**
  *@author Daniel Prynych
  */

/**
 * Tato trida zajistuje zobrazeni vsech promenych,
 * ktere podporuje prislusny ovladac/driver/model (ovladac) NUTu
 * pro urcenou UPS-ku
 * @author Daniel Prynych
 * @short NUT show UPS vars
 * @version 0.4
*/
class KNutPrintUpsVar : public KDialog  {

    Q_OBJECT

public:
/**
 * Constructor - makes window, reads values of variables
 * and will write names and values of variables
 * Konstruktor - vytvori okno, nacte hodnoty promennych,
 * a vypise jmena a hodnoty promenych.
 * @param upsnet Je odkaz na instanci KNutNet - obsahuje hodnoty promennych systemu NUT.
 * @param useDescription Urcuje zda pouzit pro promenne a prikazy taky popis.
 * pokud data o UPS-kach neexistuji vraci funkce upsOk vzdy false.
 *
 * since 0.3.1
 **/
  KNutPrintUpsVar(KNutNet* const upsNet, const bool useDescription, const bool allowNotification, QWidget* parent = 0);

/**
 * @since  0.1
 **/
  ~KNutPrintUpsVar();




private:


/********** private functions ****************/
  
/**
 * @internal
 * adds name into part of tree
 * Prida jmeno do casti stromu.
 * @param table is top of tree's part
 * @param varName is text which is addes into tree on the first column
 * @param varName is text which is addes into tree on the second column
 * @param table je vrchol casti stromu.
 * @param varName je text ktere se prida do stromu na prvni sloupec.
 * @param value je text ktere se prida do stromu na druhy sloupec.
 *
 * @since  0.3
 **/
  QTreeWidgetItem* addTreeWIdgetItem (QTreeWidgetItem *rootWidget, QString varName, QString value);


/**
 * @internal
 * Try if name is existed in subtree.
 *
 * widgetItem is root /mainItem/ of widget tree.
 * @param name is name which is found in subtree.
 *
 * @since  0.3
 **/
  QTreeWidgetItem* existTreeWidgetItem (const QTreeWidgetItem *widgetItem, const QString name);


/**
 * @internal
 * Obarvi radky.
 *
 * @since  0.1
 **/
  void colorRow (void);


/**
 * @internal
 *
 * @since  0.1
 **/
  void colorRowChildren (const QTreeWidgetItem *item, bool &rowBackground );


private slots:

/**
 * @internal
 * Nacte a prepise hodnoty promenych
 *
 * @since  0.3
 **/
  void slotReloadVars (void);


/**
 * @internal
 * Repaint background of tree,
 *
 * @since  0.1
 **/
void slotTreeCollapsedExpanded (QTreeWidgetItem *); 


/****** private vars ************/
private:

  KNutNet* m_upsNet;
 
  bool m_allowNotification;

  struct dataVarDef {
    QString name;
    QTreeWidgetItem* value;
    };

  QVector<dataVarDef> m_dataVarList;

  QTreeWidget* m_treeWidget;

  KIcon m_iconDir;
};

#endif
