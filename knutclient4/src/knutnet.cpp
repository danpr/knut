/***************************************************************************
                          knutnet.cpp  -  description
                             -------------------
    begin                : Ne led 12 2003
    copyright            : (C) 2003 by Daniel Prynych
    email                : Daniel at Prynych.cz
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "knutnet.h"

#include <QtCore/QString>
#include <QtCore/QTimer>

#include <QtCore/QByteArray>
#include <QtCore/QTextStream>
#include <QtNetwork/QTcpSocket>

//#include <iostream>




KNutNet::KNutNet (const QString upsAddress, const QString upsName, const unsigned short port, unsigned int countRepeat, const unsigned  int delay)
        : QObject(), m_upsAddress(upsAddress), m_upsName(upsName), m_countRepeat(countRepeat), m_port(port), m_delay(delay)  {


qDebug ("KNutNet::Constructor");

  m_state = Idle;  //idle is value
  m_numberVars = 0;
  m_numberRWVars = 0;
  m_numberIComms = 0;
  m_upsStatusVar = 0;
  m_nutProtocol = 0;
  m_nutVariables = versionVarUnknown;
  m_switchToTCP = false;

  m_description = false; // popis neni natazen  - description is not loaded

  m_unConnectTimer = new QTimer( this );
  connect( m_unConnectTimer, SIGNAL(timeout()), this, SLOT(slotTimerReconnect()) );

  m_errorState = NoError;

  m_commandSocket = new QTcpSocket( this );
  connect( m_commandSocket, SIGNAL( hostFound()),this, SLOT( slotHostFound()));
  connect( m_commandSocket, SIGNAL( connected()),this,SLOT( slotConnected()));


  connect( m_commandSocket, SIGNAL( disconnected()),this,SLOT( slotDisconnected()));

  connect( m_commandSocket, SIGNAL( error(QAbstractSocket::SocketError)),this, SLOT( slotConnectionError(QAbstractSocket::SocketError)));
  }


KNutNet::~KNutNet(){

  // doesn't run slotDisconnected when class is ended
  disconnect( m_commandSocket, SIGNAL( disconnected()),this,SLOT( slotDisconnected()));

  close(); // close connetion when is existed
  m_state = UnConnected ;
//  deleteVars() is made in close();
  }


void KNutNet::slotDisconnected (void) {

  m_state = UnConnected ;
  m_unConnectTimer->stop();
//  emit connectionClosed();
  }


void KNutNet::close (void) {
qDebug("KNutNet::close %d",m_state);


  QByteArray  inBuffer;
  QString  outBuffer;

  m_unConnectTimer->stop();

  deleteVars();

  if ((m_state == Connected) || (m_state == Connecting)) {
    m_state = Closing;
    if (m_state == Connected) {
      inBuffer="LOGOUT\n";
      getUpsData (inBuffer,outBuffer);
      // getUpsDate makes "commandSocket->flush()"
      }
    
    if (m_commandSocket->bytesToWrite() > 0) KNutSleep::msleep(1000);
    m_commandSocket->abort(); // closes the socket
    m_state = UnConnected;
    }
  }


void KNutNet::open (void) {

  qDebug ("KNutNet::open %d", m_state);

  while (m_state == HostLookup ) {
    KNutSleep::msleep(100);
    }
  if ((m_state == Connected) || (m_state == Connecting)) close(); // close connetion when is existed
  else {
    while (m_state == Closing ) {
       KNutSleep::msleep(100);
      }
    }
  if (m_upsAddress != "") {
    if ( m_commandSocket->state() != QAbstractSocket::ConnectingState ) {
      m_firstConnect=true;
      m_numberConnection=0;
      m_state = HostLookup;
      emit tryFirstConnection(m_countRepeat); // how many times we can try to connect with ups server /upsd/

      m_commandSocket->connectToHost(m_upsAddress,m_port);
      }
    }
  }


void KNutNet::newUPS (const QString upsAddress, const QString upsName, const unsigned short port) {


qDebug("KNutNet::newUPS");
  if ((m_state == Connected) || (m_state == Connecting)) { 
    close();
    while (m_state == Closing ) {
      KNutSleep::msleep(100);
      }
    }

  m_upsAddress = upsAddress;
  m_upsName = upsName;
  m_port = port;

  m_state = Idle;
  m_numberVars = 0;
  m_numberRWVars = 0;
  m_numberIComms = 0;
  m_upsStatusVar = 0;
  m_nutProtocol = 0;
  m_nutVariables = versionVarUnknown;
  m_switchToTCP = false;

  m_errorState = NoError;
  //firstConnect=true;
  //numberConnection=0;
  }




//int KNutNet::getState ( void ){ return m_state; }

bool KNutNet::isDescription ( void ){ return m_description; }

int KNutNet::getNutProtocol ( void ) { return m_nutProtocol; }

KNutNet::nutVarVersion KNutNet::getNutVariables ( void ) {
 if ((m_nutVariables < 1) || (m_nutVariables > 2)) {
   return versionVarOne;
   }
 else {
   return (nutVarVersion)m_nutVariables;
   }
 }

bool KNutNet::switchedToTCP ( void ) { return m_switchToTCP; }


KNutNet::NetError KNutNet::getUpsVars ( void) {
qDebug("int KNutNet::getUpsVars");
  NetError returnValue;

  m_errorState=NoError;
  if (m_state == Connected) {

    // init vars and lists
    m_numberVars = 0;
    m_numberRWVars = 0;
    m_numberIComms = 0;
    deleteVars();
    int countWaitRevision = 5;
    do {
      switch (m_nutProtocol) {
        case 1:
          returnValue = getUpsVars1();
        break;
        case 2:
          returnValue = getUpsVars2();
        break;
        default:
        return NoError;
        }
      if ( readStatus() != KNutNet::WAIT) return returnValue;
      countWaitRevision--;
      if (countWaitRevision) KNutSleep::msleep(2);
      }
    while (countWaitRevision);
    return NoListVars; //getUpsVars can't read list variables
    }
  else {
    return NotConnection;
    }
  }


KNutNet::NetError KNutNet::getUpsValues (const bool allVars ) {// allVars = true vse; allVars = false jen activate
qDebug("KNutNet::getUpsValues");

  m_errorState=NoError; // vynulujeme chyby
  if (m_state == Connected) {
    // init vars and lists
    switch (m_nutProtocol) {
      case 1:
        return getUpsValues1(allVars);
      break;
      case 2:
        return getUpsValues2(allVars);
      break;
      default:
      return NoError;
      }
    }
  else {
    return NotConnection;
    }
  }  



int KNutNet::getDescription (bool always) {

  m_errorState=NoError; // vynulujeme chyby
  if (m_state == Connected) {
    switch (m_nutProtocol) {
      case 1:
        return getDescription1 (always);
      break;
      case 2:
        return getDescription2 (always);
      break;
      default:
        return 0;
      }
    }  
  else return NotConnection;
  }


int KNutNet::instantCommand (const QString command, const QString userName, const QString password, const bool onlyCommand) {
  m_errorState=NoError; // vynulujeme chyby

  if (m_state == Connected) {
    QString outBuffer;

    // funkce sendComm sama mastavi promenou error
    if (onlyCommand) {
      if (!(m_errorState = sendComm("INSTCMD", command, "",true))) {
//        netMutex=false;
        return 0;
        }
      else { 
//        netMutex=false;
        return m_errorState;
        }
      }
    else {
      if (!(m_errorState = sendComm ("USERNAME", userName, ""))) {
        if (!(m_errorState = sendComm("PASSWORD", password, ""))) {
          if (!(m_errorState = sendComm("INSTCMD", command, "",true))) {
//            netMutex=false;
            return 0;
            }
          else { 
            return m_errorState; // spatny prikaz wrong command
            }
          } //username
        else { 
          return m_errorState;  //spatne heslo - wrong password
	  }
        }
      else {
	return m_errorState; // spatne jmeno wrong name
	}
      }
    }
  else return NotConnection;
  }


int KNutNet::setVariable (const QString variable, const QString value, const QString userName, const QString password, const bool onlyVariable) {
  m_errorState=NoError; // vynulujeme chyby
  if (m_state == Connected) {
    QString outBuffer;
    QString setCommand;

    switch (m_nutProtocol) {
      case 1: setCommand = "SET";
         break;
      case 2: setCommand = "SET VAR";
      }
    if (onlyVariable) {
      if (!(m_errorState = sendComm(setCommand, variable, value, true))) {
        return 0;
        }
      else { 
        return m_errorState;
        }
      }
    else {
      if (!(m_errorState = sendComm ("USERNAME", userName, ""))) {
        if (!(m_errorState = sendComm("PASSWORD", password, ""))) {
          if (!(m_errorState = sendComm(setCommand, variable, value, true))) {
            //netMutex=false;
            return 0;
            }
          else {
	    return m_errorState;
	    }
          } //username
        else {
	  return m_errorState;
	  }
        }
      else {
	 return m_errorState;
	 }
      }
    }
  else return NotConnection;
  }


int KNutNet::readNumberVars (typeOfVar typVar) {
//  Vraci pocet promenych
  m_errorState=NoError;
  if (m_state == Connected) {
    switch (typVar) {
      case AllVars:
        return m_numberVars;
        break;
      case ROVars:
        return m_numberVars - m_numberRWVars;
        break;
      case RWVars:
        return m_numberRWVars;
        break;
      default:
        return -1;
      }
    }
  else return -1;
  }

int KNutNet::readNumberComms (void) {
//  Vraci pocet prikazu
  m_errorState=NoError;
  if (m_state == Connected) return m_numberIComms;
  else return -1;
  }


int KNutNet::readIComm (const int seqNumber, upsICommDef& upsIComm) {
  m_errorState =NoError;
  if (m_state == Connected) {
//    if ((seqNumber < 1) || ((unsigned int)seqNumber > m_listIComms.size())) {
    if ((seqNumber < 1) || (seqNumber > m_listIComms.size())) {
      m_errorState=CommNotFind;
      return m_errorState;
      }
    upsIComm = m_listIComms[seqNumber-1];
    return 0;
    }
  else return NotConnection;
  }


int KNutNet::readVars (const QString name, struct upsVarDef& allVar) {
  m_errorState =NoError;
  if (m_state == Connected) {
      QVector<upsVarDef>::const_iterator it;
    for (it = m_listVars.begin(); it != m_listVars.end(); it++) {
       if ((*it).upsVarName == name) {
         allVar = (*it);
         return 0;
         }
       }
     m_errorState=VarNotFind;
     return m_errorState;
    }
  else return NotConnection;
  }

  int KNutNet::readVars (const int seqNumber, struct upsVarDef& allVar, const typeOfVar typVar) {

  m_errorState =NoError;
  if (m_state == Connected) {
    int n = 1;
    QVector<upsVarDef>::const_iterator it;

    for (it = m_listVars.begin(); it != m_listVars.end(); it++) {
      if ((typVar== AllVars) || ((typVar==ROVars) && ((*it).upsVarType)) ||
        ((typVar==RWVars) && (!(*it).upsVarType))) {
        if (n == seqNumber) {
          allVar = (*it);
          return 0;
          }
        n++;
        }
      }
    m_errorState=VarNotFind;
    return m_errorState;
    }
  else return NotConnection;
  }


QString KNutNet::readStringVar (const QString name) {

  m_errorState =NoError;
  if (m_state == Connected) {
      QVector<upsVarDef>::const_iterator it;
    for (it = m_listVars.begin(); it != m_listVars.end(); it++) {
      if ((*it).upsVarName == name) return (*it).upsValue;
      }
    m_errorState = VarNotFind;
    return 0l;  
    }
  else return 0l;
}


QString KNutNet::readEnumValueVar (const QString name, const int valueNumber) {

  m_errorState =NoError;
  if (m_state == Connected) {
        QVector<upsVarDef>::const_iterator it;

    for (it = m_listVars.begin(); it != m_listVars.end(); it++) {
      if ((*it).upsVarName == name) {
        if ((valueNumber < 1) || (valueNumber > (*it).upsVarMax)) {
          m_errorState=EnumValueNotFind;
          return 0l;
          }
        // vratime hodnotu
        return (*(*it).upsEnumValues)[valueNumber-1];
        }
      }
    m_errorState = VarNotFind;
    return 0;
    }
  else return 0;
  }


int KNutNet::readStatus(void) {

qDebug ("KNutNet::readStatus %d",m_upsStatusVar);

  m_errorState=NoError;
  return m_upsStatusVar;
  }



int KNutNet::existName ( const QString name ) {

  m_errorState =NoError;
  if (m_state == Connected) {
        QVector<upsVarDef>::const_iterator it;
    for (it = m_listVars.begin(); it != m_listVars.end(); it++) {
      if ((*it).upsVarName == name) return 0;
      }
    m_errorState = VarNotFind;
    return m_errorState;
    }
  else return NotConnection;
  }

/*************************************************************************/  
/*                                                                       */
/*                           PRIVATE FUNCTIONS                           */
/*                                                                       */
/*************************************************************************/


