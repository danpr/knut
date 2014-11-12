/***************************************************************************
                          knsmonsystems.h  -  description
                             -------------------
    begin                : ?t lis 25 2004
    copyright            : (C) 2004 by Daniel Prynych,,,
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

#ifndef KNSMONSYSTEMS_H
#define KNSMONSYSTEMS_H

#include "knscommondata.h"

#include <KDE/KPushButton>
#include <KDE/KLineEdit>
#include <KDE/KNumInput>
#include <KDE/KComboBox>
  

#include <QtGui/QWidget>

/**
  *@author Daniel Prynych,,,
  */

class QLabel;
class QString;
class QCheckBox;
class QRadioButton;
class QTreeWidget;
class QTreeWidgetItem;

class KNSMonSystems : public QWidget  {
   Q_OBJECT
  public: 
    KNSMonSystems(KNSCommonData* commonData, QWidget *parent=0);
    ~KNSMonSystems();

  /**
   * Smaze seznam UPScek (ListView) a zavola funkci clearWidgets.
   * Clears list of UPS (ListView) and calls functions clearWidgets
   *
   * @since  0.1
   **/
  void clear(void);


  void loadItems(void);

  void setMainButtons(const bool buttons = true);
  void setOkCancelButtons(const bool buttons = true);

  void setInputWidgetsReadOnly (const bool readOnly = true);


  /**
   * Smaze hodnoty (Jmeno UPSky , Adresu, Heslo, Jmeno uzivatele...) pro sledovani ve vstupni casti
   * Smaze jen ReadOnly widgety ne comboBoxy
   * Clears values (UPSName , Address Password, UserName ...) of monitoring in input part.
   * Clears ReadOnly widgets no combo boxes.
   *
   * @since  0.1
   **/
  void clearROWidgets(void);

  void deleteUps( const QString upsName );

  /**
   * Smaze zaznamy (prvky) z TreeView a z commonDate
   * jejiz jmeno uzivatele je rovno promenne userName
   * Deletes records (items) from listView and from commonData,
   * whose user Name is equal variable userName
   *
   * @param userName is name of user.
   * @param userName je jmeno uzivatele.
   *
   * @since  0.1
   **/
  void deleteUser( const QString userName );

  void addUser( const QString );


  /**
   * Nastavi heslo pro vybraneho uzivatele
   * Sets password for selected user
   *
   * @param name is name od user.
   * @param name je jmeno uzivatele.
   * @param password is password of user.
   * @param password je heslo uzivatele.
   *
   * @since  0.1
   **/
  void changePassword(const QString userName, const QString newPassword);


  /**
   * Nastavi zpusob sledovani pro vybraneho uzivatele
   * Sets type of monitoring for selected user
   *
   * @param name is name od user.
   * @param name je jmeno uzivatele.
   * @param newTypeMon is  user's type of monitoring.
   * @param newTypeMon je zpusob sledovani uzivatele.
   *
   * @since  0.1
   **/
  void changeTypeMon(const QString newUserName, const typeOfMonitoring newTypeMon);


  signals:
  void changed (void);

  public slots:
  void setStandardSlot (void);


  private slots:
  void selectedItemListSlot(QTreeWidgetItem* item, int );

  /**
   * Prida dalsi sekci.
   * Add other section
   *
   * @since  0.1
   **/
  void addItemSlot(void);


  /**
   * Opravi vybranou sekci.
   * Edit selected section
   *
   * @since  0.1
   **/
  void editItemSlot(void);


  /**
   * Smaze vybranou sekci.
   * Delete selected section
   *
   * @since  0.1
   **/
  void deleteItemSlot(void);

  /**
   * Odmitne pridani/opravu/smazani sekce.
   * Reject add/edit/delete section
   *
   * @since  0.1
   **/
  void cancelItemSlot(void);


  /**
   * Potvrdi pridani/opravu/smazani sekce.
   * Commit add/edit/delete section
   *
   * @since  0.1
   **/
  void okItemSlot(void);

  void localHostSelectedSlot(void);


  void userChangedSlot (const QString & name);


  private:

    enum typeAction {
      noneAction =0,
      addAction =1,
      editAction = 2,
      deleteAction =3
      };

  /**
   * Smaze hodnoty (Jmeno UPSky , Adresu, Heslo, Jmeno uzivatele...) pro sledovani ve vstupni casti
   * Smaze vsechny widgety take comboBoxy
   * Clears values (UPSName , Address Password, UserName ...) of monitoring in input part.
   * Clears alls widgets comboboxes too.
   *
   * @since  0.1
   **/
    void clearAllWidgets(void);


    void setUserComboBox (void);

    void setModulesComboBox (const QString editName);

    bool existLocalUps (const QString name);

    bool isPrepare(void);

    // fills input form 
    void setInputForm (void);

    typeAction m_action;

    KNSCommonData* m_commonData;
    bool m_enableButton;

    monitorUpsList* m_monitorUpsList;

    QTreeWidget* m_listViewWidget;

    KPushButton* m_addButton;
    KPushButton* m_editButton;
    KPushButton* m_deleteButton;
    KPushButton* m_standardButton;
    KPushButton* m_okButton;
    KPushButton* m_cancelButton;

    KLineEdit* m_upsName;
    KComboBox* m_upsNameBox;
    KLineEdit* m_hostName;
    KIntNumInput* m_powerValue;
    QLabel* m_powerValueLabel;
    KLineEdit* m_userName;
    KComboBox* m_userNameBox;
    KLineEdit* m_password;

    QCheckBox* m_localHostBox;

    QRadioButton* m_masterType;
    QRadioButton* m_slaveType;


    QString m_oldUpsName;
    QString m_oldHostName;


  };

#endif
