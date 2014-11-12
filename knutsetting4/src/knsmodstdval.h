/***************************************************************************
                          knsmodstdval.h  -  description
                             -------------------
    begin                : So b?e 12 2005
    copyright            : (C) 2005 by Daniel Prynych,Zajecov,Czech,
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

#ifndef KNSMODSTDVAL_H
#define KNSMODSTDVAL_H


#include "knscommondata.h"
#include "knsintlineedit.h"
#include "knsvalueedit.h"



#include <klineedit.h>
#include <knuminput.h>
#include <ktextedit.h>


#include <qwidget.h>

/**
  *@author Daniel Prynych,Zajecov,Czech,
  */

class QLabel;
class QString;
class QCheckBox;
class QHBoxLayout;


class KNSModStdVal : public QWidget  {
   Q_OBJECT
public: 
  KNSModStdVal(QWidget *parent=0);
  ~KNSModStdVal();

/**
 * Smaze vstupni widgety v tomto panelu
 * Clears input widgets in this panel;
 *
 * @since  0.1
 **/
  void clear(void);


/**
 * Nastavi vstupni widgety v tomto panelu
 * Sets input widgets in this panel;
 *
 * @param moduleIt je iterator seznamu modulu
 * @param moduleIt is iterator from list of modules
 *
 * @since  0.1
 **/
  void getData( modulesList::iterator moduleIt);

/**
 * Nacte data ze vstupnich widgetu.
 * Reads datas from input widgets in this panel.
 *
 * @param moduleIt je iterator seznamu modulu
 * @param moduleIt is iterator from list of modules
 *
 * @since  0.1
 **/
  void putData( modulesList::iterator moduleIt);


/**
 * Nastavi vsechny vstupni widgety pro zapis a cteni nebo jen pro cteni.
 * Sets all input widgets for writing and reading or reading only.
 *
 * @since  0.1
 **/
  void setReadOnly( const bool readOnly );


private:

  static const int sdOrderMin=-1;
  static const int maxStartDelayMin=1;

  void initValues(void);
  void enabledWidgets(const bool readOnly);


  QHBoxLayout* m_noLockLayout;

  KNSValueEdit* m_sdOrder;
  KNSValueEdit* m_maxStartDelay;

  QCheckBox* m_noLockUsed;
  KTextEdit* m_descTextEdit;

  bool m_enableButton;

};

#endif