int KNutNet::version (int countRepeat, const int delay) {
qDebug("KNutNet::version");

  QByteArray  inBuffer;
  QString  outBuffer;

  m_errorState =NoError;
  do {
    inBuffer="VER\n";

    if ((m_errorState=getUpsData (inBuffer,outBuffer)) == NoError) {
      outBuffer=outBuffer.trimmed();

      if (outBuffer.length() > 0) {
        outBuffer = outBuffer.mid(outBuffer.indexOf("upsd")+4);
        int n = outBuffer.indexOf(".");
        if (n > 0) {
          QString firstNumber= outBuffer.left(n);
          QString secondNumber= outBuffer.mid(n+1);
          int n = secondNumber.indexOf(".");
          if (n > 0) {
            bool ok;
            secondNumber = secondNumber.left(n);
            int secondInt = secondNumber.toInt(&ok);
            if (!ok) {m_errorState = UnknownFormatVer; return m_errorState;}
            int firstInt = firstNumber.toInt(&ok);
            if (!ok) {m_errorState = UnknownFormatVer; return m_errorState;}
            if ((firstInt == 0) || ((firstInt == 1)  && (secondInt < 3))) {

              m_nutProtocol =1;
              m_nutVariables =versionVarOne;
              }
            else {
              m_nutProtocol =2;
              m_nutVariables =versionVarTwo;
              }
            return 0;
            }
          else m_errorState = UnknownFormatVer; // n is <= 0 the second dot
          }
        else m_errorState = UnknownFormatVer; // n is <= 0 the first dot
        }
      m_errorState = UnknownAnswer; // i cannot specify number of protocol and number of variables
      return m_errorState;
      }
    qDebug("KNutNet::version after get error %d",countRepeat);
    countRepeat--;
    if (countRepeat) KNutSleep::msleep (delay);
    }
  while ((countRepeat));
  return m_errorState;
  }



