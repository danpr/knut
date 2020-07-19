/***************************************************************************
                          knutconst.h  -  description
                             -------------------
    begin                : Tue Sep 24 2001
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

#ifndef KNUTCONST_H
#define KNUTCONST_H

#include <QColor>

namespace knc {

  const int StartPort =1024;
  const int StopPort= 65535;
  const int MinDelay = 2000;

  const int MinCols = 1;
  const int MaxCols = 5;

  const int DefaultCountCols = 3; // prednastaveny pocet sloupcu analogovych meraku
  const int DefaultPort = 3493;  // default ports - prednastaveny port

  const int DefaultDelay = 2000; //2000 msec

  const int DefaultInputVoltage = 230;
  const int DefaultInputFrequency = 50;

  const int NumberOfTypes = 16; // count types of variables //  pocet typu promenych (none,utility ....)
  const int NumberOfVars = NumberOfTypes -1; // how many variables we can measure //  kolik promenych muzeme merit 


  enum {VarNone, // none
       VarInVolt,  //input.voltage
       VarBattPct,  //battery.charge
       VarUpsTemp,  //ups.temperature
       VarInFreq,   //input.frequency
       VarUpsLoad,  //ups.load
       VarAmbTemp,  //ambient.temperature
       VarAmbHumid, //ambient.humidity 
       VarBattVolt, //battery.voltage
       VarOutVolt,  //output.voltage
       VarOutCurrent,  //output.current
       VarOutFreq,  //output.frequency
       VarBattTemp, //battery.temperature
       VarBattCurrent, //battery.current
       VarInCurrent, //input.current
       VarUpsPower }; //ups.power

  const int maxCountKNutEvent = 100;

  const int numberVarError = -1000;

  const QColor defaultBGColor = Qt::lightGray;
  const QColor defaultOkColor = Qt::green;
  const QColor defaultFingerColor = Qt::black;
  const QColor defaultWarnningColor = Qt::yellow;
  const QColor defaultErrorColor = Qt::red;
  const QColor defaultScaleColor = Qt::black;
  const QColor defaultFontColor =  Qt::black;
  const QColor defaultKIconColor =  Qt::white;


  enum upsDriverConnection {  // state of connection to usbd, this isn't state of knutnet 
    connecting,
    connected,
    stale,
    notConnected,
    error
    };

  enum mainWindowStartVariant{
      mw_yes=0,
      mw_no,
      mw_auto
      };



  }


#endif
