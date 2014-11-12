/***************************************************************************
                          knsdoublelistbox.h  -  description
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

#ifndef KNSDOUBLELISTBOX_H
#define KNSDOUBLELISTBOX_H


#include <KDE/KPushButton>  

#include <QtGui/QGroupBox>



/**
  *@author Daniel Prynych,,,
  */

class QWidget;
class QStringList;
class QListWidget;
class QListWidgetItem;

class KNSDoubleListBox : public QGroupBox  {
   Q_OBJECT
public: 
  KNSDoubleListBox(QWidget *parent=0, const QString title="");
  ~KNSDoubleListBox();

  void clear (void);

/**
 * Naplni levy (hlavni) box.
 * Fills left (main) box
 *
 * @since  0.1
 **/
  void setMainList (const QStringList list);

  
/**
 * Naplni pravy (vyberovy) box.
 * Pred pouzitim teto funkce, musite zavolat funkci setMainList se seznamem vsech jmen.
 * Jmena ze senamu jsou presunuta z leveho do praveho boxu.
 * Vsechna jmena jsou vlozena do seznamu pristupovych jmen.
 * Fills right (selected) box.
 * Before using this function, you must call function setMainList with list of all names.
 * Names from list are moved from left to right box.
 * All names are included to list of access names.
 *
 * @since  0.1
 **/
  void setSelectList (const QStringList list);


/**
 * Renames name (item) in left (main) and right (slected) box. Rename name in list if access names
 * Prejmenuje jmeno (prvek) v levem (hlavnim) a pravem (vyberovem) boxu.
 * Prejmenuje jmneo v seznamu pristupovych jmen
 *
 * @since  0.1
 **/
  void renameSeletedItems(const QString oldName, const QString newName);


  void setReadOnly (const bool readOnly);


/**
 * Get list of access names.
 * Ziska seznam pristupvych jmen
 *
 * @since  0.1
 **/
  QStringList getSelectList (void);



/**
 * Znovu naplni pravy (vyberovy) box.
 * Pred pouzitim teto funkce, musite zavolat funkci setMainList se seznamem vsech jmen.
 * Jmena ze senamu pristupovych jmen jsou presunuta z leveho do praveho boxu.
 * Fills right (selected) box again for filling uses list of access names.
 * Before using this function, you must call function setMainList with list of all names.
 * Names from list of access names are moved from left to right box.
 *
 * @since  0.1
 **/
  void reviewSelectList (void);


private slots:


/**
 * Presune prvek z prave casti do leve.
 * Moves item from right box to left box
 *
 * @since  0.1
 **/
  void moveToLeftSlot(void);

/**
 * Presune prvek z leve casti do prava.
 * Moves item from left box to right box
 *
 * @since  0.1
 **/
  void moveToRightSlot(void);



private:

  QListWidget* m_leftBox;
  QListWidget* m_rightBox;

  KPushButton* m_moveRightButton;
  KPushButton* m_moveLeftButton;

  QStringList* m_selectedItems;

};

#endif