KNutNet::NetError KNutNet::getFirstUpsName (QString& firstUpsName) {
  QByteArray  inBuffer;
  QString  outBuffer;
  QString  varOutBuffer;
  int lenString;

  m_errorState=NoError;
  inBuffer = "LIST UPS\n";
  if ((m_errorState=getUpsData (inBuffer,outBuffer,"END LIST UPS"))== NoError) {

    QString inLine, upsNameRet, varName, value;
    int key, typeValue;
    bool beginList = false;
    QTextStream varInputStream(&outBuffer);
    while (!(inLine = varInputStream.readLine()).isNull()) {
      key = parseLine (inLine, upsNameRet, varName, value, typeValue, lenString);
      switch (key) {
        case BEGIN_LIST_UPS: {
          beginList = true;;
          break;
          }
        case UPS:
         if (beginList ) {
           firstUpsName = upsNameRet;
           return NoError;
           }
        case END_LIST_UPS:
          firstUpsName = "";
            m_errorState=NoUpsHere;
            return m_errorState;
        case ERR:
          m_errorState = upsTranslateError(value);
          return m_errorState;
        default:
          m_errorState=UnknownAnswer;
          return m_errorState;
        }
      }// end while
    }
  else {
     m_errorState=NoData;
     return m_errorState; 
     }
  //for compiler only
  return NoError;
  }



int KNutNet::setKey (const QString line) {

  QString myLine=line.trimmed();
  if (myLine == "OK") return OK;
  if (myLine == "ERR") return ERR;
  if (myLine == "BEGIN") return BEGIN;
  if (myLine == "END") return END;
  if (myLine == "VAR") return VAR;
  if (myLine == "TYPE") return TYPE;
  if (myLine == "DESC") return DESC;
  if (myLine == "CMDDESC") return CMDDESC;
  if (myLine == "UPSDESC") return CMDDESC;
  if (myLine == "UPS") return UPS;
  if (myLine == "LIST") return LIST;
  if (myLine == "RW") return RW;
  if (myLine == "CMD") return CMD;
  if (myLine == "ENUM") return ENUM;
  if (myLine == "STARTTLS") return STARTTLS;
  return NONE;
  }

QString KNutNet::parseValue (const QString line) {

  if (line.isEmpty()) return "";
  if (line[0] !=  QChar('\"')) {
    if ((m_posChar=line.indexOf(' ')) == -1) return line;
    else return line.left(m_posChar+1);
    }
  else {
    int len = line.length();
    QString outString = "";
    for (int i=1; i < len; i++) {
      if ( line[i] == QChar('\\')) {
        if ((i+1) <len) {
          if ((line[i+1] == QChar('\"')) || (line[i+1] == QChar('\"'))) {
            outString +=line[i+1];
            i++;
            }
          else i +=2;  
          }
        outString +=line[i];
        continue;
        }
      if (line [i] == QChar('\"')) {
        return outString;
        }
      outString += line[i];    
      }
    return outString;
    }
  return "";
  }

int KNutNet::parseTypeValue (QString line, int& lenString ) {
  int ret=0;
  QString word;

  lenString = 0;
  if (line.isEmpty()) return 0;
  QTextStream inputStream(&line);
  while (!inputStream.atEnd()) {
    inputStream >> word;
    if (word == "RW") ret += RW_FLAG;
    if (word == "ENUM") ret += ENUM_FLAG;
    if (word.indexOf("STRING:") != -1) {
      word = word.mid(7);
      ret += STRING_FLAG;
      lenString = word.toInt();
      }
    }
  return ret;
  }


int KNutNet::parseLine(QString& line, QString& upsName ,QString& varName, QString& value, int& typeValue, int& lenString) {
  int posChar;
  QString word1, word2, word3;
  int key1, key2, key3;

  line = line.trimmed();

  upsName = "";
  varName = "";
  value = "";
  typeValue =0;

  if (line.isEmpty()) return NONE;

  if ((posChar=line.indexOf(' ')) == -1) {
    if (setKey(line) == OK) return OK;
    else return NONE;
    }
  else {
    key1 = setKey( line.left(posChar));
    line = line.mid(posChar+1);

    switch (key1) {

      case NONE:
        return key1;
      case OK:
        return key1;
      case ERR:
        value = parseValue(line);
        return key1;
      default:
        if ((posChar=line.indexOf(' ')) == -1) return NONE;
        word2 = line.left(posChar);
        key2 = setKey( word2);
        line = line.mid(posChar+1);

        switch (key1) {
          case BEGIN:
            if ((key2 == LIST) && (setKey(line) == UPS)) return BEGIN_LIST_UPS;
          break;
          case END:
            if ((key2 == LIST) && (setKey(line) == UPS)) return END_LIST_UPS;
          break;
          case UPS:
            upsName = word2;
            value = parseValue(line);
            return key1;
          case CMD:
            upsName = word2;
            varName = parseValue(line);
            return key1;
          }

        if ((posChar=line.indexOf(' ')) == -1) return NONE;

        word3 = line.left(posChar);
        key3 = setKey( word3);
        line = line.mid(posChar+1);

        switch (key1) {
          case VAR:
            upsName = word2;
            varName = word3;
            value = parseValue(line);
            return VAR;
          case TYPE:
            upsName = word2;
            varName = word3;
            typeValue = parseTypeValue(line,lenString);
            return TYPE;
          case ENUM:
            upsName = word2;
            varName = word3;
            value = parseValue(line);
            return ENUM;
          case DESC:
            upsName = word2;
            varName = word3;
            value = parseValue(line);
            return DESC;
          case CMDDESC:
            upsName = word2;
            varName = word3;
            value = parseValue(line);
            return CMDDESC;
          case BEGIN:
            if (key2 != LIST) return NONE;
            if (key3 == VAR) {
              upsName=line.trimmed();
              return BEGIN_LIST_VAR;
              }
            if (key3 == RW) {
              upsName=line.trimmed();
              return BEGIN_LIST_RW;
              }
            if (key3 == CMD) {
              upsName=line.trimmed();
              return BEGIN_LIST_CMD;
              }
            if (key3 == ENUM) {
              if ((posChar=line.indexOf(' ')) == -1) return NONE;
              upsName = line.left(posChar);
              line=line.mid(posChar+1);
              varName = line.trimmed();
              return BEGIN_LIST_ENUM;
              }
            return NONE;  
          case END:
            if (key2 != LIST) return NONE;
            if (key3 == VAR) {
              upsName=line.trimmed();
              return END_LIST_VAR;
              }
            if (key3 == RW) {
              upsName=line.trimmed();
              return END_LIST_RW;
              }
            if (key3 == CMD) {
              upsName=line.trimmed();
              return END_LIST_CMD;
              }
            if (key3 == ENUM) {
              if ((posChar=line.indexOf(' ')) == -1) return NONE;
              upsName = line.left(posChar);
              line=line.mid(posChar+1);
              varName = line.trimmed();
              return END_LIST_ENUM;
              }
            return NONE;
          default:
            return NONE;
          }
      }
    }
  }


KNutNet::NetError KNutNet::newDescription (const QByteArray  inBuffer, QString& upsDescription) {
  QString  outBuffer;

  if ((m_errorState=getUpsData (inBuffer,outBuffer))== NoError) {
    outBuffer=outBuffer.trimmed();
    QString word1, word2, word3;

    if ((m_posChar = outBuffer.indexOf(' ')) == -1)
      word1 = outBuffer;
    else {
      word1 = outBuffer.left(m_posChar);
      word2 = outBuffer.mid(m_posChar+1);
      }
    if (word1 == "DESC") {
      if ((m_posChar = word2.indexOf('\"')) == -1) word3="";
      else
        {
        word3 = word2.mid(m_posChar+1);
        if ((m_posChar = word3.indexOf('\"')) != -1) word3=word3.left(m_posChar);
        }
      if (!word3.isEmpty()) upsDescription=word3;
      }
    else {
      if (word1 == "ERR" ) return upsTranslateError(word2);
      else return UnknownAnswer;
      }
    }
  else return m_errorState;
  return NoError;
  }


