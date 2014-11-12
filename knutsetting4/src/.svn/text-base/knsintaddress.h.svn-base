/***************************************************************************
                          knsintaddress.h  -  description
                             -------------------
    begin                : So kv? 8 2004
    copyright            : (C) 2004 by Daniel Prynych,,,
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

#ifndef KNSINTADDRESS_H
#define KNSINTADDRESS_H

#include "knsintlineedit.h"

#include <KDE/KLineEdit>

#include <QtGui/QFrame>

class QWidget;
class QString;

/**
  *@author Daniel Prynych,,,
  */

class KNSIntAddress : public QFrame  {
   Q_OBJECT
public: 
  KNSIntAddress(QWidget *parent=0, unsigned int first=0, unsigned int second=0, unsigned int third=0, unsigned int fourth=0, unsigned int fifth =0);
  ~KNSIntAddress();

  QString text (void);

  void setText (const QString& text);

  void setReadOnly ( bool readOnly );

signals:
  void textChanged (const QString& text);    

private:
  KNSIntLineEdit* firstNumber;
  KNSIntLineEdit* secondNumber;
  KNSIntLineEdit* thirdNumber;
  KNSIntLineEdit* fourthNumber;
  KNSIntLineEdit* fifthNumber;

private slots:
  void valueChangedSlot ( void );

  
};

#endif
