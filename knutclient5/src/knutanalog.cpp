 /***************************************************************************
 *   Copyright (C) 2007 by Daniel Prynych                                  *
 *   Daniel@prynych.cz                                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "knutanalog.h"

/* pocitani uhlu */
/* nula je ve 3 hodiny */
/* prirustek je kladny ve smeru hod rucicek */

#include <QApplication>

#include <QPolygon>
#include <QPainter>
#include <QFont>
#include <QColor>
#include <QFontMetrics>
#include <QBitmap>
#include <QColormap>

#include <stdio.h>
#include <math.h>


KNutAnalog::KNutAnalog( QWidget *parent, int bottomLimit, int topLimit, int typeOfView, int countOfParts, int countOfSubParts, QColor pointerColor, QColor scaleColor, QColor textColor, QFont *analogFonts, int qualityOfPointer )
    : QFrame( parent), m_bottomLimit(bottomLimit), m_topLimit(topLimit), m_typeOfView(typeOfView), m_countOfParts(countOfParts), m_countOfSubParts(countOfSubParts), m_pointerColor(pointerColor), m_scaleColor(scaleColor) ,m_textColor(textColor) {


qDebug("KNutAnalog::KNutAnalog");
  if (qualityOfPointer < 0 ) {
    qualityOfPointer = 0;
    } 
  else {
    if (qualityOfPointer > 5  ) {
      qualityOfPointer = 5;
      }
   } 

  if ( qualityOfPointer==0) {
    m_speedOfAAlias=none;
    m_makeBlurMotion=false;
    }
  else {
    if ((qualityOfPointer==2) || (qualityOfPointer== 4)) m_speedOfAAlias=slow; else m_speedOfAAlias=fast;
    if ((qualityOfPointer==3) || (qualityOfPointer== 4)) m_makeBlurMotion=true; else m_makeBlurMotion=false;
    }

  if (m_countOfParts > 6) m_countOfParts=6;
  else if (m_countOfParts < 2) m_countOfParts=2;

  if ( m_countOfSubParts > 6 ) m_countOfSubParts = 6;
  if ( m_countOfSubParts == 0 ) {
    if (m_countOfParts < 4) m_countOfSubParts =5;
    else {
      if (m_countOfParts < 6) m_countOfSubParts=4;
      else  m_countOfSubParts =3;
      }
    }

  m_valueOfPointer=m_bottomLimit; // sets starting value of pointer

  /* hodnoty zavisle na typu zobrazeni */
  switch (m_typeOfView) {
  case 1 :
    m_direction = 1; /* smer =1 ve smeru (clockwise), smer =-1 proti smeru hod. rucicek */
    m_angleOfViewer=270;
    m_startAngle=225;
    m_centerX=m_widthOfBackGround/2;
    m_centerY=m_widthOfBackGround/2;
    m_widthOfScale=118;
    m_radiusOfScaleNumbers=37;
    break;
  case 2 :
    m_direction = -1; /* smer =1 ve smeru (clockwise), smer =-1 proti smeru hod. rucicek */
    m_angleOfViewer=90;
    m_startAngle=0;
    m_centerX=13;
    m_centerY=110; 
    m_widthOfScale=210;
    m_radiusOfScaleNumbers=83;
    break;
  case 3 :
    m_direction = 1; /* smer =1 ve smeru (clockwise), smer =-1 proti smeru hod. rucicek */
    m_angleOfViewer=90;
    m_startAngle=135;
    m_centerX=m_widthOfBackGround/2;
    m_centerY=m_widthOfBackGround-15; 
    m_widthOfScale=170;
    m_radiusOfScaleNumbers=65;
    break;
  }
  // konec hodnot zavislych na typu zobrazeni

  if (analogFonts == 0) { // pokud neni predan font nastavi se systemovy font
//    m_scaleFont = kapp->font();
//    m_textFont = kapp->font();

    m_scaleFont = qApp->font();
    m_textFont = qApp->font();
  }
  else {
    m_scaleFont= *analogFonts;
    m_textFont= *analogFonts;
  }

  m_scaleFont.setBold (false);
  m_textFont.setBold (true);

  m_countOfColorParts = 0;
  setMinimumSize( AnalogWidth, AnalogWidth );
  setMaximumSize( AnalogWidth, AnalogWidth );
  setFrameStyle( QFrame::Box | QFrame::Raised );
  setLineWidth( 2 );

  m_backgroundBigImage = new QImage ( (m_widthOfBackGround) * 3,(m_widthOfBackGround)*3, QImage::Format_RGB32 );
  m_backgroundSmallImage = new QImage (m_widthOfBackGround, m_widthOfBackGround, QImage::Format_RGB32); 

  m_scaleLayerSmall = new QPixmap ( (m_widthOfBackGround), (m_widthOfBackGround)); //
  m_scaleLayerSmall->fill(palette().color(backgroundRole()).rgb()); //color if background

  setDefaultArcColor(); // sets standard registers // nastavime standartni registry

  }

