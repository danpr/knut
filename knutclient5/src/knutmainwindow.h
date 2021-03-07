/***************************************************************************
                          knutmainwindow.h  -  description
                             -------------------
    begin                : Po cen 16 2003
    copyright            : (C) 2003 by Daniel Prynych
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

#ifndef KNUTMAINWINDOW_H
#define KNUTMAINWINDOW_H


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "knutprefdlg.h"
#include "knutupsdata.h"
#include "knutvardata.h"
#include "knutfrontpanel.h"
#include "knutnet.h"
#include "knutanalog.h"
#include "knutpanelmeter.h"


#include <KXmlGuiWindow>
#include <KToggleAction>
//#include <KAction>

#include <QDateTime>
#include <QAction>

class QWidget;
class QString;
class QStringList;
class QPalette;
class QSplitter;
class QProgressBar;


/**
 * Trida slouzi pro zobrazeni okna s informacemi o UPS.
 * Class is used for view window with informations about UPS
 *
 * @author Daniel Prynych
 * @short Hlavni okno
 * @version 0.4.1
 */
class KNutMainWindow : public KXmlGuiWindow {
  Q_OBJECT
  public:

/**
 * Constructor - makes main window.
 * Konstruktor - vytvori hlavni okno.
 *
 * @param tListKNutEvent pointer on QStringList contain text information about events (drop connection, switch ..) 
 * @param tActiveUpsInfo pointer on data about active UPS
 * @param tSettingData  pointer on basic data of program (background color, intercircle, pointers...,fonts..) 
 * @param tAnalogRecords pointer in class which works with data about UPS
 * @param tListKNutEvent ukazatel na QStringList obsahuje textove udaje o udalostech (ztrata spojeni, prepnuti atd.)
 * @param tActiveUpsInfo ukazatel na data o aktivni UPS
 * @param tSettingData ukazatel na zakladni data programu (barvy pozadi, mezikruzi, rucicky ... ,fonty atd )
 * @param tAnalogRecords ukazatel na tridu ktera pracuje s daty o upsekach
 *
 * @since  0.3.1
 **/
   KNutMainWindow(QStringList* tListKNutEvent, KNutVarData::upsInfoDef* tActiveUpsInfo, KNutVarData::SettingDataDef* tSettingData, KNutUpsData* tUpsRecords, KNutVarData* tAnalogRecords, QWidget* parent=0);

/**
 * Destructor
 *
 * @since  0.1.1
 **/
    ~KNutMainWindow();

/**
 * Repaints information of main panel.
 * Prekresli informace hlavniho panelu.
 *
 * @since  0.1.2
 **/
  void repaintPanel(void);


/**
 * Repaints analog panels based on their status.
 * Prekresli analogove panely na zaklade jejich stavu.
 * immediately repaint of panel 
 *
 * @since  0.3.1
 **/
  void repaintAnalogs ( void );

/**
 * Da prikaz k prekresleni analogovych panelu, hlavniho panelu a nastavi jejich hodnoty na
 * zaklade XFer hodnot.
 * Give command to repaint analog panels, main panel and sets their values based on XFer values
  * @param run = TRUE makes immediately repaint of analog panels
 *
 * @since  0.3.1
 **/
  void showUpsData (void);


/**
 * Adds other line into list of events and shows them on bottom part of panel.
 * Prida dalsi radku do seznamu udalosti a zobrazi je v dolni casti panelu.
 *
 * @param text text of event.
 * @param text udava text udalosti.
 *
 * @since  0.1
 **/
  void addComboStatusItem (const QString &text);

/**
 * Rebuilds main panel based on value changeSetting.
 * This value generates config windies
 * Prestavi hlavni panel na zaklade hodnoty changeSetting.
 * Tuto hodnotu generuje konfiguracni okno.
 *
 * @param changeSetting tell which setting was changed.
 * @param changeSetting udava jake nastaveni bylo zmeneno.
 *
 * @since  0.1.2
 **/
  void setConfig (unsigned int changeSetting);

/**
 * Sets ComboBox (for name of UPS)
 * Nastavi ComboBox (pro jmeno UPSky).
 *
 * &param name je jmeno UPSky ktere bude nastavena.
 * &param name is name of ups which will be set.
 *
 * @since  0.1
 **/
  void setCombo(const QString &name);
  
/**
 * Nastavi vsechny panely na nulovo hodnotu
 * Sets all pannels to zero value
 *
 * @since  0.2
 **/
  void zeroingPanels (void);

/**
 * Cleans front panel and panel meter (includes analog panels) .
 *
 * @since  0.1
 **/
  void cleanPanels (void);
  
/**
 * Sets and shows connect progress bar.
 * Nastavi a zobrazi connect progress bar.
 *
 * &param is count of parts of progress bar.
 * &param je pocet casti progres baru.
 *
 * @since  0.1
 **/
 void startConnectPB ( const int count );
 
/**
 * Hide connect progress bar.
 * Schova connect progress bar.
 *
 * @since  0.1
 **/
 void hideConnectPB ( void );


/**
 * Sets amount of progress for connect progress bar.
 * Nastavi delku vyplne pro connect progresbar.
 *
 * &param is amount of progress.
 * &param je delka vyplne.
 *
 * @since  0.1
 **/
 void setProgressConnectPB ( const int progress );

 
  Q_SIGNALS:
/**
 * This signal is emited when other UPS was selected.
 * Tento signal je vyslan pokud byla vybrana jina UPS-ka.
 *
 * @since  0.1
 **/
  void UPSChanged (QString);

/**
 * This signal is emited when request for viewing preferences window was ordered from menu.
 * Tento signal je vyslan pokud byl z menu zadan pozadavek na zobrazeni okna
 * pro nastaveni programu (preference).
 *
 * @since  0.1
 **/
  void preferenceUps (void);

/**
 * This signal is emited, when in menu was odered requirement for viewing list of values .
 * Tento signal je vyslan, pokud byl z menu zadan pozadavek na zobrazeni seznamu promennych.
 *
 * @since  0.1
 **/
  void showUPSVars (void);

/**
 * Tento signal je vyslan pokud byl z menu zadan pozadavek na zmenu RW promennych.
 * This signal is emited when request for change RW varianles was orederd from menu.
 *
 * @since  0.1
 **/
  void makeRWVars(void);


/**
 * Tento signal je vyslan pokud byl z menu vybran pozadavek na obnovu spojeni.
 * This signal is emited when request for refreshing connection was selected from menu.
 *
 * @since  0.1
 **/
  void makeRefreshConnection(void);

/**
 * This signal is emited when request for making instant commands was selected from menu.
 * Tento signal je vyslan pokud byl z menu zadan pozadavek na provedeni okamzitych prikazu.
 *
 * @since  0.1
 **/
  void makeInstComms(void);

/**
 * This signal is emited whem we need save configuration of program.
 * Tento signal je vyslan pokud potrebujeme ulozit konfiguraci programu.
 *
 * @since  0.1
 **/
 void saveConf(void);



/**
 * This signal is emited whem request for ending all application was selected from menu.
 * Tento signal je vyslan pokud byl z menu zadan pozadavek na ukonceni cele aplikace.
 *
 * @since  0.1
 **/
 void endApplication();

