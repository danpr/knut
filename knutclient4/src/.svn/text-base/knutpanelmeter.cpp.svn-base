/***************************************************************************
                          knutpanelmeter.cpp  -  description
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

#include "knutpanelmeter.h"


#include <QtGui/QLayout>
#include <QtGui/QLabel>
#include <QtGui/QFont>


#include <math.h>
#include <stdlib.h>
#include <unistd.h>

//#include <iostream>



KNutPanelMeter::KNutPanelMeter(QPalette aPanelPalette, KNutVarData::upsInfoDef* activeUpsInfo, KNutVarData::SettingDataDef* settingData, KNutVarData* analogRecords, QWidget *parent) : QWidget(parent), m_settingData(settingData), m_activeUpsInfo(activeUpsInfo), m_analogRecords(analogRecords), m_aPanelPalette(aPanelPalette) {

  setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Preferred);

  m_analogPanels.clear();
  m_widthArea =0;
  m_heightArea =0;
 }

KNutPanelMeter::~KNutPanelMeter(){

  deActivateAnalogs();
  }



void KNutPanelMeter::deActivateAnalogs (void) {

qDebug("KNutPanelMeter::deActivateAnalogs");

  analogPanelsList::const_iterator itc;
  for (itc = m_analogPanels.begin(); itc < m_analogPanels.end(); itc++) {
    if ( (*itc).upsAnalogs != 0) {
        (*itc).upsAnalogs->hide(); // hides analogMeter
        delete (*itc).upsAnalogs; // deletes analogMeter
        }
    }
  m_analogPanels.clear();
  setSize();
  }


void KNutPanelMeter::activateAnalogs (void) {
qDebug ("KNutPanelMeter::activateAnalogs");

  KNutVarData::VarStrucDef analogMemberData;
  // deletes variables which ups doesn't support 
  // vyjmeme promene ktere ups nepodporuje

  int ii =0;
  for (int i=0; i< knc::NumberOfVars; i++) {
    // otestujeme zda dane UPS podporuje tuto promenou // pokud existuje vrati 1
  // moves free records // is null/ to end

    if (!m_activeUpsInfo->upsNet->existName(KNutVarData::nameUpsVar (m_activeUpsInfo->record.upsVarCode[i],m_activeUpsInfo->nutVariables))) {
      m_activeUpsInfo->record.upsVarCode[ii++] = m_activeUpsInfo->record.upsVarCode[i];
      }
    }
  for (int i=ii; i< knc::NumberOfVars; i++) m_activeUpsInfo->record.upsVarCode[i]=0;// doplnime zbytek nulami to znamena zadna promena

  for (int i=0; i< knc::NumberOfVars; i++) {
    if (m_activeUpsInfo->record.upsVarCode[i] != 0) {


      analogPanelRecordDef analogPanelRecord;
      analogMemberData=m_analogRecords->getData (m_activeUpsInfo->record.upsVarCode[i]);

      analogPanelRecord.upsVarCode = m_activeUpsInfo->record.upsVarCode[i];

      analogPanelRecord.scaleEnd=analogMemberData.end;
      analogPanelRecord.scaleStart=analogMemberData.start;
      QColor analogFingerColor, analogScaleColor, analogFontColor;
      if (m_settingData->customOAnalogColor) {
        analogFingerColor = m_settingData->analogFingerColor;
        analogScaleColor = m_settingData->analogScaleColor;
        analogFontColor = m_settingData->analogFontColor;
        }
      else {
        analogFingerColor = knc::defaultFingerColor;
        analogScaleColor = knc::defaultScaleColor;
        analogFontColor = knc::defaultFontColor;
        }

//sets font
      QFont analogFont;
      if (m_settingData->customFont) {
        analogFont = m_settingData->aPanelFont;
        }
      else {
        analogFont = m_settingData->systemFont;
        }
       // doesn't make background
      analogPanelRecord.upsAnalogs = new KNutAnalog( this,analogMemberData.start,analogMemberData.end,analogMemberData.typeAnalog,analogMemberData.numberItem,analogMemberData.numberInItem,analogFingerColor,analogScaleColor,analogFontColor,&(analogFont),m_settingData->typeDigitalProcessing);
      //nastavime barvu mezikruzi
      if (m_settingData->customOAnalogColor) {
        analogPanelRecord.upsAnalogs->setArcColor(1,m_settingData->analogOKColor); // doesn't make background
        analogPanelRecord.upsAnalogs->setArcColor(2,m_settingData->analogWarnningColor); // doesn't make background
        analogPanelRecord.upsAnalogs->setArcColor(3,m_settingData->analogErrorColor); // doesn't make background
        }
      // po vygenerovani panelu jsou standartni barvy nastaveny automaticky
      //nastavime mezikruzi
      for (int j = 0; j < 5; j++ ) {
        if (!((analogMemberData.scaleStruct[j].start == 0) && (analogMemberData.scaleStruct[j].end ==0 ))) {
          analogPanelRecord.upsAnalogs->addArc (analogMemberData.scaleStruct[j].start,analogMemberData.scaleStruct[j].end,analogMemberData.scaleStruct[j].scaleColor); // doesn't make background
        }
      }
      analogPanelRecord.upsAnalogs->setText(analogMemberData.nameAnalog,analogMemberData.nameUnit);
      analogPanelRecord.upsAnalogs->setPalette(m_aPanelPalette);
      analogPanelRecord.upsAnalogs->makeBackground(false); // makes background of analog panel
      analogPanelRecord.upsAnalogs->repaintPointer();

      moveUpsAnalog (i, analogPanelRecord.upsAnalogs);
      analogPanelRecord.upsAnalogs->show();
      //repaintPointer makes all picture of analog panel asembles background with pointer and makes antialiasing
      
      m_analogPanels.push_back(analogPanelRecord);
      }
    }
  setSize();
  }


void KNutPanelMeter::moveUpsAnalog (const int i, KNutAnalog *upsAnalog) {

qDebug("void KNutPanelMeter::moveUpsAnalog ");
  int y = (i/m_settingData->countCols);
  int x = i - y * m_settingData->countCols;

  x = x *(KNutAnalog::AnalogWidth+spacePanels) +spacePanels;
  y = y * (KNutAnalog::AnalogWidth+spacePanels) + spacePanels;
  upsAnalog->move(x,y);
  }

void KNutPanelMeter::setSize (void) {

qDebug("KNutPanelMeter::setSize");

int x =0;
int y =0;
if (m_analogPanels.count() != 0) { 
   y = ((m_analogPanels.count()-1)/m_settingData->countCols)+1;
  if (m_analogPanels.count() > m_settingData->countCols) {
     x = m_settingData->countCols;}
  else {
    x = m_analogPanels.count();
    }
  m_widthArea = x *(KNutAnalog::AnalogWidth+spacePanels) +spacePanels;
  m_heightArea = y * (KNutAnalog::AnalogWidth+spacePanels) + spacePanels;
  }
else {
  m_widthArea = 0;
  m_heightArea = 0;
  }
  setMinimumSize (m_widthArea,m_heightArea);
  setMaximumWidth (m_widthArea);
  adjustSize();
  repaint();
  }

void KNutPanelMeter::repaintScaleAnalogPanel (int upsVarCode, float value) {
qDebug("KNutPanelMeter::repaintScaleAnalogPanel");

  bool scaleChanged = false;
  analogPanelsList::iterator it;

  for (it = m_analogPanels.begin(); it < m_analogPanels.end(); it++) {
  if ((*it).upsAnalogs != 0){
    if ( (*it).upsVarCode == upsVarCode) {
      switch ((*it).upsVarCode) {

        case knc::VarOutCurrent : // output.current
          if ((*it).scaleEnd < maxCurrent) { // max. measured value for analog pointer
            if ((*it).scaleEnd < value) {
              (*it).scaleEnd=(static_cast<int>(floor((value+1)/5)))*5 +5;
              if ((*it).scaleEnd > maxCurrent) (*it).scaleEnd=maxCurrent;
                (*it).upsAnalogs->setRange((*it).scaleStart,(*it).scaleEnd,true);
              scaleChanged = true;
              }
            }
          break; // end VarOutCurrent

        case knc::VarInCurrent : // input.current
          if ((*it).scaleEnd < maxCurrent) { // max. measured value for analog pointer
            if ((*it).scaleEnd < value) {
              (*it).scaleEnd=(static_cast<int>(floor((value+1)/5)))*5 +5;
              if ((*it).scaleEnd > maxCurrent) (*it).scaleEnd=maxCurrent;
                (*it).upsAnalogs->setRange((*it).scaleStart,(*it).scaleEnd,true);
              scaleChanged = true;
              }
            }
          break; // end VarIntCurrent

        case knc::VarUpsPower : // ups.power
          if ((*it).scaleEnd < maxUpsPower) { /// max. measured value for analog pointer
            if ((*it).scaleEnd < value) {
              (*it).scaleEnd =  (trunc(value/500)+1)*500; 
              if ((*it).scaleEnd > maxUpsPower) (*it).scaleEnd = maxUpsPower;
                (*it).upsAnalogs->setRange((*it).scaleStart,(*it).scaleEnd,true);
              scaleChanged = true;
              }
            }
          break; // end VarUpsPower

          case knc::VarBattVolt : // battery.voltage
            if ((*it).scaleEnd < maxBattVolt) {
              if ((*it).scaleEnd < value) {
                (*it).scaleEnd=(static_cast<int>(floor((value+1)/10)))*10 +10;
                if ((*it).scaleEnd > maxBattVolt) (*it).scaleEnd=maxBattVolt;
                  (*it).upsAnalogs->setRange((*it).scaleStart,(*it).scaleEnd,true);
              scaleChanged = true;
                }
              }
            break;

          case knc::VarBattCurrent : // battery.current
            if (((*it).scaleEnd < maxBattCurrent) && ((*it).scaleStart > minBattCurrent)) {
              if (((*it).scaleEnd < value) || ((*it).scaleStart > value)) {
                if ((value >10) || (value <- 15)) {
                  if ((value >20) || (value < -30)) {
                    if ((value >30) || (value < -45)) {
                      if ((value >100) || (value < -150)) {
                        (*it).scaleEnd=200;
                        (*it).scaleStart=-300;
                        }
                      else { 
                        (*it).scaleEnd=100;
                        (*it).scaleStart=-150;
                        }
                      }
                    else {
                      (*it).scaleEnd=30;
                      (*it).scaleStart=-45;
                      }
                    }
                  else {
                    (*it).scaleEnd=20;
                    (*it).scaleStart=-30;
                    }
                  }
                (*it).upsAnalogs->setRange ((*it).scaleStart,(*it).scaleEnd);
                scaleChanged = true;
                }
              }
            break;

          } // switch
        if (scaleChanged) {
//          (*it).upsAnalogs->hide();// schovame analogovy panel   hides analog panel
          (*it).upsAnalogs->setPointer(value); // jistota pokud by nam to nekde propadlo
//          (*it).upsAnalogs->show(); //zobrazime merak  shows analog panel
          (*it).upsAnalogs->repaint(); // prekreslime , repaints panel

          }
        } // panelNumber
      } //analog
   } // for it
}


void KNutPanelMeter::repaintAnalogPointer (const int upsVarCode, const float value) {
qDebug("KNutPanelMeter::repaintAnalogPointer");

  analogPanelsList::const_iterator itc;
  for (itc = m_analogPanels.begin(); itc < m_analogPanels.end(); itc++) {
  if ((*itc).upsVarCode == upsVarCode) {
    if ((*itc).upsAnalogs != NULL) {
      (*itc).upsAnalogs->setPointer (value); // nastavi hodnotu rucicky
      (*itc).upsAnalogs->repaint();
      }
    }
  }
}



void KNutPanelMeter::setAnalogBG(const QPalette &aPanelPalette) {

  m_aPanelPalette = aPanelPalette;
  analogPanelsList::iterator it;
  for (it = m_analogPanels.begin(); it < m_analogPanels.end(); it++) {

    if  ((*it).upsAnalogs)
      (*it).upsAnalogs->setPalette(m_aPanelPalette);
    }
 }


void KNutPanelMeter::changeAnalogPanelsDigProcessing (void) {

  analogPanelsList::iterator it;
  for (it = m_analogPanels.begin(); it < m_analogPanels.end(); it++) {
    if ((*it).upsAnalogs != 0L) (*it).upsAnalogs->setDigitalProcesing(m_settingData->typeDigitalProcessing,true);
    }
  }


void KNutPanelMeter::repaintAnalogPanels (const bool repaintRangeFrequencyPanels, const bool repaintRangeVoltagePanels, const bool repaintAllPanels) {
qDebug ("KNutPanelMeter::repaintAnalogPanels");
KNutVarData::VarStrucDef analogMemberData;

  // pregenerovani panelu slouzi pro settingMain a setting Analog
  analogPanelsList::iterator it;
  for (it = m_analogPanels.begin(); it < m_analogPanels.end(); it++) {

    if ((*it).upsAnalogs != 0) {  
           //if upsAnalogs[x] is set to zero, this upsAnalog is not used
           // record.upsVarCode contain list of code of analog meter 
           // 0 is code for none analog meter, 1 for Input Voltage, 2 for Battery Charge,
      if ((((*it).upsVarCode == knc::VarInFreq) && (repaintRangeFrequencyPanels)) ||
           (((*it).upsVarCode == knc::VarOutFreq) && (repaintRangeFrequencyPanels)) ||
           ((((*it).upsVarCode == knc::VarInVolt) ||
             ((*it).upsVarCode == knc::VarOutVolt)) && (repaintRangeVoltagePanels))) {
        analogMemberData=m_analogRecords->getData ((*it).upsVarCode); // nacteme data
        (*it).upsAnalogs->hide();// schovame analogovy panel
        (*it).upsAnalogs->delArc(); //zrusime segmenty
        (*it).upsAnalogs->setRange (analogMemberData.start,analogMemberData.end,false);
        for (int j = 0; j < 5; j++ ) {
          if (!((analogMemberData.scaleStruct[j].start == 0) && (analogMemberData.scaleStruct[j].end ==0 ))) {
            (*it).upsAnalogs->addArc (analogMemberData.scaleStruct[j].start,analogMemberData.scaleStruct[j].end,analogMemberData.scaleStruct[j].scaleColor);
            } // end if (!
          }// end for (int i=0

        (*it).upsAnalogs->makeBackground(true); // repaints analog meter

        (*it).upsAnalogs->repaintPointer(); // repaints analog meter
        (*it).upsAnalogs->repaint(); // repaints analog meter
        (*it).upsAnalogs->show(); // shows analog meter
        } // end if (((activeUpsInfo
      if (repaintAllPanels) {
        if (m_settingData->customOAnalogColor) {
          (*it).upsAnalogs->setPointerColor(m_settingData->analogFingerColor);
          (*it).upsAnalogs->setArcColor(1,m_settingData->analogOKColor);
          (*it).upsAnalogs->setArcColor(2,m_settingData->analogWarnningColor);
          (*it).upsAnalogs->setArcColor(3,m_settingData->analogErrorColor);
          (*it).upsAnalogs->setScaleColor(m_settingData->analogScaleColor);
          (*it).upsAnalogs->setFontColor(m_settingData->analogFontColor);
          }
        else {
          (*it).upsAnalogs->setPointerColor (knc::defaultFingerColor);
          (*it).upsAnalogs->setDefaultArcColor();
          (*it).upsAnalogs->setScaleColor (knc::defaultScaleColor);
          (*it).upsAnalogs->setFontColor (knc::defaultFontColor);
          }
        (*it).upsAnalogs->repaintAll();
        }
      }
    } // end for
  setSize();
  }

void KNutPanelMeter::zeroingPanels (void) {
qDebug("KNutPanelMeter::zeroingPanel");
  analogPanelsList::iterator it;
  for (it = m_analogPanels.begin(); it < m_analogPanels.end(); it++) {
    if ((*it).upsAnalogs != NULL) { // jistota pokud by nam to nekde propadlo
      (*it).upsAnalogs->setPointer(0); // nastavi hodnotu rucicky
      (*it).upsAnalogs->repaint();
      }
    }
  }



QSize KNutPanelMeter::sizeHint (void) const {

  return QSize(m_widthArea,m_heightArea);
  }


void KNutPanelMeter::slotSetPanelsFont(const QFont &newFont) {

  analogPanelsList::iterator it;
  for (it = m_analogPanels.begin(); it < m_analogPanels.end(); it++) {
    if ((*it).upsAnalogs) {
      (*it).upsAnalogs->slotSetAllFont(newFont);
      // je nutno prekresli i rucicku
      }
    }
  }

#include "knutpanelmeter.moc"