KNutAnalog::~KNutAnalog() {

  delete (m_scaleLayerSmall);
}


void KNutAnalog::makeBackground (const bool run) {
qDebug("KNutAnalog::makeBackground");
  paintBackground();
  if (run) {
    repaint();
    }
  }


void KNutAnalog::setText (const QString &inscription, const QString &text_up) {

  m_inscription=inscription;
  AnMJ=text_up;
  }

void KNutAnalog::setDefaultArcColor(void) {

   m_arcColors[0] = Qt::white;
   m_arcColors[1] = Qt::green;
   m_arcColors[2] = Qt::yellow;
   m_arcColors[3] = Qt::red;
   m_arcColors[4] = Qt::blue;
   m_arcColors[5] = Qt::cyan;
   m_arcColors[6] = Qt::magenta;
   m_arcColors[7] = Qt::gray;
   m_arcColors[8] = Qt::black;
  }


void KNutAnalog::setArcColor(const int reg, const QColor &color ) {
  if ((reg > -1) && (reg < 9)) m_arcColors[reg] = color;
  }


void KNutAnalog::addArc (const double startOfCircle, const double endOfCircle, const int regColor) {

  if (m_countOfColorParts < 5) {
    m_extentOfDye[m_countOfColorParts].start=startOfCircle;
    m_extentOfDye[m_countOfColorParts].end=endOfCircle;
    if ((regColor <0) || (regColor > 8)) {
      m_extentOfDye[m_countOfColorParts++].color=0;
      }
    else { 
      m_extentOfDye[m_countOfColorParts++].color=regColor;
      }
    }
  }


void KNutAnalog::delArc (void) {

  m_countOfColorParts = 0;
  }



void KNutAnalog::setRange (const int Astart, const int Aend ,const bool run) {

  m_bottomLimit=Astart;
  m_topLimit=Aend;
/*dan*/  
  if ((m_bottomLimit != Astart) && (m_topLimit != Aend)) {
    // pokud zmenime rozasah musime prekrelit i rucicku
    if (run) {
       paintWholeLayer(true); // repaint pointer and background
       repaint ();
       }
    }
  }



void KNutAnalog::setMember (const int Count, const bool run) {

  m_countOfParts=Count;
  paintWholeLayer(true);
  if (run) {
    repaint ();
    }
  }



void KNutAnalog::setPointerColor (const QColor &pointerColor ) {

  m_pointerColor = pointerColor;
  }


void KNutAnalog::repaintPointer (void ) {

  paintWholeLayer (false);
  }


void KNutAnalog::setFontColor (const QColor &fontColor ) {

  m_textColor = fontColor;
  }


void KNutAnalog::setScaleColor (const QColor &scaleColor) {

  m_scaleColor = scaleColor;
  }