  protected:
  void closeEvent( QCloseEvent *e );



  private Q_SLOTS:
  void slotDescription(void);
  void slotPreferencesUps(void);
  void slotPrintUPSVars(void);
  void slotInstComms(void);
  void slotRWVars(void);
  void slotChangeRWVars ( QString );
  void slotCloseKNutClient (void);
  void slotChangeUPS (const QString &name);
  void slotConfigureKeys(void);
  void slotRefreshConnection(void);

/************** private common functions ************************/
  private:

  void setPalettes (void);


  void activeconfiguration (void);
  int activeMainCombo (void);
  void hideUpsData (void);

  void setToolBar (void);

  void saveConfiguration ( void );

/************ init functions ********************/
  void initVars ( void );
  void initAction ( void );
  void initStatusBar ( void );
  void initToolBarAction ( void );
  void initStatusBarMessage ( void );


/************** private vars **************************/
// Nazvy promenych UPS cislovany jsou od 0 do 10 "NONE", "UTILITY", "BATTPCT", "UPSTEMP", "ACFREQ", "LOADPCT", "AMBTEMP", "AMBHUMID", "BATTVOLT", "OUTVOLT", "CURRENT"

  QStringList* m_listKNutEvent;
  KNutVarData::upsInfoDef* m_activeUpsInfo; // values for selected ups / hodnoty pro vybrane ups
  KNutVarData::SettingDataDef* m_settingData;  // v teto strukture jsou ulozeny promene ktere nastavuji konfiguraci
  KNutUpsData* m_upsRecords;
  KNutVarData* m_analogRecords;


  QAction *m_quit;
  QAction *m_preferencesUps;
  KToggleAction *m_descriptionAction;

  QWidget *m_mainArea; // main arrea, on its will server main panel and analog panels 
                // hlavni plocha na ni umistime panel a analogove ukazatele

  QSplitter* m_mainSplitter;
  KNutFrontPanel* m_frontPanel;

  // nactene hodnoty promenych UPS, tyto promene se ctou jen jednou
  // readly values of UPS's variables, these varieables is read only once
  QString m_manufacturer;
  QString m_upsModel;
  QString m_serial;
  QString m_firmrev;
  bool m_existLowHighXfer;
  double m_lowXfer, m_highXfer;
  QPalette m_mainPalette, m_areaPalette, m_mPanelPalette, m_aPanelPalette;

  // Combo a pomocne struktury pro dolni radku
  // Combo and help structures for bottom line.
  KComboBox *m_infoCombo;
  KComboBox *m_upsComboBox;

  KNutPanelMeter* m_panelMeter;

  QProgressBar* m_connectProggresBar;
  QString m_progressFormatString; 

  };
#endif