void KNutNet::cleanConnecting (void) {
  qint64 rMaxLen; 
  QByteArray  recvBuffer; 
  qint64 lengthInputData;

  int n = 0;
  while (m_commandSocket->bytesAvailable() > 0) {

    if ( (m_commandSocket->bytesAvailable()+1) > rBufferLen) rMaxLen = rBufferLen;
    else rMaxLen=m_commandSocket->bytesAvailable()+1;

    recvBuffer.resize(rMaxLen);
    if ((lengthInputData = m_commandSocket->read(recvBuffer.data(),rMaxLen)) == -1) {
      break;
      }
     if (lengthInputData == 0) break;
     if (n > 20) break; else n++;
    }
  }


KNutNet::NetError KNutNet::getUpsData (const QByteArray sBuffer, QString& rBuffer, const QByteArray endString) {

  //rbufferLen a sbufferLen are maximal value of buffers
  QByteArray  recvBuffer;
  qint64 lengthInputData;
  qint64 dataAvailable;

  QMutexLocker getUpsDataLocker( &m_getUpsDataMutex );

  cleanConnecting();
  // first we clean input TCP buffer

  //cleans buffer 
  rBuffer=""; // clean main input buffer;

  // after write block, runs command flush for sending data to server inmediately
  if (m_commandSocket->write(sBuffer.data(),sBuffer.length()) < 0 ) return SendErr;
  m_commandSocket->flush();
//waiting for data
  if (!(m_commandSocket->waitForReadyRead(1500))) {
    if (m_errorState != NoError) {
      return m_errorState;
      }
    }

// gets data
  bool endRecv = false;
  do {
    dataAvailable=m_commandSocket->bytesAvailable();
    if (recvBuffer.size() < (dataAvailable+1) ) recvBuffer.resize(dataAvailable+1);
    if ((lengthInputData = m_commandSocket->read(recvBuffer.data(),dataAvailable)) == -1) return RecvErr;

//correct for X86_64  by Alexey Sidorov    
    recvBuffer[(int)lengthInputData]='\0';

    //Problem is that
    //for 64bit environment is Q_LONG defined like _int64 and
    //for other environments is Q_LONG defined like long      


    rBuffer.append(recvBuffer.data()); // adds reading data to main input buffer
    if (endString.isEmpty()) endRecv = false;
    else {
      if (recvBuffer.lastIndexOf(endString) == -1)  {
//doesn't find right end of input data
        if (m_commandSocket->bytesAvailable() == 0) {
          if (!(m_commandSocket->waitForReadyRead(1000)))  {
            if (m_errorState != NoError) {
            return m_errorState;
            }
          } 
        }
      if (m_commandSocket->bytesAvailable() > 0)  endRecv=true; // makes other cyckle of while
      else endRecv=false;
      }
//finds right end of input data
    else endRecv = false; // we have found what we want to find
    }    
  } while (endRecv);
  return NoError;   // all is OK
}



 void KNutNet::setRWVars (const QString varName, const bool valueType, const int varMax, QVector<QString>* enumValues) {
      QVector<upsVarDef>::iterator it;

   for (it = m_listVars.begin(); it != m_listVars.end(); it++) {
     if ((*it).upsVarName == varName) {
       (*it).upsVarMax = varMax;
       (*it).upsValueType=valueType;
       (*it).upsEnumValues=enumValues;
       (*it).upsVarType=false;
       break;
       }
    }
  }

void KNutNet::upsSetType (struct upsVarDef& upsVar, const QString name, const QString value) {

  if (((m_nutVariables = versionVarTwo) && (name == "STATUS")) || ((m_nutVariables == versionVarOne) && (name == "ups.status")))
    upsVar.upsVarActive=true;
  else upsVar.upsVarActive=false;

  upsVar.upsVarName = name;
  upsVar.upsVarType=true;
  upsVar.upsValueType=true;
  upsVar.upsVarMax=0;
  upsVar.upsValue=value;
  upsVar.upsDescription="";
  upsVar.upsEnumValues=NULL;
}


void KNutNet::deleteVars (void) {

      QVector<upsVarDef>::iterator it;
  for (it = m_listVars.begin(); it != m_listVars.end(); it++) {
    if ((*it).upsEnumValues != 0) {
      (*it).upsEnumValues->clear();
      delete (*it).upsEnumValues;
      }
    }
  m_listVars.clear();
  m_listIComms.clear();
  }

void KNutNet::genStatusFlags (QString value) {
qDebug("KNutNet::genStatusFlags");
  m_upsStatusVar = 0;
  QTextStream inputStream(&value);
  QString word;
  while (!inputStream.atEnd()) {
    inputStream >> word;
    if (word == "OFF") m_upsStatusVar +=OFF;
    if (word == "OL") m_upsStatusVar +=OL;
    if (word == "OB") m_upsStatusVar +=OB;
    if (word == "LB") m_upsStatusVar +=LB;
    if (word == "CAL") m_upsStatusVar +=CAL;
    if (word == "TRIM") m_upsStatusVar +=TRIM;
    if (word == "BOOST") m_upsStatusVar +=BOOST;
    if (word == "OVER") m_upsStatusVar +=OVER;
    if (word == "RB") m_upsStatusVar +=RB;
    if (word == "CHRG") m_upsStatusVar +=CHRG;
    if (word == "WAIT") m_upsStatusVar = WAIT;
    //OFF     -  1 ups je vypnuta
    //OL      -  2 ups je bezi na sit
    //OB      -  4 ups bezi na baterie
    //LB      -  8 baterie je vybita (pokud je zaroven OB dojde k shutdownu)
    //CAL     - 16 je spustena kalibrace
    //OVER    - 128 ups je pretizena
    //RB      - 256 ups pozaduje vymenu baterie
    //WAIT    - 512 ups zada o strpeni // UPS asks for waiting 
    //CHRG    - 1024 ups battery is charged
    }
  }


int KNutNet::getDescription1 (bool always) {

  if (always || (! m_description)) {
    QByteArray  inBuffer;
    QString  outBuffer;

     QVector<upsVarDef>::iterator itv;
     QVector<upsICommDef>::iterator itc;
     for (itv = m_listVars.begin(); itv != m_listVars.end(); itv++) {
      //nacteme promenou
      inBuffer = "VARDESC ";
      inBuffer += (*itv).upsVarName.toLocal8Bit() + ('\n');
      if ((m_errorState == newDescription (inBuffer, (*itv).upsDescription)) != NoError) {
        //netMutex.unlock();
        //netMutex=false;
        return m_errorState;
        }
      }
    for (itc = m_listIComms.begin(); itc != m_listIComms.end(); itc++) {
      //nacteme promenou
      inBuffer = "INSTCMDDESC ";
      inBuffer += (*itc).upsCommName.toLocal8Bit()+"\n";
      if ((m_errorState = newDescription (inBuffer, (*itc).upsDescription)) != NoError) {
        //netMutex.unlock();
        //netMutex=false;
        return m_errorState;
        }
      }
    //netMutex=false;
    m_description = true;
    }
  return 0;
  }



