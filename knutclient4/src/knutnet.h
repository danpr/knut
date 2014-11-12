/***************************************************************************
                          knutnet.h  -  description
                             -------------------
    begin                : Ne led 12 2003
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

#ifndef KNUTNET_H
#define KNUTNET_H

#include <KApplication>

#include <QtCore/QObject>
#include <QtCore/QMutex>
#include <QtNetwork/QAbstractSocket>

#include <QtCore/QVector>
#include <QtCore/QThread>

class QTcpSocket;
class QString;
class QTimer;

struct upsVarDef {
  QString upsVarName;
  bool upsVarActive;     // value is active, getUpsValue reads its value
  bool upsVarType;       // typ true(1) RO -  false (0) RW (muzeme menit)
  bool upsValueType;     // typ true(1) char  - false (0) enum (vyctova)
                           // ma vyznam jen pro RW promenne
  int  upsVarMax;        // udave max delku string promenne nebo pocet moznosti pro enum promennou
                           // ma vyznam jen pro RW promenne
  QString upsValue;     // hodnota promene
  QString upsDescription;  // informace o promene
  QVector<QString>* upsEnumValues;
  };


struct upsICommDef {
  QString upsCommName;
  QString upsDescription;
  };



/**
  *@author Daniel Prynych
  */

/**
 * This class reads data from serveru NUT.
 * Since version 0.5 is written and API is changet.
 * Since 0.6.1 supports protocol client-server and variables for upsd >= nut 1.3
 * Since 0.6.1 doesn't ups name like ups_name@host_address
 * Since 0.7 doesn't support UDP
 * Since 0.8 support KDE4 only not KDE3
 * Tato trida zajistuje cteni dat ze serveru NUT.
 * Od verze 0.5 je uplne prepsana a je zmeneno i API.
 * Od verze 0.6.1 proporuje take client-server protokol a promenne pro upsd >= nut 1.3 
 * Od verze 0.6.1 nepodporuje jmeno ups ve tvaru jmeno_ups@adresa_pc
 * Od verze 0.7 neni podporovano UDP
 * Od verze 0.8 podporuje jen KDE4 ne KDE3
 *
 * @author Daniel Prynych
 * @short Communication with UPS - Komunikace s UPS
 * @version 0.8
*/
class KNutNet : public QObject  {
   Q_OBJECT
public: 

  enum NetState {Idle, UnConnected = Idle, HostLookup, Connecting, Connected, Closing };
//  enum NetState {Idle, UnConnected = Idle, HostLookup, Connecting, Connected, ConnectError,Closing };
  // idle is same like UnConnected
  // neni pripojeno, vyhledani prijemce, pripojuji, pripojeno, pripojuji

  enum nutVarVersion {versionVarUnknown = 0, versionVarOne = 1 ,versionVarTwo = 2};


  enum typeOfVar {AllVars, ROVars, RWVars };

  enum NetError {
    NoError,
    LowMem,        // no memmory - nebyla pridelena pamet
    NullAddress,   // no address of host nebyla zadana adresa pocitace
    NoSuchHost,    // this host noexist  neexistuje takovy pocitac
    CantConnect,   // I cannot connect to server Nemohu se spojit se serverem
    SocketReadErr,     // I cannot read data from socket - Nelze cist data ze soketu
    UnknownSocketError = SocketReadErr,
    SocketTimeout = SocketReadErr,
    SocketResourceError = SocketReadErr, // too many sockets
    SocketAccessError = SocketReadErr, //application lacked the required privileges
    ConnectErr,    // connect cannot is made - Nejde provest connect
    NutProtocolVarErr, //  Nelze zjistit verze protokolu
    NoUpsHere,  // ni UPS on this address na teto adrese neni zadna UPS
    ConnectionRefused, // server refused connection
    ConnetionClosedByServer, // connection was closed by server upsd // by the second side
    NetworkError,
 