void KNutAnalog::setDigitalProcesing (const int qualityOfPointer, const bool run ) {

  int qop; 
  if (qualityOfPointer < 0) qop= 0;
  else {
    if (qualityOfPointer > 4) qop = 4;
    else qop = qualityOfPointer;
    }

  if ( qop==0) {
    m_speedOfAAlias=none;
    m_makeBlurMotion=false;
    }
  else {
    if ((qop==2) || (qop== 4)) {
      m_speedOfAAlias=slow;
      }
    else {
      m_speedOfAAlias=fast;
      }
    if ((qop==3) || (qop== 4)) {
      m_makeBlurMotion=true;
      }
     else {
       m_makeBlurMotion=false;
      }   
    }
  paintWholeLayer (true);
  if (run) repaint ();
}



void KNutAnalog::setPointer (const double pointerValue ) {
qDebug("KNutAnalog::setPointer");

  if (m_valueOfPointer != pointerValue) {
    m_valueOfPointer = pointerValue;
    paintWholeLayer (false);
    }
  }



//************** PUBLIC SLOTS  **********************************

void KNutAnalog::slotSetScaleFont(QFont newScaleFont){
  m_scaleFont = newScaleFont;
  paintWholeLayer(true);
  repaint();
  }

void KNutAnalog::slotSetTextFont(QFont newTextFont){
  m_textFont = newTextFont;
  paintWholeLayer();
  paintWholeLayer (true);
  repaint();
  }

void KNutAnalog::slotSetAllFont(QFont newAllFont){
  m_scaleFont = newAllFont;
  m_textFont = newAllFont;
  m_scaleFont.setBold (false);
  m_textFont.setBold (true);
  paintWholeLayer (true);
  repaint();
  }


void KNutAnalog::repaintAll (void) {
  paintWholeLayer(true);
  repaint ();
  }


//************** protected ****************************

void KNutAnalog::resizeEvent( QResizeEvent * ) {
}


void KNutAnalog::paintEvent( QPaintEvent *e ) {

  if (isVisible()) {

    QRect layerRect(0,0,m_widthOfBackGround, m_widthOfBackGround);
    QPoint leftTopPoint( m_startOfBackGround, m_startOfBackGround);

    QFrame::paintEvent(e);
    QPainter localPainter;

    localPainter.begin(this);
    localPainter.drawPixmap( leftTopPoint, *m_scaleLayerSmall, layerRect);
    localPainter.end();    
    }
  }


//***************private ************************

void KNutAnalog::paintBackground ( void ) {
  QPainter paint;

qDebug ("KNutAnalog::paintBackGround");

  m_backgroundBigImage->fill(palette().color(backgroundRole()).rgb()); //color if background
  paint.begin ( m_backgroundBigImage );
  paint.setBackgroundMode(Qt::OpaqueMode);
  paint.setBackground(palette().color(backgroundRole()));
  paintPartOfCircle(&paint, m_centerX*3, m_centerY*3,m_widthOfScale*3);  //CMK pro inter
  paintScale(&paint, m_centerX*3, m_centerY*3,m_widthOfScale*3);  // Scale
  paint.end();

  if (m_speedOfAAlias == none) {
    makeMotionBlur(m_backgroundBigImage,m_backgroundBigImage->pixel(1,1));
    *m_backgroundSmallImage =  makeAntialiasing(m_backgroundBigImage, m_backgroundBigImage->pixel(1,1),slow);
    }
  }


void KNutAnalog::paintWholeLayer( const bool repaintBackground) {
  QPainter paint;

qDebug ("KNutAnalog::paintWholeLayer");
  if (repaintBackground) paintBackground();

  if (m_speedOfAAlias == none) {
    QImage pointerSmallImage(*m_backgroundSmallImage);
    paintPointerFastly ( pointerSmallImage, m_valueOfPointer);
    *m_scaleLayerSmall = QPixmap::fromImage(pointerSmallImage);
    }
  else
    {
    QImage pointerBigImage (*m_backgroundBigImage);
    paintPointerSlowly ( pointerBigImage,  m_valueOfPointer, m_centerX*3, m_centerY*3,m_widthOfScale*3);
    makeMotionBlur(&pointerBigImage,pointerBigImage.pixel(1,1));
    QImage smallImage =  makeAntialiasing(&pointerBigImage, pointerBigImage.pixel(1,1),slow);
    *m_scaleLayerSmall = QPixmap::fromImage(smallImage);
   }

  paint.begin( m_scaleLayerSmall ); // paint on layer of scale
      // painting numbers
  paintScaleNumbers( &paint ); //painting of numbers
      // painting texts
  paintText( &paint );
  paintValueOfPointer ( &paint, m_valueOfPointer);
  paint.end ();
  } 