int KNutNet::getDescription2 (bool always) {

  if (always || (! m_description)) {
    QByteArray  inBuffer;
    QString  outBuffer;
    QString upsNameRet;
    QString varName;
    QString value;
    int typeValue;
    int lenString;

     QVector<upsVarDef>::iterator itv;
     QVector<upsICommDef>::iterator itc;

     for (itv = m_listVars.begin(); itv != m_listVars.end(); itv++) {
      //nacteme promenou
      inBuffer = "GET DESC ";
      inBuffer += m_upsName.toLocal8Bit() + " " + (*itv).upsVarName.toLocal8Bit() + "\n";

      if ((m_errorState=getUpsData (inBuffer,outBuffer)) == NoError) {
        outBuffer=outBuffer.trimmed();
        if ( !outBuffer.isEmpty() ) {
          int key = parseLine(outBuffer, upsNameRet, varName, value, typeValue, lenString);
          switch (key) {
            case DESC:
              (*itv).upsDescription = value;
               break;
            case ERR:
              m_errorState=upsTranslateError(value);
              return m_errorState;
            default:
              m_errorState=UnknownAnswer;
              return m_errorState;
            }
          }
        else {
          m_errorState = NoData;
          return m_errorState;
          }
        } // end for
      else {
        return m_errorState;
        }
      }
    for (itc = m_listIComms.begin(); itc != m_listIComms.end(); itc++) {
      //nacteme promenou
      inBuffer = "GET CMDDESC ";
      inBuffer += m_upsName.toLocal8Bit() + " " + (*itc).upsCommName.toLocal8Bit()+"\n";
      if ((m_errorState=getUpsData (inBuffer,outBuffer)) ==  NoError) {
        outBuffer=outBuffer.trimmed();
        if ( !outBuffer.isEmpty() ) {
          int key = parseLine(outBuffer, upsNameRet, varName, value, typeValue, lenString);
          switch (key) {
            case CMDDESC:
              (*itc).upsDescription = value;
               break;
            case ERR:
              m_errorState=upsTranslateError(value);
              return m_errorState;
            default:
              m_errorState=UnknownAnswer;
              return m_errorState;
            }
          }
        else {
          m_errorState = NoData;
          return m_errorState;
          }
        }
      else {
        return m_errorState;
        }
      }
    m_description = true;
    }
  return 0;
  }



KNutNet::NetError KNutNet::getUpsVars1 ( void) {
// getUpsVars nacte promene a jejich typ
// vraci kod chyby nebo 0 pokud je vse v poradku
  QByteArray  inBuffer;
  QString  outBuffer;
  QString  varOutBuffer;
  upsVarDef upsVar;
  upsICommDef upsIComm;

  // init vars and lists
  if (!m_upsName.isEmpty()) { // pokud upsName neni je vzdy nastaveno na null
    inBuffer="LISTVARS ";
    inBuffer += m_upsName.toLocal8Bit() + "\n";
    }
  else inBuffer ="LISTVARS\n";

  if ((m_errorState=getUpsData (inBuffer,outBuffer)) == NoError) {
    outBuffer=outBuffer.trimmed();
    if (outBuffer.length() > 0) {
    // precteme data
      QTextStream inputStream(&outBuffer);
      QString word;
      int count =1 ;
      while (!inputStream.atEnd()) {
        inputStream >> word;
        switch (count) {
          case 1:
          // vyndame prvni polozku
          if (word == "VARS") count++;
          else {
            if (word == "ERR") {
              inputStream >> word;
              m_errorState = upsTranslateError(word);
              }
            else m_errorState=UnknownAnswer;
            return m_errorState;
            }
          break;
        case 2:
          count++;
          if ((word.length() > 0) && word.startsWith("@")) break;
          default:
          // pridame polozku do tabulky
          upsSetType(upsVar, word);
          m_listVars.push_back(upsVar);
          m_numberVars++;
          break;
          }
        }
      }
    else { m_errorState=NoData; return m_errorState; }
    }
  else return m_errorState;

  if ((m_errorState = getUpsValues1(true))) return m_errorState;

  // *********** RW VARIABLES
  if (!m_upsName.isEmpty()) { // pokud upsName neni je vzdy nastaveno na null
    inBuffer="LISTRW ";
    inBuffer += m_upsName.toLocal8Bit() + "\n";
    }
  else inBuffer ="LISTRW\n";

  if ((m_errorState=getUpsData (inBuffer,outBuffer)) == NoError) {
    outBuffer=outBuffer.trimmed();
    if (!outBuffer.isEmpty()) {
    // precteme data
      QTextStream inputStream(&outBuffer);
      QString word;
      int count =1 ;
      bool valueType;
      int varMax;
      while (!inputStream.atEnd()) {
        inputStream >> word;
        switch (count) {
          case 1:
          // vyndame prvni polozku
           if (word == "RW") count++;
           else {
             if (word == "ERR") {
               inputStream >> word;
               m_errorState = upsTranslateError(word);
               }
             else m_errorState=UnknownAnswer;
             return m_errorState;
             }
          break;
          case 2:
          count++;
          if ((word.length() > 0) && word.startsWith("@")) break;
          default:
          // Zpracujeme polozku v tabulce
          // zjistime informaci o promene
          inBuffer="VARTYPE " + word.toLocal8Bit();
          if (!m_upsName.isEmpty()) {
            inBuffer.append("@"); // pokud upsName neni je vzdy nastaveno na null
            inBuffer.append(m_upsName.toLocal8Bit());
            }
          inBuffer.append("\n");
          if ((m_errorState=getUpsData (inBuffer,varOutBuffer)) == NoError) {
            varOutBuffer=varOutBuffer.trimmed();
            if ( !outBuffer.isEmpty() ) {
              QTextStream varInputStream(&varOutBuffer);
              QString word1, word2, word3;
              varInputStream >> word1;
              varInputStream >> word2;
              varInputStream >> word3;
              if (word1 == "TYPE") {
                if ((word2.isEmpty()) || (word3.isEmpty())) {
                  m_errorState=UnknownFormat;
                  return m_errorState;
                  }
                if (word2 == "ENUM") valueType=false;
                else {
                  if (word2 =="STRING") valueType=true;
                  else {
                    m_errorState=UnknownFormat;
                    return m_errorState;
                    }
                  }
                bool ok = true;
                varMax=word3.toInt(&ok);
                if (!ok) {m_errorState=UnknownFormat; return m_errorState;};
                } // word1 = Type
              else {
                if (word1=="ERR") {
                  m_errorState = upsTranslateError(word2);
                  return m_errorState;
                  }
                else {
                  m_errorState = UnknownAnswer;
                  return m_errorState;
                  }
                }
              }
            else { m_errorState=NoData; return m_errorState; }
            }
          else return m_errorState;
          // ********** ENUM / STRING ******************

          QVector<QString>* enumString=0;
          if (!valueType) {
          // nacteme enum hodnoty
            inBuffer="ENUM " + word.toLocal8Bit();
            // pokud existuje pridame jmeno UPS-ky
            if (!m_upsName.isEmpty()) {
              inBuffer.append("@");
              inBuffer.append(m_upsName.toLocal8Bit());
              }
            inBuffer.append("\n");
            if ((m_errorState=getUpsData (inBuffer,varOutBuffer,"END\n")) == NoError) {
//               varOutBuffer.stripWhiteSpace();  nemuze provest protoze bychom si odstranili konce radek
              if (!varOutBuffer.isEmpty()) {
                QString inLine, word1, word2;
                QTextStream varInputStream(&varOutBuffer);
                int inCountLine=1;
                while (!(inLine = varInputStream.readLine()).isNull()) {
                  inLine=inLine.trimmed();

                  if ((m_posChar=inLine.indexOf(' ')) == -1) word1=inLine;
                  else {
                    word1=inLine.left(m_posChar);
                    inLine = inLine.mid(m_posChar+1);
                    }
                  if (word1.isEmpty()) { m_errorState = UnknownFormat; return m_errorState; }
                  if (inCountLine == 1) {
                  if ((m_posChar=inLine.indexOf(' ')) == -1) word2=inLine;
                  else word2=inLine.left(m_posChar);
                    if (word1 =="ENUM") {
                      if (word2 != word) { m_errorState = UnknownFormat; return m_errorState; }
                      varMax=0;
                      inCountLine++;
                      enumString = new QVector<QString>;
                      enumString->clear();
                      }
                    else {
                      if (word1=="ERR")
                        m_errorState=upsTranslateError(word2); // prevede chybu na jeji kod
                      else m_errorState=UnknownAnswer;
                      return m_errorState;
                      }
                    }
                  else {
                    if (word1 == "END") break;
                    if (word1 != "OPTION") { m_errorState = UnknownFormat; return m_errorState; }
                    if ((m_posChar = inLine.indexOf('\"')) == -1) word2 = "";
                    else {
                      word2 = inLine.mid(m_posChar+1);
                      if ((m_posChar=word2.indexOf('\"')) != -1) word2 = word2.left(m_posChar);
                      }
                    enumString->push_back(word2);
                    varMax++;
                    }
                  } // end while
                } // buffer is not empty
              else { m_errorState = NoData ; return m_errorState; }
              } // getUpsData
            else return m_errorState;
            } // type of var
          else  enumString = 0l;
          // nacteme stringovou promenou
          // ulozime promene
          setRWVars (word,valueType,varMax,enumString);
          m_numberRWVars++;
          break;
          }
        } // end  while (!inputStream.atEnd())
      }
    else { m_errorState = NoData ; return m_errorState; }
    }
  else return m_errorState;

  // *************** INSTANT COMMAND
  if (!m_upsName.isEmpty()) { // pokud upsName neni je vzdy nastaveno na null
    inBuffer="LISTINSTCMD " + m_upsName.toLocal8Bit() + "\n";
    }
  else inBuffer ="LISTINSTCMD\n";

  if ((m_errorState=getUpsData (inBuffer,outBuffer)) == NoError) {
    outBuffer=outBuffer.trimmed();
    if ( !outBuffer.isEmpty() ) {
      // precteme data
      QTextStream inputStream(&outBuffer);
      QString word;
      int count =1 ;
      while (!inputStream.atEnd()) {
        inputStream >> word;
        switch (count) {
          case 1:
          // vyndame prvni polozku
           if (word == "INSTCMDS") count++;
           else {
             if (word == "ERR") {
               inputStream >> word;
               m_errorState = upsTranslateError(word);
               }
             else m_errorState=UnknownAnswer;
             return m_errorState;
             }
          break;
          case 2:
          count++;
          if ((word.length() > 0) && word.startsWith("@")) break;
          default:
          // Zpracujeme polozku v tabulky
          upsIComm.upsCommName=word;
          upsIComm.upsDescription="";
          m_listIComms.push_back(upsIComm);
          m_numberIComms++;
          break;
          }
        }
      }
    else { m_errorState=NoData; return m_errorState; }
    }
  else return m_errorState;
  return NoError;
  }