    SendErr,       // doesn't send date - Nelze odeslat data
    RecvErr,       // doesn't receive data - Nelze prijmout data 
    NoData,        // no data was received, empty string was received - Nebyla prijmuta zadna data, byl prijmut prazdny retezec
    UnknownAnswer, // Server answer no right - Server neodpovedel spravne
    UnknownFormat, // Data hasn't got right structure -  Data nemaji spravnou strukturu
    UnknownFormatVer, //Data returned after command VER don't have right structure -  Data vracena po prikazu VER nemaji spravnou strukturu
    NoServerData,  // doesn't read data from server (Server isn't answering)  Nelze precist data ze serveru (Sever neodpovida)
    NoUpsnetOk,    // UpsNetOK is setting on faLse - je nastaven UpsNetOk na false
    UnknownErr,    // unknown error unknown text after ERR -  neznama chyba - neznamy text za ERR
    CommNotFind,   // command hasn't been found  in list of module's commands - prikaz nebyl nalezen v seznamu prikazu modulu
    VarNotFind,    // variable hasn't been found in list of module's variables promena nebyla nalezena v seznamu promenych modulu
    EnumValueNotFind, //value of enumereted variable hasn't been found hodnota vyctove promene nebyla nalezena

// tyto zpravy vraci server
// this mesages is returned server upsd
    AccessDenied,        // access denied - pristup odmitnut
    PasswordRequired,    // password is required -  pro akci je nutno zaslat heslo
    PasswordIncorrect,   // password is incorrect nespravane heslo
    UnknownUps,          // this ups isn't occured on address - tato ups neni na zadane adrese
    AlreadyLoggedIn,     // prihlaseni LOGIN jiz bylo provedeno
    AlreadySetPassword,  // password has been already entered  - heslo jiz bylo zadano
    AlreadySetUsername,  // username has been already entered  - uzivatelske jmeno jiz bylo zadano
    DataStale,           // data from module isn't got - neni mozne ziskat z modulu nove data 
    MissingArgument,     // misses argument - chyby argument
    UnknownType,         // unknown type of variable -neznamy typ promene
    UnknownCommand,      // umknown command example: sends maybe "UPERNAME admin"  - nezmamy  prikaz pr: poslem treba "UPERNAME admin"

    //tyto chyby se vztahuji k driveru pro danny typ UPS-ky
    // this mistake is refered for   selected driver of UPS
    DriverNotConnected,  // driver isn't connected -  driver neni pripojen
    NoResponse,          // driver pro ups neodpovedel ve vymezenem case
    UnknownReply,        // unknown reply - neznama odpoved
    NotImplemented,      //
    CommandFailed,       // failed command - chybny prikaz
    VarUnknown,          // values isn't selected by driver - promena neni driverem podporovana
    VarNotSupported,     // cannot get data for this values - neni mozno zislat data pro tuto promenou
    InvalidValue,        // unknown value - nespravna hodnota
    UnknownInstCmd,      // unknown instant command / neznamy okamzity prikaz
    CmdNotSupported,     // this instant command isn't supported - tento okamzity prikaz neni podporovan
    NotConnection,      // operace nelze provest protoze neni provedeno pripojeni
    // this not error 
    NoListVars};        //upsd doesn't return list of variables, variable for STATUS has got value WAIT

  enum typeOfStatus {
    NS = 0,     // - NONE status of UPS wasn't read
    OFF = 1,    // - UPS is off (not supplying power)
    OL = 2,     // - UPS is online (supplying power from the line/mains)
    OB = 4,     // - UPS is on battery
    LB = 8,     // - UPS battery is low (with OB = shutdown situation)
    CAL = 16,   // - UPS is performing calibration
    TRIM = 32,  // - UPS is trimming incoming voltage (APC "SmartTrim")
    BOOST = 64, // - UPS is boosting incoming voltage (APC "SmartBoost")
    OVER = 128, // - UPS is overloaded
    RB = 256,   // - UPS battery needs to be replaced
    WAIT = 512,   // - UPS as for waiting
    CHRG = 1024   // - UPS battery is charged
    };

