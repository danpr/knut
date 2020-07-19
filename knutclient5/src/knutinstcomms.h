/***************************************************************************
                          knutinstcomms.h  -  description
                             -------------------
    begin                : ct ríj 24 2002
    copyright            : (C) 2002 by Daniel Prynych
    email                : Daniel.Prynych@alo.cz
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KNUTINSTCOMMS_H
#define KNUTINSTCOMMS_H

#include "knutnet.h"

#include <KComboBox>

#include <QLineEdit>
#include <QDialog>


class QString;

/**
 * This class is graphic interface for runinng of instant commands.
 * Tato trida je graficke rozhrani pro spusteni okamzitych prikazu.
 *
 * @author Daniel Prynych
 * @short Okamzite prikazy
 * @version 0.7
*/
class KNutInstComms : public QDialog  {
Q_OBJECT
public:


/**
 * Construktor - Creates window for select of instant command.
 * Konstruktor - vytvori okno pro vyber okamziteho prikazu.
 *
 * @param uName Determines user name.
 * @param password Determines passwprd.
 * @param uName Urcuje uzivatelske jmeno.
 * @param password Urcuje heslo.
 * @param upsNet Je odkaz na instanci tridy KNutNet.
 *
 * @since  0.5
 **/
  KNutInstComms(QString* oldUserName, QString* oldPassword, const QString uName, const QString password, KNutNet* const upsNet, QWidget* parent = 0);

/**
 * Destruktor
 *
 * @since  0.4
 **/
  ~KNutInstComms();


/**
 * Vraci true pokud existuje pripojeni na server a vytvorilo se
 * ovladaci okno pro odeslani prikazu.
 *
 * @since  0.4
 **/
 bool isUpsOk (void) { return m_upsConnectOk; };


  Q_SIGNALS:

  void signalShowMessage (QString message, QString event);


private Q_SLOTS:
/**
 * @internal
 *
 * Provede akci, da prikaz k provedeni okamziteho prikazu.
 */
  virtual void slotOk();


private:

  KNutNet* m_upsNet;

  QString* m_oldUserName;
  QString* m_oldPassword;

  QLineEdit *m_lineEditName;
  QLineEdit *m_lineEditPassword;
  KComboBox *m_instCommBox;

  bool m_upsConnectOk;
};

#endif