KNutNet::NetError KNutNet::getUpsVars2 ( void) {
  QByteArray  inBuffer;
  QString  outBuffer;
  QString  varOutBuffer;
  upsVarDef upsVar;
  upsICommDef upsIComm;
  int key;
  int typeValue;
  int lenString;
  int varMax=0;

qDebug("KNutNet::getUpsVars2");

  QVector<QString>* enumString=0;
  inBuffer = "LIST VAR " + m_upsName.toLocal8Bit() + "\n";
  if ((m_errorState=getUpsData (inBuffer,outBuffer,"END LIST VAR")) == NoError) {

    QString inLine, upsNameRet, varName, varNameRet, value;
    bool beginList = false;
    QTextStream varInputStream(&outBuffer);
    while (!(inLine = varInputStream.readLine()).isNull()) {

      key = parseLine (inLine, upsNameRet, varName, value, typeValue, lenString);
      switch (key) {
        case BEGIN_LIST_VAR:
          if (upsNameRet == m_upsName) beginList = true;
        break;
        case VAR:
         if (beginList ) {
           upsSetType(upsVar, varName, value);

           if (varName == "ups.status") genStatusFlags(value);
           inBuffer = "GET TYPE " + m_upsName.toLocal8Bit() + " " + varName.toLocal8Bit() +"\n";
           if ((m_errorState=getUpsData (inBuffer,varOutBuffer)) == NoError) {
             key = parseLine (varOutBuffer, upsNameRet, varNameRet, value, typeValue, lenString);
             switch (key) {
               case TYPE:
                if (typeValue & RW_FLAG) {
                  upsVar.upsVarMax = 0;
                  upsVar.upsEnumValues=0L;
                  if (typeValue & ENUM_FLAG) {
                    upsVar.upsValueType=false;
                    // nacteme enumValues
                    inBuffer = "LIST ENUM " + m_upsName.toLocal8Bit() + " " + varName.toLocal8Bit() +"\n";
                    if ((m_errorState=getUpsData (inBuffer,varOutBuffer,"END LIST ENUM")) = NoError) {
                      bool beginEnumList = false;
                      QTextStream varInputStream(&varOutBuffer);
                      while (!(inLine = varInputStream.readLine()).isNull()) {
                        key = parseLine (inLine, upsNameRet, varName, value, typeValue, lenString);
                        switch (key) {
                          case BEGIN_LIST_ENUM:
                            beginEnumList = true;
                            varMax=0;
                                 enumString = new QVector<QString>;
                            enumString->clear();
                            upsVar.upsEnumValues=enumString;
                          break;
                          case ENUM:
                            if (beginList ) {
                                  enumString->push_back(value);
                              varMax++;
                              }
                          break;
                          case ERR:
                            m_errorState = upsTranslateError(value);
                          return m_errorState;
                          default:
                            m_errorState=UnknownAnswer;
                          return m_errorState;
                          case END_LIST_ENUM:
                          upsVar.upsVarMax=varMax;
                          break;
                          }
                        }
                      }
                    }
                  else {
                    // Vse co neni ENUM je STRING
                    upsVar.upsValueType=true;
                    upsVar.upsVarMax=lenString;
                    }
                  upsVar.upsVarType=false;
                  m_numberRWVars++;
                  }
                 break;
               case ERR:
                 m_errorState = upsTranslateError(value);
                 return m_errorState;
               default:
                 m_errorState=UnknownAnswer;
                 return m_errorState;
               }
             }
           else { return m_errorState; }
           m_listVars.push_back(upsVar);
           m_numberVars++;
           }
          break;
        case END_LIST_VAR:
          break;
        case ERR:
          m_errorState = upsTranslateError(value);
          return m_errorState;
        default:
          m_errorState=UnknownAnswer;
          return m_errorState;
        } // end switch
      } // end while
    }
  else { return m_errorState; }
  // INSTANT COMMANDS

  inBuffer = "LIST CMD " + m_upsName.toLocal8Bit() + "\n";
  if ((m_errorState=getUpsData (inBuffer,outBuffer,"END LIST CMD")) == NoError) {

    QString inLine, upsNameRet, varName, value;
    int key, typeValue;
    bool beginList = false;

    QTextStream varInputStream(&outBuffer);
    while (!(inLine = varInputStream.readLine()).isNull()) {
      key = parseLine (inLine, upsNameRet, varName, value, typeValue, lenString);

      switch (key) {
        case BEGIN_LIST_CMD: {
          if (upsNameRet == m_upsName) beginList = true;
          break;
          }
        case CMD:
         if (beginList ) {
           // Zpracujeme polozku v tabulky
           upsIComm.upsCommName=varName;
           upsIComm.upsDescription="";
           m_listIComms.push_back(upsIComm);
           m_numberIComms++;
           }
         break;
        case END_LIST_CMD:
          break;
        case ERR:
          m_errorState = upsTranslateError(value);
          return m_errorState;
        default:
          m_errorState=UnknownAnswer;
          return m_errorState;
        } // end switch
      } // end while
    }
  else { return m_errorState; }
  return NoError;
  }