  enum keyWord {
    NONE,
    OK,
    ERR,
    BEGIN,
    END,
    VAR,
    TYPE,
    DESC,
    UPSDESC,
    CMDDESC,
    UPS,
    LIST,
    RW,
    CMD,
    ENUM,
    STARTTLS,
    BEGIN_LIST_VAR,
    BEGIN_LIST_RW,
    BEGIN_LIST_CMD,
    BEGIN_LIST_UPS,
    BEGIN_LIST_ENUM,
    END_LIST_VAR,
    END_LIST_RW,
    END_LIST_CMD,
    END_LIST_UPS,
    END_LIST_ENUM
    };

  enum flagsVar {
    RW_FLAG = 1,
    ENUM_FLAG = 2,
    STRING_FLAG = 4
    };


/**
 * Constructor sets basic variables and structures only, but doesn't read variables from server NUT
 * Konstruktor pouze nastavi zakladni promene a struktury, ale necte promene se serveru NUT.
 * @param upsName in name of UPS 
 * @param upsAddress Is address of UPS (adresa pocitace na ktery je UPS pripojena)
 * netAddress can is entered like ip address or domain name
 * example: localhost karel.domain,cz 194.212.25.55.
 * @param tPort Is name if port, for NUT server (NUT is received on this port), default is 3493.
 * @param delay is delay in msec. System waits  before one tries to make connection again. When test of connection fails,
 * @param countRepeat is maximal number of repeating for make connection. 
 *
 * Since 0.7.5 version protocol UDP is not supported
 * @param upsAddress Je adresa ups-ky (adresa pocitace na ktery je UPS pripojena)
 * netAddress muze byt zadan jako ip adresa nebo domenove jmeno pocitace
 * @param upsName je jmeno UPSky 
 * pr.: localhost  karel.domena.cz 194.212.25.55.
 * @param tPort je cislo portu na kterem nasloucha NUT server, default je 3493.
 * @param delay je zpozdeni v msec, System ceka nez znovu pokusi ukutecnit spojeni. Kdyz pokus o spojeni selhal.
 * @param countRepeat je maximalni pocet opakovani spojeni pro navazani spojeni. 
 * Od verze 0.7 neni podporovano UDP
 *
 * @since  0.7.5
 **/
  KNutNet (const QString upsAddress, const QString upsName="", const unsigned short port=3493, unsigned int countRepeat =0, const unsigned int delay = 200);

/**
 * @since  0.5
 **/
  ~KNutNet();



/** 
 * Zavre spojeni a nastavi hodnoty pro novou UPSku
 * Closes conneting and sets values for new UPS
 *
 * @since  0.1
 **/
 void newUPS (const QString upsAddress, const QString upsName, const unsigned short port=3493);


/**
 * Provadi spojeni
 * Makes conneting
 *
 * @since  0.1
 **/
  void open (void);

/** 
 * Zavre spojeni
 * Closes conneting
 *
 * @since  0.1
 **/
 void close (void);


/**
 * Returns kod of error, which is setted after last colling of same function this calls.
 * Error NotConnected isn't returned (not for function getError, getState).
 * It means then error NotConnected isn't remembered.
 * Vraci kod chyby, ktera nastala pri poslednim volani kterekoliv funkce
 * teto tridy (s vyjimkou getError, getState ),
 * chyba NotConnected neni vracena;
 * To znamena ze chyba NotConnected neni zapamatovana.
 * 
 *
 * @since  0.5
 **/
  NetError getError ( void ) { return m_errorState; };


/**
 * Vraci stav pripojeni.
 * Returns state of connection.
 *
 * @since  0.5
 **/
  NetState getState ( void ) { return m_state; };


/**
 * Returns state of description, if descriptions is loaded.
 * Vraci stav popisu, zda je nahran;
 *
 * @since  0.5
 **/
  bool isDescription ( void );


/**
 * Vraci zda doslo k prepnuti z UDP do TCP;
 * Ma vyznam je pro verzi protokolu 2
 *
 * @since  0.1
 **/
bool switchedToTCP ( void );


/**
 * Vraci cislo protokolu pro komunikaci s NUT
 * Returns nunber of protocol for comunications with NUT
 * Od verze NUTu 1.3 je zmenen protokol
 * Since version 1.3 protocol is changed
 *
 * @since  0.1
 **/
  int getNutProtocol ( void );

/**
 * Return number of NUT's version
 * Vraci cislo verze nazvu NUT-u
 * Since version if nut 1.3 are changed names of values
 * Od verze NUTu 1.3 jsou zmeneny nazvy promenych
 *
 * @since  0.1
 **/
  nutVarVersion getNutVariables ( void );


/**
 * Nacte vsechny promene UPS-ky a nastavi jejich typ (znakove/char, vyctove/enum a status)
 * Nastavi priznak aktivity jen pro status.
 * Funkce vraci kod chyby nebo 0 pokud k chybe nedojde;
 * Vzdy jsou nacteny i hodnoty promenych.
 *
 * @since  0.6.1
 **/
  NetError getUpsVars ( void);  


/**
 * Nacte hodnoty vsech promenych UPS-ky pokud je nastaven jejich priznak aktivity,
 * nebo je nastaven allVars na true.
 * Pozor po provedeni getUpsVars je priznak aktivity nastaven jen pro status.
 *
 * @param allVars Urcuje zda se prikaz tyka vsech promenych, nebo jen tech ktere
 * maji nastaven priznak aktivity na TRUE.
 *
 * @since  0.6.1
 **/
  NetError getUpsValues (const bool allVars=true );


/**
 * Nacte popisy pro kazdou promennou
 * Read descriptions for every variable
 *
 * @param always Urcuje zda se nacteny provede vzdy nebo jen kdy popis jeste
 * nebyl nacten.
 *
 * @since  0.6
 **/
  int getDescription (bool always = false);


/**
 * Provede okamzity prikaz.
 * Vraci 0 pokud provedeni skoncilo v poradku jinak kod chyby.
 *
 * @param command Udava nazev prikazu.
 * @param userName Udava uzivatelske jmeno.
 * @param password Udava heslo.
 * @param onlyCommand udava zda ma byt poslan pouze prikaz, nebo i nastaveno uzivatelske jmeno a heslo.
 *        POZOR!! uzivatelske jmeno nejde zmenit bez odpojeni a znovu pripojeni.
 *
 * @since  0.7
 **/
  int instantCommand (const QString command, const QString userName, const QString password, const bool onlyCommand = false);


/**
 * Nastavi RW promenou.
 * Vraci 0 pokud provedeni skoncilo v poradku jinak kod chyby;
 *
 * @param variable Udava nazev promenne.
 * @param value Udava hodnotu.
 * @param userName Udava uzivatelske jmeno.
 * @param password Udava heslo.
 * @param onlyVariable udava zda ma byt poslana pouze zadost na zmenu promenne, nebo i nastaveno uzivatelske jmeno a heslo.
 *        POZOR!! uzivatelske jmeno nejde zmenit bez odpojeni a znovu pripojeni.
 *
 * @since  0.7
 **/
  int setVariable (const QString variable, const QString value, const QString userName, const QString password, const bool onlyVariable = false);



/**
 * Returns number of UPS's variables. On error -1 is returned.
 * Vrati pocet promenych UPS-ky. Pri chybe vraci -1.
 *
 * @param typVar Is type of variable, its number is found out.
 * @param typVar Je typ promene pro ktere zjistujeme pocet
 * muze se jednat o RO promenne RW promene nebo o vsechny promenne.
 *
 * @since  0.6
 **/
  int readNumberVars (typeOfVar typVar = AllVars);


/**
 * Returns number of UPS's commands. On erroe -1 is returned.
 * Vrati pocet prikazu UPS-ky. Pri chybe vraci -1.
 *
 * @since  0.6
 **/
  int readNumberComms (void);



/**
 * Nastavi strukturu udavajici vsechny hodnoty pro prikaz.
 * Pri chybe vrati kod chyby
 *
 * @param seqNumber Udava cislo promene.
 * @param upsIComm Je struktura upsICommDef.
 *
 * @since  0.6
 **/
  int readIComm (const int seqNumber, upsICommDef& upsIComm);


/**
 * Nastavi strukturu obsahujici vsechny udaje promene.
 * Pri chybe vrati kod chyby.
 *
 * @param name Udava jmeno promene.
 *
 * @since  0.6
 **/
  int readVars (const QString name, struct upsVarDef& allVar);


/**
 * Nastavi strukturu obsahujici vsechny udaje promene.
 * Pri chybe vrati kod chyby.
 *
 * @param seqNumber Udava poradove cislo promene, ta zacina VZDY cislem 1.
 * @param typVar Udava typ promene /RO/RW/ALL.
 *
 * @since  0.6
 **/
  int readVars (const int seqNumber, struct upsVarDef& allVar, const typeOfVar typVar = AllVars);


/**
 * Vrati hodnotu promenne.
 * Pri chybe vrati 0l.
 *
 * @param name Udava jmeno promene.
 *
 * @since  0.6.1
 **/
  QString readStringVar (const QString name);

/**
 * Nacte jednu z hodnot vyctove promenne.
 * Vraci jednu z hodnot vyctove promenne udanou poradovym cislem hodnoty.
 * Pri chybe vrati 0l.
 *
 * @param name Udava nazev vyctove promenne.
 * @param valueNumber Udava poradove cislo hodnoty vyctove promenne.
 * @param enumValue Je naplnena hodnotou vyctove promenne.
 *
 * @since  0.6.1
 **/
  QString readEnumValueVar (const QString name, const int valueNumber);


/**
 * Vraci kod stavu (statusu) UPS-ky
 * kody je mozne scitat napt OB+LB = 12
 * OFF = 1, UPS-ka je vypnuta
 * OL = 2, UPS-ka bezi na sit
 * OB = 4 UPS-ka bezi na baterie
 * LB = 8 baterie je vybyta (pokud je zaroven OB dojde k shutdownu)
 * CAL = je spustena kalibrace UPS-ky
 * OVER =128 UPS-ka je pretizena
 * RB = 256 UPS-ka pozaduje vymenu baterie
 *
 * @since  0.5
 **/
int readStatus(void);


/**
 * Nastavi priznak aktivity.
 * Pri chybe vrati kod chyby.
 *
 * @param name Udava jmeno promene.
 *
 * @since  0.3
 **/
  int setActiveUpsVar ( const QString name, const bool active = true );

/**
 * Zrusi priznak aktivity.
 * Pri chybe vrati kod chyby.
 *
 * @param name Udava jmeno promene.
 *
 * @since  0.3
 **/
  int setUnactiveUpsVar ( const QString name, const bool unactive = true ) {return  setActiveUpsVar (name, !unactive);};


/**
 * Nastavi priznak aktivity pro VSECHNY promene.
 * Pri chybe vrati kod chyby.
 *
 * @since  0.6
 **/
  int setActiveAllUpsVars ( const bool active = true );


/**
 * Zrusi nastaveni priznaku aktivity pro VSECHNY promene.
 * Pri chybe vrati kod chyby.
 *
 * @since  0.6
 **/
  int setUnactiveAllUpsVars ( const bool unactive = true ) {return setActiveAllUpsVars (!unactive);};

/**
 * Testuje zda existuje promena danneho jmena.
 * POZOR ZMENA v NAVRATOVYCH HODNOTACH
 * Vraci 0  pokud existuje.
 * Pokud neexistuje nebo pri chybe vrati kod chyby.
 *
 * @param name Udava jmeno promene.
 *
 * @since  0.6
 **/
  int existName ( const QString name );

/*********   Consts ****************/


static const int initState = OL;

signals:
/**
 * Signal je vyslan, kdyz dojde k chybe.
 * Signal is emited, when error is occurred
 *
 * param@ error je cislo chyby.
 * param@ error je value of error.
 **/
void connectionError(int error);


/**
 * Signal je vyslan, kdyz druha starna uzavrela spojeni.
 * Signal is emited, when other side closed connection.
 *
 * @since  0.1
 **/
void connectionClosedByServer (void);

/**
 * Signal je vyslan, kdyz dojde k prvemu spojeni se serverem.
 * Signal is emited, when the first connection with server is realized
 *
 * @since  0.1
 **/
void firstConnection(void);

/**
 * Signal je vyslan, kdyz dojde k dalsimu spojeni se serverem, znovu pripojeni. 
 * Signal is emited, when next connection with server is realized, reconnect.
 *
 * @since  0.1
 **/
void nextConnection(void);

/**
 * Signal je vyslan, kdyz prevod jmena byl uspesny.
 * Signal is emited, when host lookup has succeeded
 *
 * @since  0.1
 **/
void hostFound(void);


/**
 * Signal je vyslan, kdyz spojeni je opet zkouseno. 
 * Signal is emited, when connection is tried again.
 *
 * @since  0.1
 **/
void tryRepeatFirstConnection( int );


/**
 * Signal je vyslan, kdyz spojeni je zkouseno poprve. 
 * Signal is emited, when connection is tried at first.
 *
 * @since  0.1
 **/
void tryFirstConnection( int);



void getVarDesc (int count, int item);

/*********   Private part    **********/

