/***************************************************************************
                          knutpanelmeter.h  -  description
                             -------------------
    begin                : Út b?e 16 2004
    copyright            : (C) 2004 by Daniel Prynych
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


#ifndef KNUTPANELMETER_H
#define KNUTPANELMETER_H

#include "knutanalog.h"
#include "knutvardata.h"

#include <QList>

class QWidget;
class QFont;

/**
  *@author Daniel Prynych
  */
class KNutPanelMeter : public QWidget  {
   Q_OBJECT
public:

/**
 * Konstruktor
 *
 * @Param aPanelPalette je paleta pozadi subpanelu MERAKU !!!
 * @param activeUpsInfo ukazatel na data o aktivni UPS
 * @param settingData ukazatel na zakladni data programu (barvy pozadi, mezikruzi, rucicky ... ,fonty atd )
 * @param analogRecords ukazatel na tridu ktera pracuje s daty o upsekach
 *
 * @since  0.4
 **/
  KNutPanelMeter(QPalette aPanelPalette, KNutVarData::upsInfoDef* activeUpsInfo, KNutVarData::SettingDataDef* settingData, KNutVarData* analogRecords, QWidget *parent=0);

/**
 * Destruktor
 *
 * @since  0.1
 **/
  ~KNutPanelMeter();

/**
 * Zrusi analogove panely vcetne uvoneni pameti.
 *
 * @since  0.1
 **/
  void deActivateAnalogs (void);


/**
 * Vytvori a inicializuje analogove panely.
 *
 * @since  0.1
 **/
  void activateAnalogs (void);

/**
 * Compute width a height of main panel, this widget
 *
 * @since  0.2
 **/
  void setSize ( void );


/**
 * Prekresli zmenu rozsahu pro dany typ analogoveho panelu (jenom jeden)
 * Pokud panel nepodporuje zmeny rozsahu neprovede nic.
 * Zmenu rozsahu podporuji nasledujici panely: Current,BattVolt,BattCurrent
 *
 * @param panelNumber is code of ups variable.
 * @param value je hodnota na kterou je panel nastaven.
 *
 * @since  0.1
 **/
  void repaintScaleAnalogPanel (int upsVarCode, float value);

/**
 * Paints pointer for selected analog panel
 * Prekresli rucicku pro vybrany analogovy panel.
 *
 * @param panelNumber is number of analog panel.
 * @param value is value of settimg amalog panel.
 * @param run = TRUE makes immediately repaint of panel 
 * @param panelNumber je cislo analogoveho panelu.
 * @param value je hodnota na kterou je panel nastaven.
 *
 * @since  0.2
 **/
  void repaintAnalogPointer (const int upsVarCode, const float value);


/**
 * Repaints analog panels
 * Repainting range of panel for frequency and voltage, based on current setting.
 * Prekresli a prebarvi analogove panely.
 * Prekresuje rozsah panelu pro frekvenci a nepeti na zaklade soucasneho nastaveni.
 *
 * @param repaintRangeFrequencyPanels urcuje ze ma prekreslit rozsahy panelu ktere zobrazuji vstupni a vystupni frequency.
 * @param repaintRangeVoltagePanels urcuje ze ma prekreslit rozsahy panelu ktere zobrazuji vstupni a vystupni napeti.
 * @param repaintAllPanels urcuje ze ma prebarvit vsechny prvky panelu. !!!!Neprekresluje rozsahy.
 *
 *@param repaintRangeFrequencyPanels sets repainting ranges of panels for input and output frequency
 *
 * @since  0.1
 **/
  void repaintAnalogPanels (const bool repaintRangeFrequencyPanels, const bool repaintRangeVoltagePanels, const bool repaintAllPanels);




/**
 * Changes type of digital processing of pointer and repaints it.
 * Zmeni zpusob digitalniho zpracovani rucucky a  prekresli ji
 *
 * @since  0.1
 **/
  void changeAnalogPanelsDigProcessing (void);


/**
 * Sets value of panels to zero
 * Nastavi hodnotu vsech panelu na nulu
 *
 * @since  0.1
 **/
   void zeroingPanels (void);

/**
 * Repaints backgroud of analog panels
 * Prebarvi pozadi analogovych panelu.
 *
 * @param taPanelPalette je hodnota nove palety pro pozadi
 *
 * @since  0.1
 **/
  void setAnalogBG(const QPalette &aPanelPalette);



/**
 * Vraci soucasnou velikost panelu.
 *
 * @since  0.1
 **/
  virtual QSize sizeHint (void) const;

public Q_SLOTS:

  /**
   * Sets new font for all panels
   * Nastavy novy font ve vsech panelech.
   *
   * @since  0.1
   **/
  void slotSetPanelsFont(const QFont &newFont);


private:
void moveUpsAnalog (const int i, KNutAnalog *upsAnalog);


/************ private const *****************/

  static const int spacePanels = 2; //vzdalenost mezi meraky, length between panels

  static const int maxBattVolt = 350;   //   max battery.voltage
  static const int maxCurrent = 50;    //     max output.current
  static const int maxBattCurrent = 200;   //  max battery.current
  static const int minBattCurrent = -300;   //  max battery.current
  static const int maxUpsPower = 5000; // max ups.power

/************ private struct ************************/


  struct analogPanelRecordDef {
    int upsVarCode;
    int scaleEnd;
    int scaleStart;
    KNutAnalog* upsAnalogs;
    };


/*********** private vars *******************/
  KNutVarData::SettingDataDef *m_settingData;  // v teto strukture jsou ulozeny promene ktere nastavuji konfiguraci

  KNutVarData::upsInfoDef *m_activeUpsInfo; // values for selected ups -- hodnoty pro vybrane ups

  KNutVarData *m_analogRecords;

  QPalette m_aPanelPalette;

  typedef QList <analogPanelRecordDef> analogPanelsList;
  analogPanelsList m_analogPanels;

  int m_widthArea;
  int m_heightArea;
};

#endif
