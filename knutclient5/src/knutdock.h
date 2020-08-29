/***************************************************************************
                          knutdock.h  -  description
                             -------------------
    begin                : Út zari 23 2003
    copyright            : (C) 2003 by Daniel Prynych
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

#ifndef KNUTDOCK_H
#define KNUTDOCK_H

#include "knutupsdata.h"
#include "knutvardata.h"
#include "knutconst.h"

#include <KStatusNotifierItem>

#include <QObject>
#include <QSystemTrayIcon>
#include <QBrush>
#include <QIcon>


class QAction;
class QString;
class QImage;
class QColor;
class QBrush;

/*KF5*/
/**
 * This class makes icons in dock.
 * Tato trida zajistuje vytvoreni icony v docku.
 *
 * @author Daniel Prynych
 * @short Dock's icons
 * @version 0.5
  */
class KNutDock : public KStatusNotifierItem  {

  Q_OBJECT
  public:

  enum KNTDconst { DefaultToolTipFlags = 0x398};


  enum upsConnection {
   idle, // ups isn't connected
   connecting, // ups is connecting
   ok, // ups is connected
   error // error of connecting
   };

  enum eTypeOfIcons { pictureKI = 0, generalKI};


  enum toolTipItems {
   TTMFR = 0x8, // 8,    // FRM
   TTModel = 0x10, // 16,    // Model
   TTSerial = 0x20, // 32,    // Serial
   TTFirm = 0x40, //64,    // Firmware Revision
   TTRun = 0x80,// 128,    // Runtime
   TTCharge = 0x100, // 256,    // Charge
   TTLoad = 0x200 // 512    // Load
   };




/**
 * Constructor - makes icon in dockbar.
 * Konstruktor - vytvori iconu v docku.
 *
 * @param  activeUpsInfo pointer to structure upsInfo
 * @param  settingData  pointer to structure type of SettingDataDef
 * @param  upsRecord pointer to class tActiveUpsInfo
 * @param  parent is ignored
 * @param  activeUpsInfo odkaz na structuru upsInfo
 * @param  settingData  odkaz na strukturu typu SettingDataDef
 * @param  upsRecord odkaz na instanci tridy tActiveUpsInfo
 * @param  upsRecord pointer to class tActiveUpsInfo
 * @param  parent je ignorovan
 * @since  0.4
 * Constructor calls parent constructor with parametr NULL because it
 * uses own working of signal activated because in parent class signal activated is connected to activatedOrHide,
 * but procedure activatedOrHide doesn't work when parent object is 0 / NULL.
 *
 **/
    KNutDock(KNutVarData::upsInfoDef* activeUpsInfo, KNutVarData::SettingDataDef* settingData, KNutUpsData* upsRecords, QWidget* parent=0);

/**
 * Destruktor.
 *
 * @since  0.1.2
 **/
    ~KNutDock();


/**
 * Nastavi zakladni nastaveni pro dock.
 * Sets basic setting for dock.
 *
 * @since  0.1
 **/
 void clearDockValues(void);


/**
 * Vytvori menu (seznam) UPS.
 * Makes menu (list) of UPS.
 *
 * @since  0.1
 **/
  void changeUps(void);


/**
 * Zmeni styl/typ icon
 * Changes style/type of icon
 *
 * @since  0.1
 **/
  void changeKIcon (void);

/**
 * Zmeni pozadi icony v kickeru
 * Changes background of icon in kicker
 *
 * @since  0.2
 **/
  void changeKIBColor(void);


/**
 * Zmeni prvky v tooltip-u
 * Changes items in toolTip
 *
 * @since  0.1
 **/
 void changeToolTip(void);

/**
 * Provede nove zpracovani dat a jejich nakresli iconu docku, interni repaint je volan vzdy.
 * Tato funkce take vola funkci ToolTip.
 * Makes new processing of data and paints icons of dock, internal repaint is colled always.
 * This function calls function ToolTips too.
 * @param always Kdyz je true prorovede vykresleni vzdy, kde je false provede vykresleni jen kdyz je zmenena nejaka hodnota UPS.
 * @param always When param is true, icon is made always, is one is false icon is makeing when same UPS's value is changed.
 *
 * @since  0.5
 **/
  void repaintDock ( const bool always = false);


/**
 * Nastavi v Menu oznaceni pro aktivni UPS.
 * Sets mark for active UPS in menu.
 *
 * @since  0.1
 **/
  void setNewUPS(void);