KNutNet::NetError KNutNet::getUpsValues1 (const bool allVars ) {// allVars = true vse; allVars = false jen activate
    QString endChar;
    QByteArray  inBuffer;
    QString  outBuffer;
    QVector<upsVarDef>::iterator it;

  // Nacte hodnotu promenych
  // reads value of variables
qDebug("KNutNet::getUpsValues1");


    emit getVarDesc (m_numberVars, 0);

    int numberVar  = 0;
    for (it = m_listVars.begin(); it != m_listVars.end(); it++) {
      numberVar++;
      if ((allVars) || ((*it).upsVarActive)) {
        //nacteme promenou
        inBuffer = "REQ " + (*it).upsVarName.toLocal8Bit();
        if (!m_upsName.isEmpty()) { // pokud upsName neni je vzdy nastaveno na null
          inBuffer.append("@");
          inBuffer.append(m_upsName.toLocal8Bit());
          }
        inBuffer.append("\n");
//        if ((error=getUpsData (inBuffer,outBuffer)) == NoError) {
        m_errorState=getUpsData (inBuffer,outBuffer);
        // when outBuffer is empty sends and reads date again, but only the only time
        if ((m_errorState == NoError) && (outBuffer.isEmpty())) m_errorState=getUpsData (inBuffer,outBuffer);
        if (m_errorState == NoError) {

          endChar = "";
          if ((outBuffer.length()>0) && (outBuffer.at(outBuffer.length()-2)==' ')) endChar=' ';

          outBuffer=outBuffer.trimmed();
          outBuffer += endChar;
          emit getVarDesc (m_numberVars, numberVar);

          if ( !outBuffer.isEmpty() ) {
            QTextStream inputStream(&outBuffer);
            QString word1, word2, word3 ;
            inputStream >> word1;
            inputStream >> word2;
            if (inputStream.atEnd()) { // neexistuje treti cast VALUE
              if (word1 == "ERR") {
                // vracena chyba
                m_errorState=upsTranslateError(word2);
                return m_errorState;
                }
              else {
//                if ((word2 != "MODEL") && (word2 != "MFR")) {   
                if (endChar != " ") { // if values of variable is empty, string is ended on char space
                  m_errorState=UnknownFormat;
                  return m_errorState;
                  }
                }
              }
            inputStream >> word3;
            if (word1 == "ANS") {
              // answer - odpoved
              // odstranime pripadne jmeno upsky
              if ((m_posChar = word2.indexOf('@')) != -1) word2 = word2.left(m_posChar);
              if ( word2 != (*it).upsVarName ) {
                m_errorState=UnknownAnswer;
                return m_errorState;// neni to odpoved na spravnou promennou
                }
              // test pro starsi format modulu, vraceli chybu v hodnote promene
              if ((m_errorState=upsOldTranslateError(word3))) {
                return m_errorState;
                }
              // zalozime hodnotu do vectoru;
              (*it).upsValue=word3;
              if ((*it).upsVarName == "STATUS") {
                // dogenerujeme dalsi slova pro status
                word3 += inputStream.readLine();
                genStatusFlags(word3);
                }
              }
            else {
              // neznama odpoved - neni ANS ani ERR
              m_errorState=UnknownAnswer;
              return m_errorState;
              }
            }
          else {
            m_errorState=NoData;
            return m_errorState;
            }
          }
        else {
          return m_errorState;
          }
        }
      }
    return NoError;
  }



KNutNet::NetError KNutNet::getUpsValues2 (const bool allVars ) {// allVars = true vse; allVars = false jen activate
  QString varName;
  QString upsNameRet;
  QString value;
  int typeValue;
  int lenString;

  // reads value of variables
  QByteArray  inBuffer;
  QString  outBuffer;
  QVector<upsVarDef>::iterator it;

qDebug ("KNutNet::getUpsValues2");
  emit getVarDesc (m_numberVars, 0);
   int numberVar  = 0;
  for (it = m_listVars.begin(); it != m_listVars.end(); it++) {

    numberVar++;
    if ((allVars) || ((*it).upsVarActive)) {
      //reads variable
      inBuffer = "GET VAR "+m_upsName.toLocal8Bit()+" "+(*it).upsVarName.toLocal8Bit() + "\n";

      m_errorState=getUpsData (inBuffer,outBuffer);
      // when outBuffer is empty sends and reads date again, but only the only time
      if ((m_errorState == NoError)  &&  (outBuffer.isEmpty())) m_errorState=getUpsData (inBuffer,outBuffer);
      if (m_errorState == NoError) {
        outBuffer=outBuffer.trimmed();
        emit getVarDesc (m_numberVars, numberVar);
        if ( !outBuffer.isEmpty() ) {
          int key = parseLine(outBuffer, upsNameRet, varName, value, typeValue, lenString);
          switch (key) {
            case VAR:
              if ( varName != (*it).upsVarName ) {
                m_errorState=UnknownAnswer;
                return m_errorState;// it isn't answer for right variable - neni to odpoved na spravnou promennou
                }
              (*it).upsValue=value;
              if ((((*it).upsVarName == "STATUS") && (m_nutVariables == versionVarOne)) || (((*it).upsVarName == "ups.status") && (m_nutVariables == versionVarTwo))) {
                // dogenerujeme dalsi slova pro status
                genStatusFlags(value);
                }
              break;  
            case ERR:
              m_errorState=upsTranslateError(value);
              return m_errorState;
            default:
              m_errorState=UnknownAnswer;
              return m_errorState;
            }
          }
        else {
          m_errorState= NoData;
          return m_errorState;
          }
        }
      else {
        return m_errorState;
        }
      }
    }
  return NoError;
  }


KNutNet::NetError KNutNet::sendComm (const QString command, const QString arg1, const QString arg2, const bool useUpsName) {
  NetError localError;

// nenastavi promenou error ale nenuluje ji
  QByteArray inBuffer;
  QString outBuffer;

  switch (m_nutProtocol) {
    case 1:
    inBuffer=command.toLocal8Bit() + " " + arg1.toLocal8Bit();
    if (useUpsName && (!m_upsName.isEmpty())) // pokud upsName neni je vzdy prazdne
      inBuffer += "@"+m_upsName.toLocal8Bit();
    if (!arg2.isEmpty()) inBuffer += " " + arg2.toLocal8Bit();
    break;
    case 2:
    inBuffer = command.toLocal8Bit() + " ";
    if (useUpsName) inBuffer += m_upsName.toLocal8Bit() + " ";
    inBuffer.append(arg1.toLocal8Bit());
    if (!arg2.isEmpty()) inBuffer += " \"" + arg2.toLocal8Bit() +"\"";
    break;
    }
  inBuffer.append("\n");

  if ((localError=getUpsData (inBuffer,outBuffer)) == NoError) {
    outBuffer=outBuffer.trimmed();
    QTextStream inputStream(&outBuffer);
    QString word1, word2;
    inputStream >> word1;
    inputStream >> word2;
    if (word1.isEmpty()) return UnknownAnswer; // zadna odpoved
    if (word1 != "OK") {
      // neni rovno OK
      if (word1 == "ERR") return upsTranslateError (word2);
      else return UnknownAnswer;
      }
    else return NoError;
    }
  else return localError;
  }


