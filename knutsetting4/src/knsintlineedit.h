/***************************************************************************
                          knsintlineedit.h  -  description
                             -------------------
    begin                : Út kv? 18 2004
    copyright            : (C) 2004 by Daniel Prynych,,,
    email                : Daniel@Prynych.cz
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KNSINTLINEEDIT_H
#define KNSINTLINEEDIT_H



#include <KDE/KIntSpinBox>


#include <QtGui/QWidget>
#include <QtCore/QString>
#include <QtGui/QLineEdit>



/**
  *@author Daniel Prynych,,,
  */

class KNSIntLineEdit : public KIntSpinBox  {
   Q_OBJECT
public: 
  KNSIntLineEdit(int max, unsigned int length, QWidget *parent=0);
  ~KNSIntLineEdit();

  void setReadOnly ( bool readOnly );

signals:
  void dataChanged(void);


private slots:
  void valueChangedSlot( int );
  void textChangedSlot( const QString & text );


private:
  unsigned int m_length;


};

#endif