void KNutAnalog::paintPointerSlowly (QImage &pointerImage,  double position, int centerX , int centerY, int widthOfCircle) {
qDebug("KNutAnalog::paintPointerSlowly");

  QPainter paint;
  double angle;
  int lengthOfPointer = widthOfCircle / 2 - 12;

  if (position < m_bottomLimit) position=m_bottomLimit;
  else {
    if (position > m_topLimit) position = m_topLimit;
    }
  // painting of pointer
  angle = ((m_angleOfViewer/(m_topLimit-m_bottomLimit))*(position-m_bottomLimit))*((-1)*m_direction)+m_startAngle;

  paint.begin( &pointerImage ); // paints of pointer

  paint.setBackgroundMode(Qt::OpaqueMode);

  paint.setBrush( m_pointerColor ); // sets color
  paint.setPen ( m_pointerColor );

  QPolygon pts;
  // k posuvu a k otoceni pouziva funkce paintu
  // for rotate and translate uses functions of paint
  paint.translate (centerX, centerY);
  paint.rotate(-angle);
  pts.setPoints( 4, -6, 0, 0, -6, lengthOfPointer, 0, 0, 6 );
  paint.drawConvexPolygon( pts );

  paint.end ();
  }



void KNutAnalog::paintPointerFastly (QImage &pointerImage, double position) {
qDebug ("KNutAnalog::paintPointerFastly");

  double uhel;

  QPainter paint(&pointerImage);
  int delka = m_widthOfScale / 2 - 4;

  paint.setBackgroundMode(Qt::OpaqueMode);

  if (position < m_bottomLimit) position=m_bottomLimit;
  else {
    if (position > m_topLimit) position = m_topLimit;
    }
  /* painting of pointer*/
  uhel = ((m_angleOfViewer/(m_topLimit-m_bottomLimit))*(position-m_bottomLimit))*((-1)*m_direction)+m_startAngle;

  paint.setBrush( m_pointerColor ); // nastavime barvu
  paint.setPen ( m_pointerColor );
  QPolygon pts;
  // for translate a rotate uses functions of paint // k posuvu a k otoceni pouziva funkce paintu
  paint.translate (m_centerX, m_centerY);
  paint.rotate(-uhel);
  pts.setPoints( 4, -2, 0, 0, -2, delka, 0, 0, 2 );
  paint.drawPolygon( pts );
  }



void KNutAnalog::paintPartOfCircle ( QPainter *paint, int centerX , int centerY, int widthOfCircle ) {
  double Astart, Aend;

qDebug("KNutAnalog::paintPartOfCircle");
  // pocitani uhlu
  //nula je ve 3 hodiny
  // prirustek je kladny proti smeru hod rucicek

  //int extent = m_topLimit-m_bottomLimit; //rozsah
//  double extent = m_topLimit-m_bottomLimit; //rozsah
//  double jednotka = ((double)m_angleOfViewer/extent);
  double jednotka = ((double)m_angleOfViewer/(m_topLimit-m_bottomLimit));
  bool painted = false;

//  int smerMK = (-1)*m_direction;
  // paints parts of circle
  for (int i=0;i<m_countOfColorParts;i++) {
    if ((m_extentOfDye[i].start < m_topLimit) && (m_extentOfDye[i].end > m_bottomLimit)) {
      if (m_extentOfDye[i].start < m_bottomLimit) Astart = m_bottomLimit; else Astart = m_extentOfDye[i].start;
      if (m_extentOfDye[i].end > m_topLimit) Aend = m_topLimit; else Aend = m_extentOfDye[i].end;
      paint->setBrush ( m_arcColors[m_extentOfDye[i].color] );
      paint->setPen (Qt::NoPen);
      paint->drawPie((centerX-widthOfCircle/2),(centerY-widthOfCircle/2),widthOfCircle,widthOfCircle,(int)((((Astart-m_bottomLimit)*jednotka)*((-1)*m_direction)+m_startAngle)*16),(int)(((-1)*m_direction)*((Aend-Astart)*jednotka)*16));
      painted = true; // bylo provedeno vykresleni
      }
    }
  if ((m_countOfColorParts > 0) && (painted)) { // kreslime jen pokud bylo kreslena kruhova vysec
    // musime vybarvit stred kotouce
    //fills center of circle
    paint->setBrush ( paint->background().color() ); //The angles  are 1/16th of a degree
    paint->drawPie((centerX-(widthOfCircle/2)+18),(centerY-(widthOfCircle/2)+18),widthOfCircle-36,widthOfCircle-36,(int)(m_startAngle*16+90*m_direction),(int)((-m_direction)*(m_angleOfViewer*16+180)));
    }
  }