 Q_SIGNALS:
/**This signal is emited when end of application was selected in menu.
 * Tento signal je vyslan pokud bylo vybrano v menu ukonceni aplikace.
 * 
 *
 * @since  0.1
 **/
  void endApplication(void);

/**
 * Tento signal je vyslan pokud bylo vybrano v menu minimalizace aplikace.
 * Tento signal is emited when minimize application was selected in menu.
 * 
 *
 * @since  0.1
 **/
  void minimize(void);

/**
 * Tento signal je vyslan pokud bylo vybrano v menu polozka preferences.
 * This signal is emited when item preferences was selected in menu.
 *
 * @since  0.1
 **/
  void preferences(void);



/**
 * Tento signal je vyslan pokud byla vybrana activace hlavniho okna (Main Window).
 *
 * @since  0.1
 **/
  void activeMainWindow(void);


/**
 * Tento signal je vyslan pokud bylo vybrano zobrazeni promennych;
 * This signal is emited when showing values was selected.
 *
 * @since  0.1
 **/
  void printUPSVars(void);

/**
 * Tento signal je vyslan pokud bylo vybrano provedeni okamziteho prikazu;
 * This signal is emited when running instant command was selected.
 *
 * @since  0.1
 **/
  void instComms(void);

/**
 * Tento signal je vyslan pokud bylo vybrana zmena RW promennych.
 * This signal is emited when change of RW values was selected.
 *
 * @since  0.1
 **/
  void RWVars(void);

/**
 * Tento signal je vyslan pokud bylo vybrana zmena RW promennych.
 * This signal is emited when refresh connection was selected
 *
 * @since  0.1
 **/
  void makeRefreshConnection(void);


/**
 * Tento signal je vyslan pokud je vybrana nova UPS;
 * This signal is emited when new UPS is selected
 *
 * @since  0.1
 **/
  void UPSChanged(QString name);


protected:
/**
 * @internal
 *
 * Tests event and calls menu and main window, when lefy or right mouse button is pressed
 * @since 0.1
 **/
//  bool event( QEvent * );


  public Q_SLOTS:
/**
 * @internal
 *
 * @since  0.1
 **/
  virtual void activate(const QPoint &pos);



  private Q_SLOTS:

/**
 * @internal
 * Vola proceduru KAboutApplication, zobrazi informace o teto aplikaci a KDE.
 * Calls procedure KAboutApplication, shows informations about this application and KDE.
 *
 * @since  0.1
 **/
  void slotShowMyAbout();

 
 void slotExitApplication (void);

/**
 * @internal
 * Vysle signal minimize.
 * Sends signal minimize.
 *
 * @since  0.1
 **/
  void slotMinimize (void);


/**
 * @internal
 * Vysle signal peferences.
 * Sends signal preferences.
 *
 * @since  0.1
 **/
  void slotPreferences (void);

/**
 * @internal
 * Vysle signal printUPSVars.
 * Sends signal printUPSVars.
 *
 * @since  0.1
 **/
  void slotPrintUPSVars(void);

/**
 * @internal
 * Vysle signal instComms.
 * Sends signal instComms.
 *
 * @since  0.1
 **/
  void slotInstComms(void);


/**
 * @internal
 * Vysle signal RWVars.
 * Sends signal RWVars.
 *
 * @since  0.1
 **/
  void slotRWVars(void);

/**
 * @internal
 * Vysle signal upsName
 * Sends signal upsName.
 *
 * @since  0.1
 **/
  void slotUpsName(QAction *);

/**
 * @internal
 * Vysle signal makeRefreshConnection
 * Sends signal makeRefreshConnection.
 *
 * @since  0.1
 **/
  void slotRefreshConnection(void);


  protected:
/**
 * @internal
 * Nastavi vse pro vykresleni zakladni ikony.
 * Tato ikona je kreslena dokud neni navazano spojeni
 * Sets all for painting of basic icon.
 * This icon is painted
 *
 * @since  0.3
 **/
  void initSysTrayImage(void);

  void setIconBackgroundLayer (void);

  void loadIcons(void);

