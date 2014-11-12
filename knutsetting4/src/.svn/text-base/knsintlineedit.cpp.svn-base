/***************************************************************************
                          knsintlineedit.cpp  -  description
                             -------------------
    begin                : Út kv? 18 2004
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

#include "knsintlineedit.h"

#include <QtGui/QValidator>
#include <QtGui/QFontMetrics>


KNSIntLineEdit::KNSIntLineEdit(int max, unsigned int length, QWidget *parent )
               :KIntSpinBox(0,max,1,0,parent,10) {

  int ourMaxWidth = 0;
  int charWidth;

  // find max width of number 
  QFontMetrics fm =  fontMetrics();
  for (int i = 0; i < 10; i++) {
    charWidth=fm.width(QString::number(i));
    if (charWidth > ourMaxWidth) ourMaxWidth = charWidth;
    }

  //  qt3  ourMaxWidth=(ourMaxWidth*(length))+upRect().width();

  ourMaxWidth=(ourMaxWidth*(length));

  setMaximumWidth(ourMaxWidth);
  setMinimumWidth(ourMaxWidth);

  setSingleStep(0);

  connect(this, SIGNAL(valueChanged(int)), this, SLOT(valueChangedSlot(int)));
  connect(this, SIGNAL(valueChanged(const QString&)), this, SLOT(textChangedSlot(const QString&)) );
  }

KNSIntLineEdit::~KNSIntLineEdit(){
  }


void KNSIntLineEdit::valueChangedSlot( int ) {

  emit dataChanged();
  }

void KNSIntLineEdit::textChangedSlot( const QString & text ) {

  //corrects empty text
  if (text == "") setValue(0);
  emit dataChanged();
  }


void KNSIntLineEdit::setReadOnly ( bool readOnly ) {

  if (readOnly) {
    KIntSpinBox::setReadOnly(true);
    setSingleStep(0);
    }
  else {
    KIntSpinBox::setReadOnly(false);
    setSingleStep(1);
    }
  }

#include "knsintlineedit.moc"
