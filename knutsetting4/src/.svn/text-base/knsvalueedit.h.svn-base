/***************************************************************************
                          knsvalueedit.h  -  description
                             -------------------
    begin                : ?t b?e 31 2005
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

#ifndef KNSVALUEEDIT_H
#define KNSVALUEEDIT_H

#include <KDE/KComboBox>
#include <KDE/KNumInput>
#include <KDE/KLineEdit>

#include <QtGui/QLabel>




/**
  *@author Daniel Prynych,Zajecov,Czech,
  */

enum typeOfValueWidget {
  VW_STRING =0,
  VW_NUMBER =1,
  VW_COMBO=2
  };


class QWidget;
class QString;
class QCheckBox;


class KNSValueEdit : public QLabel  {
   Q_OBJECT
public: 
  KNSValueEdit(QWidget *parent=0, const typeOfValueWidget type = VW_STRING, const bool force = false, const bool variable = true);
  ~KNSValueEdit();

  void setReadOnly (bool readOnly);

  void setMinValue (int min);
  void setMaxValue (int max);

  void setDefNumValue (int def);
  void setDefStringValue (QString def);
  void setDefComboValue (QString def);

  void setText (QString value);
  void setValue (int value);
  void addItem(const QString value, const QString notify);
  void setChecked (bool check);

  QString text (void);
  int value (void);
  bool isChecked (void);

  virtual QSize sizeHint(void) const;

  virtual QSize minimumSizeHint(void) const;


protected slots:

  void valueUsedSlot (void);
  void comboHighlightedSlot (const QString & text);



private:

  int m_minNum;
  int m_maxNum;

  int m_defNum;
  QString m_defString;

  QCheckBox* m_checkBox;
  QLabel* m_comboLabel;

  typeOfValueWidget m_type;

  QList<QString>* m_comboList;

  //when force is true value is switch on everytime,
  bool m_force;
  bool m_variable;

  KLineEdit* m_valueStringWidget;
  KIntNumInput* m_valueIntWidget;
  KComboBox* m_valueComboWidget;

};

#endif