void KNutAnalog::paintScale ( QPainter *paint , int centerX , int centerY, int widthOfCircle) {

  int bottomMax = widthOfCircle/2 - 29;
  int bottom = widthOfCircle/2 -17 ;
  int smer_stup = (-1)*m_direction;
  paint->save();
  paint->setPen ( m_scaleColor);

  paint->translate( centerX, centerY );

  double stupne = smer_stup * (m_angleOfViewer/(m_countOfParts*(m_countOfSubParts)));

  // nakresleni prvni linky
  //paints of the first line
  paint->rotate( -m_startAngle);
  paint->setBrush ( m_scaleColor );
  paint->drawRect( bottomMax,-1,30,3 );

  for ( int i=0; i<(m_countOfParts); i++ ) {
    for ( int j=0; j<(m_countOfSubParts-1); j++) {
      paint->rotate( -stupne );
      paint->drawRect( bottom,-1,18,2 );
      }
    paint->rotate( -stupne );
    paint->drawRect( bottomMax,-1,30,3 );
    }
  paint->restore();
  }



QImage KNutAnalog::makeAntialiasing (QImage* bigImage, const QRgb colorOfBG, speedOfAAlias speed) {
  ulong imageRed;
  ulong imageGreen;
  ulong imageBlue;
  QRgb imageRgb;
  ulong sumOfRed;
  ulong sumOfGreen;
  ulong sumOfBlue;

qDebug ("KNutAnalog::makeAntialiasing");

  if (speed == slow) {
    sumOfRed =  (qRed ( colorOfBG ))*9;
    sumOfGreen = (qGreen ( colorOfBG ))*9;
    sumOfBlue = (qBlue ( colorOfBG ))*9;
    }
  else {
    sumOfRed =  (qRed ( colorOfBG ))*16;
    sumOfGreen = (qGreen ( colorOfBG ))*16;
    sumOfBlue = (qBlue ( colorOfBG ))*16;
    }
  int smallImageWidth=bigImage->width()/3;
  int smallImageHeight=bigImage->height()/3;

  QImage smallImage(smallImageWidth,smallImageHeight, bigImage->format());

  for (int x = 0; x < smallImageWidth; x++) {
    for (int y = 0; y < smallImageHeight; y++) {
      imageRed = 0;
      imageGreen = 0;
      imageBlue = 0;

      if (speed == slow) {
////  uses divide, slowly compute
          for (int xi = 0 ; xi < 3; xi ++) {
            for (int yi = 0 ; yi < 3; yi ++) {
            imageRgb = bigImage->pixel(x*3+xi,y*3+yi);
            imageRed +=  qRed ( imageRgb );
            imageGreen += qGreen ( imageRgb );
            imageBlue += qBlue ( imageRgb );
            }
          }
          if (((imageRed == sumOfRed) && (imageGreen == sumOfGreen) && (imageBlue == sumOfBlue))) {
            smallImage.setPixel (x,y,colorOfBG);
             }
          else {
            imageRed = imageRed / 9;
            imageGreen = imageGreen/9;
            imageBlue = imageBlue/9;
            smallImage.setPixel (x,y,qRgb(imageRed,imageGreen,imageBlue));
            }
          }
      else {
////  uses bitmove, fastly compute
          for (int xi = 0 ; xi < 3; xi ++) {
            for (int yi = 0 ; yi < 3; yi ++) {
              if ((xi == 1) && (yi == 1)) { // center of 3x3 area
                imageRgb = bigImage->pixel(x*3+xi,y*3+yi);
                imageRed +=  qRed ( imageRgb )*4;
                imageGreen += qGreen ( imageRgb )*4;
                imageBlue += qBlue ( imageRgb )*4;
                }
              else {
                if ((xi == 1) || (yi == 1))    {
                  imageRgb = bigImage->pixel(x*3+xi,y*3+yi);
                  imageRed +=  qRed ( imageRgb )*2;
                  imageGreen += qGreen ( imageRgb )*2;
                  imageBlue += qBlue ( imageRgb )*2;
                  }
                else {
                  imageRgb = bigImage->pixel(x*3+xi,y*3+yi);
                  imageRed +=  qRed ( imageRgb );
                  imageGreen += qGreen ( imageRgb );
                  imageBlue += qBlue ( imageRgb );
                  }
                }
              }
            }
          if ((imageRed == sumOfRed) && (imageGreen == sumOfGreen) && (imageBlue == sumOfBlue)) {
             smallImage.setPixel (x,y,colorOfBG);
             }
          else {
            imageRed = imageRed >> 4;
            imageGreen = imageGreen >> 4;
            imageBlue = imageBlue >> 4;
            smallImage.setPixel (x,y,qRgb(imageRed,imageGreen,imageBlue));
            }
        } //end of fastly
      }
    }
  return smallImage;
}


