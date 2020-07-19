/***************************************************************************
 *   Copyright (C) 2007 by Daniel Prynych                                  *
 *   Daniel.Prynych@buzuluk.cz                                             *
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
#ifndef KNUTANALOG_H
#define KNUTANALOG_H

#include <QFrame>
#include <QImage>

#include <QPixmap>  // po odladeni smazat
#include <QMutex>


class QPainter;
class QFont;

enum speedOfAAlias { none,fast,slow};

/**
 * This class shows analog measuring panels
 *
 * @author Daniel Prynych
 * @short analog measuring panels
 * @version 0.4
*/

class KNutAnalog : public QFrame  {
  Q_OBJECT
public:

  struct extentOfDyeDef {
    double start,end;
    int color;
    };


/**
 * Constructor - makes object for analog measuring panel about size 130 x 130 points
 * Konstruktor - vytvori objekt pro analogovy merici panel o rozmerech 130 x 130 bodu
 *
 * When you need paint or repaint panel you have to call function repaint
 * other functions calls this function theirself, when run = true
 * Pokud je potreba vykreslit nebo prekreslit panel je nutno zavolat funkci repaint,
 * ostatni funkce ji zavolaji sami pokud je run = TRUE.
 *
 * @param bottomLimit Is bottom value of scale..
 * @param topLimit is end value of scale.
 * @param typeOfView determine type view of scale 1- circle 2- right part of circle, 3 - top part of circle
 * @param countOfParts determine count of big parts (parents parts) of scale.
 * @param countOfSubParts determine count small parts (children parts) id scale, evere big part include e few small part.
 * @param pointerColor  determine color of pointer.
 * @param scaleColor determine color of scale.
 * @param textColor determine color of  text.
 * @param analogFonts determine fonts for text and scale.
 * @param qualityOfPainter determine  quality painting of pointer- 0 nothing. 1 fast antialiasing, 2 slow antialiasing, 3 blur motion + fast antialiasing, 4 blur motion + slow antialiasing 
 * @since  0.3
 **/
  KNutAnalog(QWidget *parent = 0, int Astart = 0, int topLimit = 100, int typeOfView = 1, int countOfParts = 5, int countOfSubParts = 0, QColor pointerColor = Qt::black, QColor m_scaleColor = Qt::black, QColor textColor = Qt::black, QFont *analogFonts = 0, int qualityOfPointer = 4 );

/**
 * Destruktor
 * @since  0.2
 **/
    ~KNutAnalog();


/**
 * Sets text for top and bottom text of panel.
 * Nastavi text dolni a horni text na panelu.
 *
 * @param  m_inscription  determinuje inscription , bottom text , usually.
 * @param  m_inscription Udava nadpis, "dolni" text, obvykle co je mereno.
 * @param text_up Udava mernou jednotku "horni" text, obvykle merna jednotka.
 *
 * @since  0.3
 **/
  void setText (const QString &m_inscription, const QString &text_up);


/**
 * Makes whole layer of analog panel.
 * Vytvori celou vrstvu analogoveho panelu.
 *
 * @param repaintBackground = TRUE makes background of analog panel too. 
 * @param repaintBackground = TRUE nakresli take pozadi analogoveho panelu.
 *
 * @since  0.1
 **/
void paintWholeLayer( const bool repaintBackground = false);


/**
 * Makes background of analog panel.
 * Vytvori pozadi analogoveho panelu.
 *
 * @param run = TRUE makes immediately repaint of panel 
 * @param run = TRUE provede okamzite  prekresleni panelu.
 *
 * @since  0.3
 **/
  void makeBackground (const bool run=false);


/**
 * Repaint pointer of analog panel.
 * Prekresli rucicku analogoveho panelu.
 *
 * @since  0.3
 **/
  void repaintPointer (void );



/**
 * Nastavuje devet zakladnich barev pro mezikruzi.
 * Sets nine basic color for circle in scale
 *
 * @since  0.3
 **/
  void setDefaultArcColor(void);


/**
 * Nastavi zakladni barvu pro mezikruzi.
 * Sets basic color for circle in scale
 *
 * @param reg Poradove cislo barvy (barevneho registru).
 * @param color Barva na kterou bude nastaven registr barvy.
 *
 * @since  0.3
 **/
  void setArcColor(const int reg, const QColor &color );



/**
 * Vybarvi cast stupnice zadanou barvou.
 * Nastavi rozsah mezikruzi, podle stupnice od pocatecni hodnoty do koncove hodnoty stupnice
 * Barevne casti se barvy postupne, podel poradoveho cisla, mohou se prekryvat.
 * Je mozno nastavit jen 5 casti !!
 *
 * @param startOfCircle Urcuje zazatek mezikruzi.
 * @param endOfCircle Urcuje konec mezikruzi.
 * @param color Urcuje barvu.
 *
 * @since  0.3
 **/
  void addArc (const double startOfCircle, const double endOfCircle, const int regColor);



/**
 * Clear colors of scale
 * Zrusi vybarveni stupnice.
 *
 * @since  0.2
 **/
  void delArc (void);



/**
 *
 * Nastavi rozsah stupnice.
 *
 * @param Astart Urcuje pocatecni hodnotu stupnice.
 * @param AEND Urcuje koncovou hodnotu stupnice.
 * @param run = TRUE makes immediately repaint of panel 
 * @param run = TRUE provede okamzite prekresleni panelu.
 *
 * @since  0.3
 */
  void setRange (const int Astart = 0, const int Aend = 100, const bool run=false);

/**
 * Sets number of parts od scale
 * Nastavi pocet dilku stupnice.
 *
 * @param Count Determine number of parts of pointer..
 * @param Count Urcuje pocet casti stupnice.
 * @param run = TRUE makes immediately repaint of panel 
 * @param run = TRUE provede okamzite prekresleni panelu.
 *
 * @since  0.2
 */
  void setMember (const int Count, const bool run=false);

/**
 * Sets color of analog pointer.
 * Nastavi barvu analogove rucicky.
 *
 * @param pointerColor determine color of pointer.
 * @param pointerColor Urcuje barvu rucicky.
 * @since  0.3
 */
  void setPointerColor (const QColor &pointerColor);


/**
 * sets color of text;
 * Nastavi barvu textu.
 *
 * @param fontColor determine color of font.
 * @param fontColor urcuje barvu fontu.
 *
 * @since  0.3
 */
  void setFontColor (const QColor &fontColor );



/**
 * Sets color of scale
 * Nastavi barvu stupnice.
 *
 * @param scaleColor determine color if scale.
 * @param scaleColor urcuje barvu stupnice.
 *
 * @since  0.3
 */
  void setScaleColor (const QColor &scaleColor);


/**
 * Sets type of digital processing of pointer. 0 nothing. 1 fast antialiasing, 2 slow antialiasing, 3 blur motion + fast antialiasing, 4 blur motion + slow antialiasing.
 *
 * @param qualityOfPointer determine quality of pointer's digital procesing
 * @param run = TRUE makes immediately repaint of panel 
 * @param run = TRUE provede okamzite prekresleni panelu.
 *
 * @since  0.2
 */
  void setDigitalProcesing (const int qualityOfPointer, const bool run =false );



/**
 * Sets pointer on value pointerValue.
 * Posune rucicku na hodnotu pointerValue.
 *
 * @param pointerValue Urcuje hodnotu na jakou bude nastavena rucicka.
 *
 * @since  0.2
 */
  void setPointer (const double pointerValue );


/**
 * Const. determines width of analog panel in points.
 * Konstanta, udava velikost analogoveho panelu v bodech.
 *
 * @since  0.2
 */
  static const int AnalogWidth = 130;


