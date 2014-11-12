/***************************************************************************
                          knsintaddress.cpp  -  description
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

#include "knsintaddress.h"

#include <QtGui/QLayout>
#include <QtGui/QLabel>
#include <QtCore/QString>
#include <QtGui/QWidget>


KNSIntAddress::KNSIntAddress(QWidget *parent, unsigned int first, unsigned int second, unsigned int third, unsigned int fourth, unsigned int fifth) : QFrame(parent) {
  
  QHBoxLayout* mainLayout = new QHBoxLayout(this);

  firstNumber = new KNSIntLineEdit(255,3,this);
  mainLayout->addWidget(firstNumber);

  QLabel* label2= new QLabel (".",this);
  mainLayout->addWidget(label2);

  secondNumber = new KNSIntLineEdit(255,3,this);
  mainLayout->addWidget(secondNumber);

  QLabel* label3= new QLabel (".",this);
  mainLayout->addWidget(label3);

  thirdNumber = new KNSIntLineEdit(255,3,this);
  mainLayout->addWidget(thirdNumber);

  QLabel* label4= new QLabel (".",this);
  mainLayout->addWidget(label4);

  fourthNumber = new KNSIntLineEdit(255,3,this);
  mainLayout->addWidget(fourthNumber);

  QLabel* label1= new QLabel (" / ",this);
  mainLayout->addWidget(label1);

  fifthNumber = new KNSIntLineEdit(32,2,this);
  mainLayout->addWidget(fifthNumber);
  
  firstNumber->setValue(first);
  secondNumber->setValue(second);
  thirdNumber->setValue(third);
  fourthNumber->setValue(fourth);
  fifthNumber->setValue(fifth);

  connect (firstNumber,SIGNAL(dataChanged()),this,SLOT(valueChangedSlot()));
  connect (secondNumber,SIGNAL(dataChanged()),this,SLOT(valueChangedSlot()));
  connect (thirdNumber,SIGNAL(dataChanged()),this,SLOT(valueChangedSlot()));
  connect (fourthNumber,SIGNAL(dataChanged()),this,SLOT(valueChangedSlot()));
  connect (fifthNumber,SIGNAL(dataChanged()),this,SLOT(valueChangedSlot()));
  }

  
KNSIntAddress::~KNSIntAddress(){
  }

  
QString KNSIntAddress::text (void) {
  QString retAddress("");

  retAddress = QString::number(firstNumber->value())+"."
              +QString::number(secondNumber->value())+"."
              +QString::number(thirdNumber->value())+"."
              +QString::number(fourthNumber->value())+"/"
              +QString::number(fifthNumber->value());
  return retAddress;
  }

void KNSIntAddress::setText (const QString& text) {

  if (!text.isEmpty()) {
    firstNumber->setValue(text.section(".",0,0).toInt());
    secondNumber->setValue(text.section(".",1,1).toInt());
    thirdNumber->setValue(text.section(".",2,2).toInt());
    fourthNumber->setValue(text.section("/",0,0).section(".",3,3).toInt());
    fifthNumber->setValue(text.section("/",1,1).toInt());
    }
  }

  void KNSIntAddress::setReadOnly ( bool readOnly ) {

    firstNumber->setReadOnly(readOnly);
    secondNumber->setReadOnly(readOnly);
    thirdNumber->setReadOnly(readOnly);
    fourthNumber->setReadOnly(readOnly);
    fifthNumber->setReadOnly(readOnly);
    }

    
void KNSIntAddress::valueChangedSlot ( void ) {

    emit textChanged (text());
    }

#include "knsintaddress.moc"