  private:


class KNutSleep : public QThread {
  public:
  static void msleep ( int delay) { QThread:: msleep (delay); };
  };

// internal funkcion


/**
 * Cleans input buffer
 * Vycisti vstupni buffer
 *
 * @since  0.1
 **/
void cleanConnecting (void);


/**
 * Prevede textovy nazev klice na jeho numerickou hodnotu
 * Jen pro protokol verze 2.
 * Funkce vraci kod klice;
 * param@ line textovy nazev klice.
 *
 * @since  0.1
 **/
int setKey (const QString line);

QString parseValue (const QString line);

/**
 * Parsuje typ promenne (RW ENUM STRING).
 * Jen pro protokol verze 2.
 * Funkce vraci soucet kodu typu promenych;
 * param@ line retezec ktery se parsuje.
 * param@ lenString vraci maximalni delka promenne typu STRING.
 *
 * @since  0.1.1
 **/
int parseTypeValue (QString line, int& lenString );



/**
 * Parsuje navratovou hodnotu z upsd (protokol klient-server).
 * Jen pro protokol verze 2.
 * Funkce vraci kod prikazu (UPS, VAR BEGIN_LIST_VAR );
 *
 * @since  0.1
 **/
int parseLine(QString& line, QString& upsName ,QString& varName, QString& value, int& typeValue, int& lenString);



/**
 * Nacte popisy pro kazdou promennou
 * Jen pro protokol verze 1.
 *
 * @param always Urcuje zda se nacteny provede vzdy nebo jen kdy popis jeste
 * nebyl nacten.
 *
 * @since  0.1
 **/
  int getDescription1 (bool always = false);

/**
 * Nacte popisy pro kazdou promennou
 * Jen pro protokol verze 2.
 *
 * @param always Urcuje zda se nacteny provede vzdy nebo jen kdy popis jeste
 * nebyl nacten.
 *
 * @since  0.1
 **/
  int getDescription2 (bool always = false);



/**
 * internal
 * Nacte nazev prvni UPS-ky na danne adrese.
 * Jen pro protokol verze 2
 * Funkce vraci kod chyby nebo 0 pokud k chybe nedojde;
 *
 * @since  0.1
 **/
NetError getFirstUpsName (QString& firstUpsName);


/**
 * internal
 * Nacte vsechny promene UPS-ky a nastavi jejich typ (znakove/char, vyctove/enum a status)
 * nastavi priznak aktivity jen pro status.
 * Jen pro protokol verze 2
 * Funkce vraci kod chyby nebo 0 pokud k chybe nedojde;
 *
 * @since  0.1
 **/
  NetError getUpsVars1 ( void);



/**
 * internal
 * Nacte vsechny promene UPS-ky a nastavi jejich typ (znakove/char, vyctove/enum a status)
 * nastavi priznak aktivity jen pro status.
 * Jen pro protokol verze 2
 * Funkce vraci kod chyby nebo 0 pokud k chybe nedojde;
 *
 * @since  0.2
 **/
  NetError getUpsVars2 ( void);



/**
 * Nacte hodnoty vsech promenych UPS-ky pokud je nastaven jejich priznak aktivity,
 * nebo je nastaven allVars na true.
 *
 * @param allVars Urcuje zda se prikaz tyka vsech promenych, nebo jen tech ktere
 * maji nastaven priznak aktivity na TRUE.
 * Jen pro protokol verze 1
 *
 * @since  0.6
 **/
  NetError getUpsValues1 (const bool allVars );

/**
 * Nacte hodnoty vsech promenych UPS-ky pokud je nastaven jejich priznak aktivity,
 * nebo je nastaven allVars na true.
 *
 * @param allVars Urcuje zda se prikaz tyka vsech promenych, nebo jen tech ktere
 * maji nastaven priznak aktivity na TRUE.
 * Jen pro protokol verze 2
 *
 * @since  0.6
 **/
  NetError getUpsValues2 (const bool allVars );


/**
 * @internal
 * Zjisti verzi upsd
 *
 * @param countRepeat maximalni pocet pokusu (ne opakovani !!)
 * @param delay zpozdeni, cas mezi opakovani
 */
  int version (int countRepeat, const int delay);


/**
 * @internal
 * Nenastavuje promenou error.
 * Doesn't set variable error
 */
  NetError getUpsData (const QByteArray sbuffer, QString& rbuffer, const QByteArray endString = (QByteArray)"");

/**
 * @internal
 **/
    void setRWVars (const QString varName, const bool valueType, const int varMax, QVector<QString>* enumValues);

/**
 * @internal
 **/
void upsSetType (struct upsVarDef& upsVar, const QString name, const QString value = "");

/**
 * @internal
 **/
  NetError upsTranslateError (const QString string);


/**
 * @internal
 **/
  void deleteVars (void);

/**
 * @internal
 **/
 NetError upsOldTranslateError (const QString string);


/**
 * @internal
 * Pro qt mensi nez 3.0.0 nemuze byt value const
 */
 void genStatusFlags (QString value);

/**
 * @internal
 * Nenastavuje promenou error.
 */
 NetError sendComm (const QString command, const QString arg1, const QString arg2, const bool useUpsName=false);



/**
 * @internal
 **/
  NetError newDescription (const QByteArray  inBuffer, QString& upsDescription);

private slots:
  void slotConnectionError(QAbstractSocket::SocketError socketError);

  void slotConnected(void);

  void slotTimerReconnect(void);

  void slotHostFound(void);

//  void slotConnectionClosed(void);

  void slotDisconnected (void);

private:

  /******* CONST ********/

  static const qint64 rBufferLen = 1024;

  QTcpSocket *m_commandSocket;
  QTimer *m_unConnectTimer;


  /****** VARS *************/
  QString m_upsAddress;
  QString m_upsName;

  unsigned int m_countRepeat;
  unsigned short m_port;
  unsigned int m_delay;

  NetState m_state;
  NetError m_errorState;
  bool m_firstConnect;
  unsigned int m_numberConnection;

  int m_nutProtocol;  // 1 for NUT <1.3,  2 for NUT => 1.3
  nutVarVersion m_nutVariables; // 0 for unknown 1 for NUT <1.3,  2 for NUT => 1.3


  int m_numberVars; // number of all variables - pocet vsech promennych
  int m_numberRWVars;
  int m_numberIComms;

  int m_upsStatusVar;
  bool m_description;

  bool m_switchToTCP;

  QMutex m_getUpsDataMutex;

  QVector<upsVarDef> m_listVars;
  QVector<upsICommDef> m_listIComms;

/********* Pomocne promenne ***********/
  int m_posChar;

};

#endif