void KNutAnalog::makeMotionBlur (QImage* image, const QRgb colorOfBG) {

  ulong imageRed, imageGreen, imageBlue;
  QRgb imageRgb;
  QImage *layerImageBlur;
  ulong sumOfRed, sumOfGreen, sumOfBlue;

  sumOfRed =  (qRed ( colorOfBG ))*9;
  sumOfGreen = (qGreen ( colorOfBG ))*9;
  sumOfBlue = (qBlue ( colorOfBG ))*9;

  int imageWidth=image->width();
  int imageHeight=image->height();

  layerImageBlur = new QImage(*image);
  layerImageBlur->detach(); //costructor makes swallow copy, have to use detach

  for (int x = 1; x < imageWidth-2; x++) {
    for (int y = 1; y < imageHeight-2; y++) {
      imageRed = 0;
      imageGreen = 0;
      imageBlue = 0;


        for (int xi = -1 ; xi < 2; xi ++) {
          for (int yi = -1 ; yi < 2; yi ++) {
            imageRgb = layerImageBlur->pixel(x+xi,y+yi);
            imageRed +=  qRed ( imageRgb );
            imageGreen += qGreen ( imageRgb );
            imageBlue += qBlue ( imageRgb );
            }
          }

        if (!((imageRed == sumOfRed) && (imageGreen == sumOfGreen) && (imageBlue == sumOfBlue))) {
          imageRed = (int)(imageRed / 9);
          imageGreen = (int)(imageGreen/9);
          imageBlue = (int)(imageBlue/9);
          image->setPixel (x,y,qRgb(imageRed,imageGreen,imageBlue));
          }
      }
    }
  delete layerImageBlur;
}






