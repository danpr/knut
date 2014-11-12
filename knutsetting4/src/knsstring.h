/***************************************************************************
                          knsstring.h  -  description
                             -------------------
    begin                : Út dub 16 2010
    copyright            : (C) 2010 by Daniel.Prynych
    email                : Daniel@prynych@.cz
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef KNSSTRING_H
#define KNSSTRING_H

#include <QtCore/QString>

/**
  *@author Daniel.Prynych
  */

class KNSString : public QString  {

public: 
  KNSString();
  KNSString( const QChar ch);
  KNSString( const QString & s);
  KNSString( const QByteArray & ba);
  KNSString( const QChar * unicode, uint length);
  KNSString( const KNSString & s);
  KNSString( const char* str);

  ~KNSString();

  KNSString extractWord(void);

  void toLowerOwn(void);

  void trimmedOwn(void);

  KNSString stripComment(void);

  void stripCommentOwn(void);


  KNSString envelop(void) const;




private:

  bool m_eFlagQuote;

};
#endif
