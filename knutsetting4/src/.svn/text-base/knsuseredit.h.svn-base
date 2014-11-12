/***************************************************************************
                          knsuseredit.h  -  description
                             -------------------
    begin                : So srp 28 2004
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

#ifndef KNSUSEREDIT_H
#define KNSUSEREDIT_H

#include "knsdoublelistbox.h"
#include "knscommondata.h"

#include <KDE/KLineEdit>


#include <QtGui/QGroupBox>


static const QString instcmdStringDef = "all load.off load.on shutdown.return shutdown.stayoff \
shutdown.stop shutdown.reboot shutdown.reboot.graceful test.panel.start test.panel.stop \
test.failure.start test.failure.stop test.battery.start test.battery.stop calibrate.start \
calibrate.stop bypass.start bypass.stop reset.input.minmax reset.watchdog beeper.on beeper.off";


/**
  *@author Daniel Prynych,,,
  */

class QWidget;
class QRadioButton;
class QCheckBox;
class QButtonGroup;
class QString;
class QStringList;

class KNSUserEdit : public QGroupBox  {
   Q_OBJECT
public: 
  KNSUserEdit( QWidget *parent=0);
  ~KNSUserEdit();


/**
 * Nastavi hodnotu editacnich poli (jmeno, heslo).
 * Sets value of edit's arrays (name, password ...)
 *
 * @param name urcuje jmeno sekce.
 * @param name determine name of section.
 * @param password urcuje heslo sekce.
 * @param password determine password of section.
 * @param isUser urcuje zda jde o sekci uzivatelskou nebo pro sledovani.
 * @param isUser determine if section is user section or monitoring section..
 * @param allowFrom is list of allow addresses for settng or monitoring.
 * @param allowFrom je seznam povolenych adres pro nastaveni nebo sledovani.
 * @param actions je retezec povolenych akci
 * @param actions is chain of allow actions
 * @param instCmds je seznam povolenych okamzitych prikazu
 * @param instCmds is list of allow instant commands
 * @param typeMon urcuje typ sledovani (true = master, false = slave)
 * @param typeMon determine type of monitoring (true = master, false = slave);
 *
 * @since  0.3
 **/
  void putData (const QString name, const QString password, const bool isUser, const QString actions, const QStringList instCmds, const typeOfMonitoring typeMon);

/**
 * Cte hodnotu editacnich poli (jmeno, heslo).
 * Reads value of edit's arrays (name, password ...)
 *
 * @param name urcuje jmeno sekce.
 * @param name determine name of section.
 * @param password urcuje heslo sekce.
 * @param password determine password of section.
 * @param isUser urcuje zda jde o sekci uzivatelskou nebo pro sledovani.
 * @param isUser determine if section is user section or monitoring section..
 * @param actions je retezec povolenych akci
 * @param actions is chain of allow actions
 * @param instCmds je seznam povolenych okamzitych prikazu
 * @param instCmds is list of allow instant commands 
 * @param typeMon urcuje typ sledovani (1 = master, 0 = slave)
 * @param typeMon determine type of monitoring (1 = master, 0 = slave);
 *
 * @since  0.2
 **/
  void getData (QString& name, QString& password, bool& isUser, QString& actions,QStringList& instCmds, typeOfMonitoring& typeMon);



/**
 * Vraci hodnotu editacniho pole password.
 * Returns value of edit's array password.
 *
 * @since  0.1
 **/
  QString getPassword (void);

/**
 * Vraci hodnotu prepinacich tlacitek, Sledovani UPS (Master/Slave).
 * Returns value of radio buttons, UPS Monitoring (Master/Slave)
 * @return typeMon urcuje typ sledovani (1 = master, 0 = slave)
 * @return typeMon determine type of monitoring (1 = master, 0 = slave);
 *
 * @since  0.1
 **/
  typeOfMonitoring getTypeMon (void);


/**
 * Vraci hodnotu prepinacich tlacitek, Typ sekce (Uzivatel/Sledovani) .
 * Returns value of radio buttons, Type of sections (User/Monitoring)
 * @return isUser urcuje zda jde o sekci uzivatelskou nebo pro sledovani.
 * @return isUser determine if section is user section or monitoring section..
 *
 * @since  0.1
 **/
  bool getIsUser (void);


/**
 * Activuje nebo deaktivuje pole pro jmeno
 * Enable or disable array for name
 *
 * @param readOnly true deactivuje a false aktivuje pole
 * @param readOnly true disables and false enables array
 *
 * @since  0.1
 **/
  void setReadOnlyName (const bool readOnly);


  void setReadOnly (const bool readOnly);



/**
 * Vycisti editacni pole (jmeno, heslo ....)
 * a nastavi pole pro povo;ene adresy a povolene prikazy
 * Cleans up edit's arrays (name, password ...)
 * and sets arrays for allow address and allow commad
 *
 *
 * @since  0.1
 **/
  void init (void);


/**
 * Vycisti editacni pole (jmeno, heslo ....)
 * Cleans up edit's arrays (name, password ...)
 *
 *
 * @since  0.1
 **/
  void clear(void);

/**
 * Tests if name and password are empty
 * signal nameAndPasswordAreEmpty is emited
 *
 **/

void testAreNamePasswordEmpty (void);


private slots:
  void namePasswordChangedSlot(const QString &);

signals:
  void nameAndPasswordAreEmpty (bool);



protected slots:
  void userClickedSlot(bool on);



private:

  //*********** VARIABLES *********
  //list of command's name
  QStringList m_instcmdDef;


  QGroupBox* m_actionsGroup;
  QGroupBox* m_monitorGroupBox;

  KLineEdit* m_nameEdit;
//  KPasswordEdit* passwordEdit;
  KLineEdit* m_passwordEdit;

  QRadioButton* m_userRadio;
  QRadioButton* m_monitorRadio;

  QCheckBox* m_SETCheckBox;
  QCheckBox* m_FSBCheckBox;

  QRadioButton* m_masterRadio;
  QRadioButton* m_slaveRadio;

  KNSDoubleListBox* m_commandWidget;

};

#endif