  public Q_SLOTS:

/**
 * Sets font of scale, change is doing immediately
 * Nastavi font stupnice, zmena je provedena okamzite.
 *
 * @param newScaleFont Urcuje novy font hodnot stupnice.
 * @param newScaleFont Urcuje novy font hodnot stupnice.
 *
 * @since  0.2
 */
  void slotSetScaleFont(QFont newScaleFont);


/**
 * Sets font of text. change is doing immediately.
 * Nastavi font textu, zmena je provedena okamzite.
 *
 * @param newTextFont Urcuje novy font pro texty.
 *
 * @since  0.2
 */
  void slotSetTextFont(QFont newTextFont);


/**
 * Nastavi font stupnice a textu, zmena je provedena okamzite.
 *
 * @param newTextFont Urcuje novy font pro texty a hodnoty stupnice.
 *
 * @since  0.2
 */
  void slotSetAllFont(QFont newAllFont);


/**
 * Repaint layer of backgroud and layer of pointer,
 * all panel will be repainted
 * Da povel k prekreseni urovne pozadi a  urovne rucicky.
 * To znamena, ze bude prekreslen kompletne cely panel.
 *
 * @since  0.2
 */
  void repaintAll (void);


  protected:
  virtual void resizeEvent( QResizeEvent * );
  virtual void paintEvent( QPaintEvent * );

  private:


/**
 * @internal
 */
  void paintBackground ( void );


/**
 * @internal
 */
  void paintPointerSlowly (QImage &pointerImage, double position, int centerX , int centerY, int widthOfCircle);

/**
 * @internal
 */
  void paintPointerFastly (QImage &pointerImage,  double position);


/**
 * @internal
 */
  void paintPartOfCircle ( QPainter *paint, int centerX , int centerY, int widthOfCircle );


/**
 * @internal
 */
  void paintScale ( QPainter *paint , int centerX , int centerY, int widthOfCircle);


/**
 * @internal
 */
  QImage makeAntialiasing (QImage *myImage, const QRgb colorOfBG, speedOfAAlias speed = slow);


/**
 * @internal
 */
  void makeMotionBlur (QImage *myImage, const QRgb colorOfBG );


/**
 * @internal
 */
  void paintScaleNumbers ( QPainter *paint );

/**
 * @internal
 */
  void paintText ( QPainter *paint );


/**
 * @internal
 */
  void paintValueOfPointer ( QPainter *p, double value);



  static const int m_widthOfBackGround = 122;
  static const int m_startOfBackGround = 4;

  int m_bottomLimit;
  int m_topLimit;
  int m_typeOfView;   // udava typ meraku
  int m_countOfParts; // pocet policek na stupnici
  int m_countOfSubParts; // pocet dilku v policku na stupnici
  QColor m_pointerColor, m_scaleColor, m_textColor; // colors of pointer, text and scale

  QColor m_arcColors[9];
  QString m_inscription;
  QString AnMJ;
  int m_countOfColorParts; // count of color parts on scale
  double m_valueOfPointer; //  position of pointer;
  double m_angleOfViewer;
  double m_startAngle; //pocatecni_uhel;
  int m_centerX;
  int m_centerY;
  int m_direction;
  int m_widthOfScale;
  int m_radiusOfScaleNumbers;

  speedOfAAlias m_speedOfAAlias;
  bool m_makeBlurMotion;

  QFont m_scaleFont;
  QFont m_textFont;

  QPixmap *m_scaleLayerSmall;

  QImage *m_backgroundBigImage; // background image without antialiasing for KNUT antialiasing
  QImage *m_backgroundSmallImage;  // background image with antiakiasing for default image;

  extentOfDyeDef m_extentOfDye[5];

  QMutex m_pointerMutex;
};


#endif