int KNutNet::setActiveUpsVar ( const QString name, const bool active ) {
  m_errorState =NoError;
  if (m_state == Connected) {
    QVector<upsVarDef>::iterator it;
    for (it = m_listVars.begin(); it != m_listVars.end(); it++) {
      if ((*it).upsVarName == name) {
        (*it).upsVarActive = active;
        return 0;
        }
      }
    m_errorState = VarNotFind;
    return m_errorState;
    }
  else return NotConnection;
  }

int KNutNet::setActiveAllUpsVars ( const bool active  ) {

  m_errorState =NoError;
  if (m_state == Connected) {
    QVector<upsVarDef>::iterator it;
    for (it = m_listVars.begin(); it != m_listVars.end(); it++) {
      (*it).upsVarActive = active;
      }
    return 0;
    }
  else return NotConnection;
  }


KNutNet::NetError KNutNet::upsTranslateError (const QString string) {
  if (string == "VAR-NOT-SUPPORTED") return VarNotSupported;
  if (string == "VAR-UNKNOWN") return VarUnknown;

  if (string == "ACCESS-DENIED") return AccessDenied;
  if (string == "PASSWORD-REQUIRED") return PasswordRequired;
  if (string == "PASSWORD-INCORRECT") return PasswordIncorrect;
  if (string == "UNKNOWN-UPS") return UnknownUps;
  if (string == "ALREADY-LOGGED_IN") return AlreadyLoggedIn;
  if (string == "ALREADY-SET-PASSWORD") return AlreadySetPassword;
  if (string == "ALREADY-SET-USERNAME") return AlreadySetUsername;
  if (string == "UNKNOWN-COMMAND") return UnknownCommand;

  if (string == "UNKNOWN-INSTCMD") return UnknownInstCmd;
  if (string == "CMD-NOT-SUPPORTED") return CmdNotSupported;

  if (string == "INVALID-VALUE") return InvalidValue;

  if (string == "NO-RESPONSE") return NoResponse;
  if (string == "UNKNOWN-REPLY") return UnknownReply;
  if (string == "NOT-IMPLEMENTED") return NotImplemented;
  if (string == "COMMAND-FAILED") return CommandFailed;

  if (string == "MISSING-ARGUMENT") return MissingArgument;
  if (string == "DATA-STALE") return DataStale;
  if (string == "UNKNOWN-TYPE") return UnknownType;

  if (string == "DRIVER-NOT-CONNECTED") return DriverNotConnected;

  if (string == "") return UnknownFormat;

  return UnknownErr;
  }

KNutNet::NetError KNutNet::upsOldTranslateError (const QString string) {
  if (string == "NOT-SUPPORTED") return VarNotSupported;
  if (string == "UNKNOWN") return VarUnknown;
  if (string == "DATA-STALE") return DataStale;
  return NoError; // nebylo nalezeno
  }

//--------------------Socket----------------


void KNutNet::slotConnectionError(QAbstractSocket::SocketError socketError) {
qDebug ("KNutNet::slotConnectionError");

  m_unConnectTimer->stop();

  if (socketError == QAbstractSocket::HostNotFoundError) {
    m_errorState = NoSuchHost;
    emit connectionError(m_errorState);
    return;
    }
                        //connection was refused                                  //  no connection
  if ((socketError == QAbstractSocket::ConnectionRefusedError) && (m_commandSocket->state()==QAbstractSocket::UnconnectedState)) {
    if (m_firstConnect) {
      if (m_numberConnection < m_countRepeat) {
        m_numberConnection++;
        m_unConnectTimer->setSingleShot( TRUE );
        m_unConnectTimer->start( m_delay );
        }
      else  {
        // i don't connect server
        m_errorState=CantConnect;
        emit connectionError(m_errorState);
        return;
        }
      } // end of if (firstConnection)
    else {
      // connecting is down, try repeated connection

      m_unConnectTimer->setSingleShot (TRUE);
      m_unConnectTimer->start( m_delay);
      }
    return;
    }

  if (socketError == QAbstractSocket::NetworkError) {
    m_errorState = NetworkError;
    emit connectionError(m_errorState);
    return;
    }

  if (socketError == QAbstractSocket::SocketAccessError) {
    m_errorState = SocketAccessError;
    emit connectionError(m_errorState);
    return;
    }

  if (socketError == QAbstractSocket::SocketResourceError) {
//qDebug ("KNutNet::slotConnectionError SocketResourceError");
    m_errorState = SocketResourceError;
    emit connectionError(m_errorState);
    return;
    }

  if (socketError == QAbstractSocket::SocketTimeoutError) {
//qDebug ("KNutNet::slotConnectionError SocketTimeoutError");
    m_errorState = SocketTimeout;
//    emit connectionError(error);
    return;
    }


  if (socketError == QAbstractSocket::RemoteHostClosedError) {
//qDebug ("KNutNet::slotConnectionError   RemoteHostClosedError");
    m_errorState = ConnetionClosedByServer;
    //slotDisconnectes is called auto by signal disconnected
    emit connectionClosedByServer();
    return;
    }


  if (socketError ==   QAbstractSocket::UnknownSocketError) {
//qDebug ("KNutNet::slotConnectionError UnknownSocketError");
    m_errorState = UnknownSocketError;
    emit connectionError(m_errorState);
    return;
    }
  }


void KNutNet::slotConnected(void) {
qDebug ("KNutNet::slotConnected");
  int n;

    if ((n = version (m_countRepeat, m_delay)) == 0) { // 0 OK, 0> error
      if ((m_nutProtocol == 0) || (m_nutVariables == versionVarUnknown)) { // answer from upsd (VER) is unknown
        m_errorState=NutProtocolVarErr;
        m_state = Closing;
        m_commandSocket->close();
        while (m_commandSocket->state() == QAbstractSocket::ClosingState) {
          KNutSleep::msleep(100);
          }
        return;
        }
      if ((m_nutProtocol == 2) && m_upsName.isEmpty()) {
      // if upsName is empty reads first ups name from upsd server 
        if ((m_errorState =KNutNet::getFirstUpsName (m_upsName)) != NoError) { // reading first upsName
          // upsd server doesn't support any ups
          m_state = Closing;
          m_commandSocket->close();
          while (m_commandSocket->state() == QAbstractSocket::ClosingState) {
            KNutSleep::msleep(100);
            }
          return;
          }
        }
      m_state=Connected;
      }
    else {
        m_state = Closing;
        m_commandSocket->close();
       }
    emit firstConnection();
  }

void KNutNet::slotHostFound(void) {

  qDebug("KNutNet::slotHostFound");
  m_state = Connecting;
  emit hostFound();
  }


void KNutNet::slotTimerReconnect(void) {
qDebug("KNutNet::slotTimerReconnect");

 m_unConnectTimer->stop();
  if (m_commandSocket->state()==QAbstractSocket::UnconnectedState) {
     emit tryRepeatFirstConnection(m_numberConnection);
//    m_state=Connecting;
    if ( m_commandSocket->state() != QAbstractSocket::ConnectingState )
      m_state = HostLookup;
      m_commandSocket->connectToHost(m_upsAddress,m_port);
    }
  }




#include "knutnet.moc"
