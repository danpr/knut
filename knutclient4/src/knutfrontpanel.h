/***************************************************************************
                          knutfrontpanel.h  -  description
                             -------------------
    begin                : Tue Aug 21 2001
    copyright            : (C) 2001 by Daniel Prynych
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

#ifndef KNUTFRONTPANEL_H
#define KNUTFRONTPANEL_H

#include <KDE/KLed>

#include <QtGui/QFrame>


/**
  *@author Daniel Prynych
  */


class QFont;
class QPalette;
class QColor;
class QLCDNumber;
class QLabel;
class QVBoxLayout;
class QHBoxLayout;
class QGroupBox;

  
/**
 * Class is used for feature of panel, which shows informations about UPS.
 * Trida slouzi pro zobrazeni panelu, ktery zobrazuje informace o UPS.
 * 
 *
 * @author Daniel Prynych
 * @short Main panel
 * @version 0.8.0
 */
class KNutFrontPanel : public QFrame  {
  Q_OBJECT
  public:


enum KNFPConst { DefaultPanelFlags = 1615}; //1+2+4+8+64+512+1024
    
    
enum panelBulb {
  PBOnLine = 1, // Panel Bulb On Line
  PBOnBatt = 2, // Panel Bulb On Battery
  PBOver = 4,    // Panel Bulb Overloading
  PBBattLow = 8,    // Panel Bulb Battery Low
  PBRepBatt = 16,    // Panel Bulb Replace Battery
  PBCal = 32,    // Panel Bulb Calibration
  PBMFRMod = 64,    // FRM + Model
  PBMFR = 128,    // FRM
  PBModel = 256,    // Model
  PBSerial = 512,    // Serial
  PBFirm = 1024,    // Firmware Revision
  PBRun=2048    // Panel LCD Number Runtime
  };



/**
 * Constructor - makes panel, which shows status of UPS
 * Konstruktor - vytvori panel, ktery zobrazuje stav UPS-ky.
 *
 * @param flags Determine which parts will be shown.
 * @param panelFonts Pointer to font, which is used for painnting, when one is 0 (zero) systems font is used.
 * @param flags Udava ktere casti se maji na panelu zobrazit.
 * @param panelFonts Odkaz na fonts ktery se ma pouzit pro vykreslovani, pokud je = 0 (nula) pouzije se systemovy font pri aplikaci.
 * @since  0.9
 */
  KNutFrontPanel( const QPalette palette, const QFont panelFonts, QWidget *parent = 0, const unsigned int flags = DefaultPanelFlags);

/**
 * Destruktor
 *
 * @since  0.3
 */
  ~KNutFrontPanel();


/**
 * Vykresli na panelu udaje.
 * Paints values on panel.
 *
 * @since  0.4
 */
 void paintPanel( void);


/**
 * Vrati nastaveni panelu.
 * Gets setting of panel (flags of panel).
 *
 * @since  0.2
 */
 unsigned int getPanelFlags( void);

/**
 * Nastavi priznaky(nastaveni) panelu. Tyto priznaky nastavuji ktere prvky budou na panelu zobrazeny.
 * Sets flags of panel. This flags sets what items will be showed on panel.
 *
 * @param upsPanelFlags flags od panel.
 * @param upsPanelFlags priznaky panelu.
 *
 * @since  0.2
 */
 void setPanelFlags(const int upsPanelFlags);

/**
 * Shows complete name (name of UPS and manufacturer) of UPS on panel.
 * Zobrazi na panelu cele jmeno (jmeno UPS a vyrobce ) UPS-ky.
 *
 * @param upsPanelName Text which is showed on panel (address of UPS and manufacturer).
 * @param upsPanelName Text ktery se zobrazi na panelu (adresa UPS-ky a vyrobce).
 *
 * @since  0.2
 */
void setAllNameUps(const QString panelUpsAllName);

/**
 * Shows name of UPS on panel.
 * Zobrazi na panelu jmeno UPS-ky.
 *
 * @param upsPanelName Text which is shown on panel (name of UPS).
 * @param upsPanelName Text ktery se zobrazi na panelu (jmeno UPS-ky).
 *
 * @since  0.3
 */
  void setNameUps(const QString panelUpsName );

/**
 * Zobrazi na panelu jmeno vyrobce UPS-ky.
 * Shows name of UPS's manufacturer on panel.
 *
 * @param panelUpsFRM Text ktery se zobrazi na panelu (vyrobce UPS).
 * @param panelUpsFRM Text which is showed on panel (UPS's manufacturer).
 *
 * @since  0.2
 */
  void setMFRUps(const QString panelUpsFRM );

/**
 * Shows serial number of UPS on panel.
 * Zobrazi na panelu seriove cislo UPS-ky.
 *
 * @param panelUpsSerial Text ktery se zobrazi na panelu (seriove cislo UPS-ky).
 * @param panelUpsSerial Text which is showed on panel (serial number of UPS).
 *
 * @since  0.2
 */
  void setSerialUps(const QString panelUpsSerial );

/**
 * Zobrazi na panelu verzi firmwaru cislo UPS-ky.
 *
 * @param upsPanelFormRev Text ktery se zobrazi na panelu (verze firmwaru UPS-ky).
 *
 * @since  0.2
 */
  void setFirmRevUps(const QString upsPanelFormRev );

/**
 * Offs bulbs on panel. / Cleans panel.
 * Zhasne indikacni prvky na panelu.
 *
 * @since  0.5
 */
  void cleanPanel(void);

/**
 * Shows bulbs on panel.
 * Rozsviti indikacni prvky na panelu.
 *
 * @param panelBulbs Code for select lighted bulbs.
 * @param panelBulbs Kod pro urceni rozsvicenych indikacnich prvku.
 *
 * @since  0.4
 */
  void setPanel(const int panelBulbs);


/**
 * Sets value of runtime.
 * Nastavi hodnotu runtime.
 *
 * @param Runtime hodnota ktera se zobrazi jako runtime.
 *
 * @since  0.4
 */
 void setRuntime(const int runtime);

/**
 * Sets palette.
 * Nastavi paletu (palette). Predefinovana funkce from QWidget.
 *
 * @param Palette which mail panel is painted.
 * @param Paleta kterou ma byt vykreslen hlavni panel
 *
 * @since  0.1
 */
 void setPalette (const QPalette & );