  void loadImages(void);

/**
 * @internal
 * Sets tooltip.
 * Nastavi ToolTip.
 *
 * @since  0.2
 **/
    void toolTip (int runtime, int status, int batteryCharge, int upsLoad );


/**
 * @internal
 * Vytvori a hlavni menu panelu.
 * Makes a main menu of panel
 *
 * @since  0.1
 **/
    void createMainMenu(void);

/**
 * @internal
 * Vytvori a podmenu pro seznam UPS-ek.
 * Makes a  submenu for list ig UPSes.
 *
 * @since  0.1
 **/
    void createUpsMenu(void);

/**
 * @internal
 * Paints a numbers.
 * Vykresli cisla.
 * @param number je hodnota cisla.
 * @param x je leva souradnice cisla
 * @param y je horni souradnice cisla
 * @param p je ukazatel na painter
 *
 * @since  0.2
 **/
    void paintNumbers (int numbers, int x, int y, QPainter* p ); //x,y levy horni roh cisla 1

/**
 * @internal
 * Paints a number
 * Vykresli cislici.
 * @param number je hodnota cislice.
 * @param x je leva souradnice cislice
 * @param y je horni souradnice cislice
 * @param p je ukazatel na painter
 *
 * @since  0.1
 **/
    void paintNumber (int x, int y, int number, QPainter *p);

/**
 * @internal
 * Paints a char.
 * Vykresli znak.
 * @param x je leva souradnice cislice
 * @param y je horni souradnice cislice
 * @param p je ukazatel na painter
 * @param char je vykreslovany znak.
 *
 * @since  0.1
 **/
    void paintChar (int x, int y, char LCDChar, QPainter *p);


/**
 * @internal
 * Vykresli pomlcku.
 * @param x je leva souradnice pomlcky
 * @param y je horni souradnice pomlcky
 * @param p je ukazatel na painter
 *
 * @since  0.1
 **/
    void paintDash (int x, int y, QPainter *p);


/**
 * @internal
 * Paints runtime layer.
 * Vykresli vrstvu runtime.
 *
 * @since  0.1
 **/
    void paintRuntime (QPainter &paint, int runtime, int status);

/**
 * @internal
 * Paints  UPS Load for GI
 * Vykresli  UPS Load pro GI.
 *
 * @since  0.2
 **/
    void paintLoadGeneralKI (QPainter &paint, int load );

/**
 * @internal
 * Paints Battery Charge  for type general icon.
 * Vykresli Battery Charge pro obecny typ icony
 *
 * @since  0.1
 **/
    void paintChargeGeneralKI (QPainter &paint,int charge );


/**
 * @internal
 * Paints Battery Charge for type picture icon. This procedure is used for offline state only.
 * Vykresli Battery Charge  pro obrazkovy typ icony Procedure je pouzita jen ve stavu offline. 
 *
 * @since  0.1
 **/
    void paintChargePictureKI (QPainter &paint,int charge );




/**
 * @internal
 * Paints layer for UPS Status, only for type general
 * Vykresli vrstvu pro UPS Status.
 *
 * @since  0.2
 **/
    void paintStatus (QPainter &paint, int status );


//    void paintEvent (QPaintEvent *);

/**
 * @internal
 * Makes sysTray icon for ok state / online or offline variant/ 
 * @since  0.1
 **/
   void makeOkIcon(void);


/**
 * @internal
 * Tests if value od variables for makeing systray icon was changed 
 * @since  0.1
 **/
bool dataValueIsChanged(void);

/**
 * @internal
 * Paints green point into main layer, only for type picture.
 * Vykresli do hlavni vrstvy zeleny bod.
 * Ma vyznam jen pro typ picture.
 *
 * @since  0.3
 **/
    void paintErrorPoint ( QPainter& paint );

/**
 * @internal
 * Paints red point into main layer, only for type picture.
 * Vykresli do hlavni vrstvy cerveny bod.
 * Ma vyznam jen pro typ picture.
 *
 * @since  0.3
 **/
    void paintOKPoint ( QPainter& paint );

/**
 * @internal
 * Clean internal variables this class.
 * Vycisti interni promene teto tridy.
 *
 * @since  0.1
 **/
  void clearValues(void);


    KNutUpsData* m_upsRecords;
    KNutVarData::SettingDataDef* m_settingData;  // v teto strukture jsou ulozeny promene ktere nastavuji konfiguraci
    KNutVarData::upsInfoDef* m_activeUpsInfo; // values for selected ups

    QIcon m_errorIcon;
    QIcon m_connectionIcon;
    QIcon m_idleIcon;

    QImage m_upsPix;
    QImage m_battPix;

    QMenu* m_menu;
    QMenu* m_upsServerMenu;

    QImage *m_mainLayer;
    QImage *m_backgroundLayer;

    int m_runtimeValue;
    int m_upsLoadValue;
    int m_batteryChargeValue;
    int m_statusValue;
    int m_statusBatValue;
    int m_statusWarningValue;


    upsConnection m_dataOk; // selects one from four type of icons's view //idle, connecting, connected, error
    upsConnection m_dataOkOld;

    int m_typeOfIcon;
    
    unsigned int m_toolTipFlags;

    QBrush m_backgroundBrush;
    QBrush m_backgroundGlobalBrush;
    QBrush m_textBrush;
    QColor m_textColor;

};

#endif
