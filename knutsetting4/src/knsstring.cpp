/***************************************************************************
                          knsstring.cpp  -  description
                             -------------------
    begin                : Út dub 16 2010
    copyright            : (C) 2010 by Daniel Prynych
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

#include "knsstring.h"


KNSString::KNSString() {
  }

KNSString::KNSString( const QChar ch) : QString(ch){
  }

KNSString::KNSString( const QString & s) : QString(s){
  }

KNSString::KNSString( const QByteArray & ba) : QString(ba){
  }

KNSString::KNSString( const QChar * unicode, uint length) : QString(unicode,length){
  }


KNSString::KNSString( const char* str) : QString(str){
  }

KNSString::KNSString( const KNSString & s) : QString(s){
  }


KNSString::~KNSString(void){
  }

void KNSString::trimmedOwn(void) {
  (*this)=this->trimmed();
  }

void KNSString::toLowerOwn(void) {
  (*this)=this->toLower();
  }
  
  
KNSString KNSString::extractWord(void){
  KNSString word("");

  int i = 0;
  (*this)=this->trimmed();

  if (length()==0) return QString();

  if (at(i) == '\"') {
    m_eFlagQuote =true;
    i++;
    }
  else m_eFlagQuote =false;

  while (i < length()) {
    if ((at(i) == '\\') && (!m_eFlagQuote)) {
       i++;
       if (!( i <length())) {
         remove (0,length());
         return word;
         }
       word +=at(i++);
       }
     else  {
       if (((at(i) == ' ') && (!m_eFlagQuote)) || ((at(i) == '=') && (!m_eFlagQuote)) || ((at(i) == '\"') && (m_eFlagQuote))) {
         
         if (at(i) == '=') {
           if (i == 0) {
              word += at(i++);
              }
           remove (0,i);
           }
         else {
           remove (0,++i);
           }
         return word;
         }
       else word  += at(i++);
       }
    }
  remove (0,length());
  return word;
  }

  
KNSString KNSString::envelop(void) const {
  KNSString line, inLine;
  bool spacesFound = false;
  int i;
  

  if (!this->isEmpty()) {
    inLine = (*this).trimmed();
    line = "";
    if ((inLine.indexOf('\\') != -1) || (inLine.indexOf('#') != -1) || (inLine.indexOf('\"') != -1)) {
      for (i=0; i < inLine.length(); i++) {
        if ((inLine.at(i) == '\\') || (inLine.at(i) == '#') || (inLine.at(i) == '\"') || (inLine.at(i) == ' '))
          line += '\\';
        line += inLine.at(i);
        }
      }
    else {
      line = inLine;
      if (inLine.indexOf(' ') !=-1 ) spacesFound = true;
      }            
    if (spacesFound) line = '\"'+line+'\"';
    return line;  
    }
  else return (*this);
  }

  
KNSString KNSString::stripComment(void) {
  int i;
  bool specialChar = false;
  bool inBracket = false;
  KNSString ownString;
  
  ownString = (*this);
  if (ownString.length()!= 0) {
    for (i=0; i < ownString.length(); i++) {
      if ((!specialChar) && ( ownString.at(i) == '\\')) specialChar=true;
      else {
        if (!specialChar) {
          if (ownString.at(i) == '\"') {
            inBracket = !inBracket;
            specialChar=false;
            continue;
            }
          if ((!inBracket) && (ownString.at(i)=='#')) {
            return ownString.left(i);
            }
          }
        specialChar=false;
        }
      } // end for
    }
  return (ownString);  
  }


void KNSString::stripCommentOwn(void) {

  (*this) = this->stripComment();  
  }
//QString::null