  public slots:

/**
 * Changes font for painting.
 * Zmeni font pro vykresleni.
 *
 * @param panelFonts Novy font.
 *
 * @since  0.2
 */
  void slotSetPanelFont(QFont newPanelFont);

  private:

/**
 * @internal
 * Sets palettes for LCDLabel.
 * Nastavi palety pro LCDLabel.
 *
 * @since  0.1
 */
  void setLCDPalette (void);

/**
 * @internal
 * Makes change of palette of all items on based values m_palette
 * Provede zmenu palety vsech prvku na zaklade promenne m_palette
 *
 * @since  0.1
 */
  void setAllPalettes (void);

/**
 * @internal
 * Makes  font's change of all items, based on values m_panelFont and m_panelBoldFont
 * Provede zmenu fontu vsech prvku, na zaklade promennych m_panelFont and mpanelBoldFont
 *
 * @since  0.1
 */
  void changePanelFont (void);


  unsigned int m_panelFlags; // selects which of rows will be shown/ urcuji ktera radka bude rozsvicena 1 OL, 2 OB, 3 OVER, 4 RB, CAL 5 ...

  QPalette m_palette;

  QLabel* m_textLabel1;
  QLabel* m_textLabel2;
  QLabel* m_textLabel3;
  QLabel* m_textLabel4;
  QLabel* m_textLabel5;
  QLabel* m_textLabel6;
  QLabel* m_textLabel7;
  QLabel* m_textLabel8;
  QLabel* m_textLabel9;
  QLabel* m_textLabel10;
  QLabel* m_textLabel11;

  QLabel* m_textLabelManufac;
  QLabel* m_textLabelName;
  QLabel* m_textLabelSerial;
  QLabel* m_textLabelFirm;
  QLabel* m_textLabelRun;


  KLed* m_frameOL;
  KLed* m_frameOB;
  KLed* m_frameOVER;
  KLed* m_frameLB;
  KLed* m_frameRB;
  KLed* m_frameCAL;

  QLCDNumber* m_LCDNumber1;
  QPalette m_LCDPaletteOk;
  QPalette m_LCDPaletteErr;
  QPalette m_bulbsPalette;

  QColor m_upsOkColor;
  QColor m_upsErrorColor;
  QColor m_upsBackColor;
  QColor m_helpColor;

  QFont m_panelFont;        // font pro header
  QFont m_panelBoldFont;    // font for date

  QGroupBox* m_bulbsGroupBox;
  QGroupBox* m_messGroupBox;
  QGroupBox* m_valGroupBox;

  bool m_typeOfLCDColor;

  QVBoxLayout* m_mainLayout;

  QHBoxLayout* m_OLayout;
  QHBoxLayout* m_LBLayout;
  QHBoxLayout* m_RBLayout;
  QHBoxLayout* m_CALLayout;
  QHBoxLayout* m_manufacModelLayout;
  QHBoxLayout* m_manufacLayout;
  QHBoxLayout* m_nameLayout;
  QHBoxLayout* m_serialLayout;
  QHBoxLayout* m_firmLayout;
  QHBoxLayout* m_runLayout;
};

#endif