void KNutAnalog::paintScaleNumbers ( QPainter *paint ) {
  //paintig of scaleNumbers

  int poz_x, poz_y, r_vyska, r_sirka;
  double posun, angle;

  double upi = 0.0174532;
  int smer_st = -m_direction;

  paint->save();  // ulozime
  QString cislo;
  paint->setPen(m_textColor);
  paint->setFont(m_scaleFont);

  QFontMetrics fm = paint->fontMetrics();
  r_vyska = ((fm.ascent()) / 2);
  posun = ((double)(m_topLimit-m_bottomLimit))/m_countOfParts;
  for (int i=0; i<=(m_countOfParts);i++) {
    cislo.sprintf( "%3.0f",m_bottomLimit+i*posun);
    cislo=cislo.trimmed();
    r_sirka = (fm.width(cislo))/2;
    angle = (m_startAngle+ ((m_angleOfViewer/m_countOfParts)*i*smer_st))*upi;
    poz_x = (int)((cos(angle)*m_radiusOfScaleNumbers+m_centerX) - r_sirka);
    poz_y = (int)((m_centerY-sin(angle)*m_radiusOfScaleNumbers) + r_vyska);
    paint->drawText (poz_x,poz_y,cislo);
    }
  paint->restore();
  }


void KNutAnalog::paintText ( QPainter *paint ) {
  QString nadpis1;
  QString nadpis2;
  int index;

  if (m_inscription.length() >1) {
    if ((index = m_inscription.indexOf(" ")) != -1) {
    // mezera nalezena space found
      nadpis1 = m_inscription.left(index);
      nadpis2 = m_inscription.mid(index+1,(uint)(m_inscription.length() - index - 1));
      }
    } else index = -1;

  paint->setPen(m_textColor);
  paint->setFont(m_textFont);
  QFontMetrics fm = paint->fontMetrics();
  int r_vyska = fm.height()/ 2;
  switch (m_typeOfView)	{
    // kruznice - kreslime dve radky nad sebou cemtrovane
    case 1 :
      if (index == -1 ) paint->drawText( m_centerX - fm.width(m_inscription)/2, 104 + r_vyska , m_inscription);
      else {
        // mame dve slova
        paint->drawText( m_centerX - fm.width(nadpis1)/2, 106 + r_vyska/2 - 2, nadpis1);
        paint->drawText( m_centerX - fm.width(nadpis2)/2, 106 + (r_vyska*3)/2 + 2, nadpis2);
        }
      paint->drawText( m_centerX - fm.width(AnMJ)/2, 44 + r_vyska , AnMJ);
      break;
      // ctvrt oblouk kreslime dve radky nad sebou centrovane v pravem hornim rohu
    case 2 :
      if (index == -1 ) paint->drawText( 121 - fm.width(m_inscription) , 6 + r_vyska , m_inscription);
      else {
        paint->drawText( 121 - fm.width(nadpis1) , 6 + r_vyska , nadpis1);
        paint->drawText( 121 - fm.width(nadpis2) , 10 + r_vyska*2 , nadpis2);
        }
      paint->drawText( m_centerX , 68 + r_vyska*2 , AnMJ);
      break;
    // horni oblouk
    case 3 :
      paint->drawText( m_centerX - fm.width(m_inscription)/2, 6 + r_vyska , m_inscription);
      paint->drawText( m_widthOfBackGround -5 -fm.width(AnMJ), m_widthOfBackGround -r_vyska, AnMJ);

      break;
    }
  }



void KNutAnalog::paintValueOfPointer ( QPainter *p, double value) {
qDebug("KNutAnalog::paintValueOfPointer");


  QString cislo;
  cislo.sprintf( "%5.1f",value);
  cislo=cislo.trimmed();

  if (value < m_bottomLimit) value=m_bottomLimit;
  else {
    if (value > m_topLimit) value = m_topLimit;
    }
  /* nakresleni rucicky*/
//  uhel = ((uhel_zobrazeni/(horni_mez-dolni_mez))*(value-dolni_mez))*((-1)*smer)+m_startAngle;

  p->setPen(m_textColor);
  p->setFont(m_textFont);
  QFontMetrics fm = p->fontMetrics();
  int r_vyska = fm.height()/ 2;


  switch (m_typeOfView) {
    case 1 :
      p->drawText( m_centerX - fm.width(cislo)/2, 75 + r_vyska , cislo);
    break;
    case 2 :
      p->drawText( m_centerX , 57 + r_vyska , cislo);
    break;
    case 3 :
      p->drawText( 5, m_widthOfBackGround -r_vyska, cislo);
    break;
    }
  }



#include "moc_knutanalog.cpp"

