/***************************************************************************
                          knutrwvar.h  -  description
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

#ifndef KNUTRWVAR_H
#define KNUTRWVAR_H

#include "knutnet.h"

#include <KDE/KDialog>
#include <KDE/KLineEdit>
#include <KDE/KLocale>
#include <KDE/KComboBox>


class QString;
class QGridLayout;

/**
 * This class is graphics interface for changes of RW variables.
 * Tato trida je graficke rozhrani pro zmenu RW promennych.
 *
 * @author Daniel Prynych
 * @short RW promenne
 * @version 0.3
*/
class KNutRWVar : public KDialog  {
  Q_OBJECT
  public: 
/**
 * Constructor - makes window for change of RW variable.
 * Konstruktor - vytvori okno pro zmenu RW promene.
 *

 * @param uName Describes user name.
 * @param password Describes password.
 * @param upsNet is pointer of intance of class KNutNet.
 * @param userName Urcuje uzivatelske jmeno.
 * @param userPassword Urcuje heslo.
 * @param uName Urcuje uzivatelske jmeno.
 * @param password Urcuje heslo.
 * @param upsNet Je odkaz na instanci tridy KNutNet.
 * @since  0.3
 **/
  KNutRWVar(QString* oldUserName, QString* oldPassword, const QString userName, const QString password, KNutNet* const upsNet, QWidget* parent = 0);

/**
 * Destructor
 *
 * @since  0.3
 **/
  ~KNutRWVar();

/**
 * Vraci true pokud existuje pripojeni na server a vytvorilo se
 * ovladaci okno pro nastaveni promennych.
 *
 * @since  0.4
 **/
  bool isUpsOk (void) { return m_upsConnectOk; };

  signals:

  void signalShowMessage (QString message, QString event);


  private slots:

/**
 * @internal
 *
 * Nastavi hodnotu a zpusob vyberu hodnoty promenne.
 */
  void slotChangeVar(int item);

/**
 * @internal
 *
 * Sets value of variable on default value
 * Nastavi hodnotu promenne na puvodni hodnotu.
 */
  virtual void slotDefault ();

/**
 * @internal
 *
 * Provede akci
 */
  virtual void slotOk();



/**
 * @internal
 *
 * Find asked item in box
 * Since at version 3.0.0 is not needed. 
 * Najde pozadovanou polozku v boxu.
 * Ve verzi qt > 3.0.0 neni potreba.
 */

  signals:
/**
 * Signal je aktivovam pokud je zmenen udaj na kterekoliv strance.
 *
 * @param changeSetting Udava co bylo zmeneno.
 *
 * @since  0.1
 **/
  void signalChangeRWVars (QString varName);


  private:
  KNutNet *m_upsNet;

  QString *m_oldUserName;
  QString *m_oldPassword;

  KLineEdit *m_lineEditName;
  KLineEdit *m_lineEditPassword;
  KComboBox *m_rWVarBox;
  KComboBox *m_valueVarBox;
  KLineEdit *m_valueVarLine;
  QGridLayout *m_passLayout;
  bool m_upsValueType;
  bool m_upsConnectOk;
  };

#endif
