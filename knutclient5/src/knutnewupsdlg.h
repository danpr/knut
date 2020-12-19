/***************************************************************************
                          knutnewupsdlg.h  -  description
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

#ifndef KNUTNEWUPSDLG_H
#define KNUTNEWUPSDLG_H

#include "knutupsdata.h"
#include "knutvardata.h"

#include <KActionSelector>
//#include <KDialog>
#include <KPageWidgetItem>
#include <KComboBox>
#include <KConfigGroup>

#include <QListWidget>
#include <QDialog>
#include <QListWidgetItem>
#include <QLineEdit>

class QLabel;
class QGroupBox;
class QButtonGroup;
class QCheckBox;

/**
 * This class is dialog for makeing or correcting informations about UPS.
 * It can set address, port and selects variable its value will be found,
 * Tato trida je dialog pro vytvoreni nebo opravu informaci a UPS.
 * Umoznuje nastavit adresu, port, a vybrat promennou jejiz hodnota se bude zjistovat.
 * @author Daniel Prynych
 * @short Panel for setting parameters of UPS 
 *
 * @version 0.5
 */
class KNutNewUpsDlg : public QDialog
{
    Q_OBJECT

public:

/**
 * Constructor - makes configure dialog.
 * Konstruktor - vytvori konfiguracni dialog.
 *
 * @param settingData pointer on structure, which includes values which is setted in dialog.
 * @param upsdata is pointer on instance of class KNutUpsData.
 * @param analogData is pointer on instance of class KNutVardata
 * @param add Urcuje zda budeme data opravovat nebo pridavat.
 * @param settingData Je odkaz na strukturu ktera obsahuje hodnoty nastavovane v dialogu.
 * @param upsData Je odkaz na na instanci tridy KNutUpsData.
 * @param analogData Je odkaz na na instanci tridy KNutVarData.
 *
 * @since  0.8
 **/
  KNutNewUpsDlg (KNutUpsData* const upsData, KNutVarData* const analogData, const bool add = true, QWidget* parent = 0);

/**
 * @since  0.4
 **/
 ~KNutNewUpsDlg();

/**
 * Reads data from items of form.
 * Precte data z polozek formulare.
 *
 * @param upRecordDef Struktura of UpsInfo - contains all items which is included in form.
 * @param upsRecordDef Struktura UpsInfo - obsahuje vsechny polozky ktere jsou ve formulari.
 *
 * @since  0.5
 **/
 KNutUpsData::upsRecordDef getData (void ) const;

/**
 * Puts date into intems of form
 * Vlozi data do polozek formulare.
 *
 * @param upsRecord Struktura of upsRecordDef - contains all items which is included in form.
 * @param upsRecord Struktura psRecordDef - obsahuje vsechny polozky ktere jsou ve formulari.
 *
 * @since  0.5
 **/
  void putData (const KNutUpsData::upsRecordDef upsRecord );

/********************* PRIVATE **************************/

  protected Q_SLOTS:

/**
 * Is activated when button OK is pressed.
 * Provede se pri stisknuti tlacitka OK.
 *
 * @since  0.1
 **/
  void slotButtonOK(void);

/**
 * Is activated when button Help is pressed.
 * Provede se pri stisknuti tlacitka Help.
 *
 * @since  0.1
 **/
  void slotButtonHelp(void);

private:
/**
 * This class is inner class for showing items.
 * Add information about order number of UPS values.
 * Tato trida je vnorena trida pro zobrazeni polozek.
 * Pridava informaci o poradovem cisle UPS promennych.
 * @author Daniel Prynych
 * @short Item of listWidget
 *
 * @version 0.2
 */
    class KNutUpsWidgetItem : public QListWidgetItem
    {
    private:
    int m_ordVarCode;

    public:
    KNutUpsWidgetItem ( QListWidget* listWidget, const QString & text=QString::null, const int ordVarCode = 0 ) :
                   QListWidgetItem (text, listWidget), m_ordVarCode (ordVarCode)  {};
    int getVarCode (void) const {return m_ordVarCode;};
    void putGetCode (const int ordVarCode) {m_ordVarCode = ordVarCode;};
    };

/**
 * Is activated from slotButtonClicked,  when button ok is pressed.
 * Provede se pri stisknuti tlacitka ok ze slotButtonClicked.
 *
 * @since  0.1
 **/
  void makeOk();

  void moveToSelectedList (const int codeUpsVar);

  bool isExistSelectedItem  (const int codeUpsVar) const;

  bool addMember;   // true pokud se pridava polozka, false edit
  KNutUpsData *m_myUpsData; // Ups data object

  QLineEdit *m_lineEdit1;
  QLineEdit *m_lineEdit2;
  QLineEdit *m_lineEdit3;
  QLineEdit *m_lineEdit4;
  QLineEdit *m_lineEdit5;
  QLineEdit *m_lineEdit6;
  QLineEdit *m_lineEdit7;

  QCheckBox *m_savePassword;

  KActionSelector *m_upsValueSelector;
};

#endif
