/***************************************************************************
                          knutfrontpanel.cpp  -  description
                             -------------------
    begin                : Tue Aug 21 2001
    copyright            : (C) 2001 by Daniel Prynych
    email                : Daniel at prynych.cz
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "knutfrontpanel.h"
#include "knutvardata.h"

#include <KLocalizedString>

#include <QToolTip>
#include <QLabel>
#include <QLCDNumber>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QPalette>
#include <QString>
#include <QFont>


#include <math.h>

KNutFrontPanel::KNutFrontPanel( const QPalette panelPalette, const QFont panelFonts, QWidget *parent, const unsigned int flags) : QFrame( parent), m_panelFlags(flags),m_palette (panelPalette) {

  m_upsOkColor = Qt::green;
  m_upsErrorColor = Qt::red;
  m_upsBackColor = Qt::white; //backgroundColor();
  m_helpColor = Qt::black;
  m_typeOfLCDColor=true;
  m_LCDNumber1 = 0;

 
  this->setFrameStyle( QFrame::Box | QFrame::Raised );
  this->setLineWidth( 2 );

  m_panelFont = panelFonts; // sets font / our selected font
  m_panelBoldFont = panelFonts; // will be changed later
    
  m_panelBoldFont.setBold (true);
  m_panelFont.setBold (false); // sets no bold

  m_mainLayout = new QVBoxLayout(this);
  m_mainLayout->setMargin ( 10 );
  m_mainLayout->setSpacing ( 10 );

  // layer for bulbs

  m_bulbsGroupBox = new QGroupBox(this);
  m_mainLayout->addWidget(m_bulbsGroupBox);
  QVBoxLayout* bulbsLayout = new QVBoxLayout(m_bulbsGroupBox);
  bulbsLayout->setMargin( 5 );


  m_textLabel1 = new QLabel( i18n("UPS On line"),m_bulbsGroupBox);
  m_frameOL = new KLed (m_upsBackColor,KLed::Off,KLed::Sunken,KLed::Circular,m_bulbsGroupBox);
  m_frameOL->setMaximumSize(30,30);
  m_frameOL->setMinimumSize(25,25);
  m_textLabel1->setToolTip(m_textLabel1->text());

  QHBoxLayout* OLLayout = new QHBoxLayout;
  OLLayout->addWidget(m_textLabel1);
  OLLayout->addSpacing(10); 
  OLLayout->addWidget(m_frameOL);
  OLLayout->setMargin (3);
  OLLayout->setStretchFactor(m_textLabel1,20);
  OLLayout->setStretchFactor(m_frameOL,5);
  m_textLabel2 = new QLabel( i18n("UPS On battery"), m_bulbsGroupBox);
  m_frameOB = new KLed (m_upsBackColor,KLed::Off,KLed::Sunken,KLed::Circular,m_bulbsGroupBox);
  m_frameOB->setMaximumSize(30,30);
  m_frameOB->setMinimumSize(25,25);
  m_textLabel2->setToolTip(m_textLabel2->text());
  bulbsLayout->addLayout(OLLayout);

  QHBoxLayout* OBLayout = new QHBoxLayout;
  OBLayout->addWidget(m_textLabel2);
  OBLayout->addSpacing(10); 
  OBLayout->addWidget(m_frameOB);
  OBLayout->setMargin (3);
  OBLayout->setStretchFactor(m_textLabel2,20);
  OBLayout->setStretchFactor(m_frameOB,5);
  bulbsLayout->addLayout(OBLayout);

  m_textLabel3 = new QLabel( i18n("UPS Overload"), m_bulbsGroupBox);
  m_frameOVER = new  KLed (m_upsBackColor,KLed::Off,KLed::Sunken,KLed::Circular,m_bulbsGroupBox);
  m_frameOVER->setMaximumSize(30,30);
  m_frameOVER->setMinimumSize(25,25);
  m_textLabel3->setToolTip(m_textLabel3->text());

  m_OLayout = new QHBoxLayout();
  m_OLayout->addWidget(m_textLabel3);
  m_OLayout->addSpacing(10); 
  m_OLayout->addWidget(m_frameOVER);
  m_OLayout->setMargin (3);
  m_OLayout->setStretchFactor(m_textLabel3,20);
  m_OLayout->setStretchFactor(m_frameOVER,5);
  bulbsLayout->addLayout(m_OLayout);

  m_textLabel4 = new QLabel( i18n("UPS Battery low"), m_bulbsGroupBox);
  m_frameLB = new  KLed (m_upsBackColor,KLed::Off,KLed::Sunken,KLed::Circular,m_bulbsGroupBox);
  m_frameLB->setMaximumSize(30,30);
  m_frameLB->setMinimumSize(25,25);
  m_textLabel4->setToolTip(m_textLabel4->text());

  m_LBLayout = new QHBoxLayout();
  m_LBLayout->addWidget(m_textLabel4);
  m_LBLayout->addSpacing(10); 
  m_LBLayout->addWidget(m_frameLB);
  m_LBLayout->setMargin (3);
  m_LBLayout->setStretchFactor(m_textLabel4,20);
  m_LBLayout->setStretchFactor(m_frameLB,5);
  bulbsLayout->addLayout(m_LBLayout);  

  m_textLabel5 = new QLabel( i18n("Replace battery"), m_bulbsGroupBox);
  m_frameRB = new  KLed (m_upsBackColor,KLed::Off,KLed::Sunken,KLed::Circular,m_bulbsGroupBox);
  m_frameRB->setMaximumSize(30,30);
  m_frameRB->setMinimumSize(25,25);
  m_textLabel5->setToolTip(m_textLabel5->text());

  m_RBLayout = new QHBoxLayout();
  m_RBLayout->addWidget(m_textLabel5);
  m_RBLayout->addSpacing(10); 
  m_RBLayout->addWidget(m_frameRB);
  m_RBLayout->setMargin (3);
  m_RBLayout->setStretchFactor(m_textLabel5,20);
  m_RBLayout->setStretchFactor(m_frameRB,5);
  bulbsLayout->addLayout(m_RBLayout);

  m_textLabel6 = new QLabel( i18n("UPS calibration"), m_bulbsGroupBox);
  m_frameCAL = new  KLed (m_upsBackColor,KLed::Off,KLed::Sunken,KLed::Circular,m_bulbsGroupBox);
  m_frameCAL->setMaximumSize(30,30);
  m_frameCAL->setMinimumSize(25,25);
  m_textLabel6->setToolTip(m_textLabel6->text());

  m_CALLayout = new QHBoxLayout();
  m_CALLayout->addWidget(m_textLabel6);
  m_CALLayout->addSpacing(10); 
  m_CALLayout->addWidget(m_frameCAL);
  m_CALLayout->setMargin (3);
  m_CALLayout->setStretchFactor(m_textLabel6,20);
  m_CALLayout->setStretchFactor(m_frameCAL,5);
  bulbsLayout->addLayout(m_CALLayout);

  m_messGroupBox = new QGroupBox(this);
  m_mainLayout->addWidget(m_messGroupBox);
  QVBoxLayout* messLayout = new QVBoxLayout(m_messGroupBox);
  messLayout->setMargin( 3 );


  m_textLabel7 = new QLabel( "", m_messGroupBox);
  m_textLabel7->setToolTip(m_textLabel7->text());

  m_manufacModelLayout = new QHBoxLayout();
  m_manufacModelLayout->addWidget(m_textLabel7);
  m_manufacModelLayout->setMargin ( 3 );
  messLayout->addLayout(m_manufacModelLayout);

  m_textLabelManufac = new QLabel( i18n("Manufac. : "), m_messGroupBox);
  m_textLabel8 = new QLabel( "", m_messGroupBox);
  m_textLabelManufac->setToolTip(m_textLabelManufac->text());

  m_manufacLayout = new QHBoxLayout();
  m_manufacLayout->addWidget(m_textLabelManufac);
  m_manufacLayout->addWidget(m_textLabel8);
  m_manufacLayout->addStretch(20);
  m_manufacLayout->setMargin ( 3 );
  messLayout->addLayout(m_manufacLayout);

  m_textLabelName = new QLabel( i18n("Name")+" : ", m_messGroupBox);
  m_textLabel9 = new QLabel( "", m_messGroupBox);
  m_textLabelName->setToolTip(m_textLabelName->text());

  m_nameLayout = new QHBoxLayout();
  m_nameLayout->addWidget(m_textLabelName);
  m_nameLayout->addWidget(m_textLabel9);
  m_nameLayout->addStretch(20);
  m_nameLayout->setMargin ( 3 );
  messLayout->addLayout(m_nameLayout);

  m_textLabelSerial = new QLabel( i18n("Serial : "), m_messGroupBox);
  m_textLabel10 = new QLabel( "", m_messGroupBox);
  m_textLabelSerial->setToolTip(m_textLabelSerial->text());

  m_serialLayout = new QHBoxLayout();
  m_serialLayout->addWidget(m_textLabelSerial);
  m_serialLayout->addWidget(m_textLabel10);
  m_serialLayout->addStretch(20);
  m_serialLayout->setMargin ( 3 );
  messLayout->addLayout(m_serialLayout);

  m_textLabelFirm = new QLabel( i18n("Firm. rev. : "), m_messGroupBox);
  m_textLabel11 = new QLabel( "", m_messGroupBox);
  m_textLabelFirm->setToolTip(m_textLabelFirm->text());

  m_firmLayout = new QHBoxLayout();
  m_firmLayout->addWidget(m_textLabelFirm);
  m_firmLayout->addWidget(m_textLabel11);
  m_firmLayout->addStretch(20);
  m_firmLayout->setMargin ( 3 );
  messLayout->addLayout(m_firmLayout);  
  
  m_valGroupBox = new QGroupBox(this);
  m_mainLayout->addWidget(m_valGroupBox);
  QVBoxLayout* valLayout = new QVBoxLayout(m_valGroupBox);
  valLayout->setMargin( 3 );
  m_textLabelRun = new QLabel( i18n("Runtime")+" : ", m_valGroupBox);
  m_textLabelRun->setToolTip(m_textLabelRun->text());

  m_LCDNumber1 = new QLCDNumber (5,m_valGroupBox);
  m_LCDNumber1->setSegmentStyle (QLCDNumber::Flat);
  m_LCDNumber1->setFrameStyle( QFrame::NoFrame );
  m_LCDNumber1->setMinimumSize(50,20);

  m_runLayout = new QHBoxLayout();
  m_runLayout->addWidget(m_textLabelRun);
  m_runLayout->addStretch(20);
  m_runLayout->addWidget(m_LCDNumber1);
  m_runLayout->setMargin ( 5 );

  m_mainLayout->addStretch(20);
  m_mainLayout->addSpacing(5);
  valLayout->addLayout(m_runLayout);

  setAutoFillBackground(true);

  setAllPalettes();

  changePanelFont();
  paintPanel ();

  setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Preferred);
  }

KNutFrontPanel::~KNutFrontPanel(){
  }


//************************************************************

void KNutFrontPanel::paintPanel( void ) {

  if (m_panelFlags & PBOnLine) {
    m_textLabel1->show();
    m_frameOL->show();
    }
  else {
    m_textLabel1->hide();
    m_frameOL->hide();
    }

  if (m_panelFlags & PBOnBatt) {
    m_textLabel2->show();
    m_frameOB->show();
    }
  else {
    m_textLabel2->hide();
    m_frameOB->hide();
    }

  if (m_panelFlags & PBOver) {
    m_OLayout->setMargin ( 3 );
    m_textLabel3->show();
    m_frameOVER->show();
    }
  else {
    m_textLabel3->hide();
    m_frameOVER->hide();
    m_OLayout->setMargin ( 0 );
    }

  if (m_panelFlags & PBBattLow) {
    m_textLabel4->show();
    m_frameLB->show();
    m_LBLayout->setMargin ( 3 );
    }
  else {
    m_textLabel4->hide();
    m_frameLB->hide();
    m_LBLayout->setMargin ( 0 );
    }

  if (m_panelFlags & PBRepBatt) {
    m_textLabel5->show();
    m_frameRB->show();
    m_RBLayout->setMargin ( 3 );
    }
  else {
    m_textLabel5->hide();
    m_frameRB->hide();
    m_RBLayout->setMargin ( 0 );
    }

  if (m_panelFlags & PBCal) {
    m_textLabel6->show();
    m_frameCAL->show();
    m_CALLayout->setMargin ( 3 );
    }
  else {
    m_textLabel6->hide();
    m_frameCAL->hide();
    m_CALLayout->setMargin ( 0 );
    }

  if (m_panelFlags & PBMFRMod) {
    m_textLabel7->show();
    m_manufacModelLayout->setMargin ( 5 );
    }
  else {
    m_textLabel7->hide();
    m_manufacModelLayout->setMargin ( 0 );
    }


  if (m_panelFlags & PBMFR) {
    m_textLabel8->show();
    m_textLabelManufac->show();
    m_manufacLayout->setMargin ( 5 );
    }
  else {
    m_textLabel8->hide();
    m_textLabelManufac->hide();
    m_manufacLayout->setMargin ( 0 );
    }

  if (m_panelFlags & PBModel) {
    m_textLabel9->show();
    m_textLabelName->show();
    m_nameLayout->setMargin ( 5 );
    }
  else {
    m_textLabel9->hide();
    m_textLabelName->hide();
    m_nameLayout->setMargin ( 0 );
    }

  if (m_panelFlags & PBSerial) {
    m_textLabel10->show();
    m_textLabelSerial->show();
    m_serialLayout->setMargin ( 5 );
    }
  else {
    m_textLabel10->hide();
    m_textLabelSerial->hide();
    m_serialLayout->setMargin ( 0 );
    }

  if (m_panelFlags & PBFirm) {
    m_textLabel11->show();
    m_textLabelFirm->show();
    m_firmLayout->setMargin ( 5 );
    }
  else {
    m_textLabel11->hide();
    m_textLabelFirm->hide();
    m_firmLayout->setMargin ( 0 );
    }

  if ( (m_panelFlags & PBMFRMod) || (m_panelFlags & PBMFR) || (m_panelFlags & PBModel) || (m_panelFlags & PBSerial) ||(m_panelFlags & PBFirm))    
    m_messGroupBox->show();
  else {
    m_messGroupBox->hide();
  }

  if (m_panelFlags & PBRun) {
    m_valGroupBox->show();
    }
  else {
    m_valGroupBox->hide();
    }
  }


unsigned int KNutFrontPanel::getPanelFlags( void) { return m_panelFlags; }

void KNutFrontPanel::setPanelFlags(const int upsPanelFlags) { m_panelFlags = upsPanelFlags; }

void KNutFrontPanel::setAllNameUps(const QString panelUpsAllName ){
  m_textLabel7->setText(panelUpsAllName);
  //QToolTip::remove(m_textLabel7);
  m_textLabel7->setToolTip(panelUpsAllName);
  }

void KNutFrontPanel::setMFRUps(const QString panelUpsFRM ){
  m_textLabel8->setText(panelUpsFRM);
  //QToolTip::remove(m_textLabelManufac);
  m_textLabelManufac->setToolTip(m_textLabelManufac->text()+panelUpsFRM);
  }

void KNutFrontPanel::setNameUps(const QString panelUpsName ){
  m_textLabel9->setText(panelUpsName);
  //QToolTip::remove(m_textLabelName);
  m_textLabelName->setToolTip(m_textLabelName->text()+panelUpsName);
  }

void KNutFrontPanel::setSerialUps(const QString panelUpsSerial ){
  m_textLabel10->setText(panelUpsSerial);
  //QToolTip::remove(m_textLabelSerial);
  m_textLabelSerial->setToolTip(m_textLabelSerial->text()+panelUpsSerial);
  }

void KNutFrontPanel::setFirmRevUps(const QString upsPanelFormRev ){
  m_textLabel11->setText(upsPanelFormRev);
  //QToolTip::remove(m_textLabelFirm);
  m_textLabelFirm->setToolTip(m_textLabelFirm->text()+upsPanelFormRev);
  }


void KNutFrontPanel::cleanPanel(void) {
  m_textLabel7->setText("");
  m_textLabel8->setText("");
  m_textLabel9->setText("");
  m_textLabel10->setText("");
  m_textLabel11->setText("");

  m_frameOL->setColor(m_upsBackColor);
  m_frameOL->off();
  m_frameOB->setColor(m_upsBackColor);
  m_frameOB->off();
  m_frameOVER->setColor(m_upsBackColor);
  m_frameOVER->off();
  m_frameLB->setColor(m_upsBackColor);
  m_frameLB->off();
  m_frameRB->setColor(m_upsBackColor);
  m_frameRB->off();
  m_frameCAL->setColor(m_upsBackColor);
  m_frameCAL->off();

  m_LCDNumber1->display("-:--");
  m_LCDNumber1->setPalette (palette());
  }

void KNutFrontPanel::setRuntime(const int runtime) {
  QString runtimeString;

  if (runtime != knc::numberVarError) {
    int min = (int)floor(runtime/60);
    int sec = runtime - (min*60);
    runtimeString=QString("%1:%2").arg(min,2).arg(sec,2);
    if (runtimeString[3] == ' ') runtimeString[3]='0';
    }
  else runtimeString="-:--";  

  m_LCDNumber1->display(runtimeString);
  }

  

void KNutFrontPanel::setPanel(const int panelBulbs) {
//  OFF			- 1 ups je vypnuta
//  OL			- 2 ups je bezi na sit
//  OB			- 4 ups bezi na baterie
//  LB			- 8 baterie je vybyta (pokud je zaroven OB dojde k shutdownu)
//  CAL			- 16 je spustena calibrace
//  OVER		- 128 ups je pretizena
//  RB			- 256 ups pozaduje vymenu baterie


  if (!(panelBulbs < 0)) {
    // ups online /na sit
    if (panelBulbs & KNutNet::OL) {
      m_frameOL->setColor(m_upsOkColor);
      m_frameOL->on();
      m_LCDNumber1->setPalette (m_LCDPaletteOk);
      m_typeOfLCDColor=true;
      }
    else {
      m_frameOL->setColor(m_upsBackColor);
      m_frameOL->off();
      }

    // ups ofline /na baterie
    if (panelBulbs & KNutNet::OB) {
      m_frameOB->setColor(m_upsErrorColor);
      m_frameOB->on();
      m_LCDNumber1->setPalette (m_LCDPaletteErr);
      m_typeOfLCDColor=false;
      }
    else {
      m_frameOB->setColor(m_upsBackColor);
      m_frameOB->off();
      }

    if (panelBulbs & KNutNet::LB) {
      m_frameLB->setColor(m_upsErrorColor);
      m_frameLB->on();
      }
    else {
      m_frameLB->setColor(m_upsBackColor);
      m_frameLB->off();
      }

    if (panelBulbs & KNutNet::OVER) {
      m_frameOVER->setColor(m_upsErrorColor);
      m_frameOVER->on();
      }
    else {
      m_frameOVER->setColor(m_upsBackColor);
      m_frameOVER->off();
      }

    if (panelBulbs & KNutNet::RB) {
      m_frameRB->setColor(m_upsErrorColor);
      m_frameRB->on();
      }
    else {
      m_frameRB->setColor(m_upsBackColor);
      m_frameRB->off();
      }

    if (panelBulbs & KNutNet::CAL) {
      m_frameCAL->setColor(m_upsErrorColor);
      m_frameCAL->on();
      }
    else {
      m_frameCAL->setColor(m_upsBackColor);
      m_frameCAL->off();
      }
    }
  }

  
/*********************************************************************************************/
/*                                                                                           */
/*                         PUBLIC SLOTS                                                      */
/*                                                                                           */
/*********************************************************************************************/
  
  
void KNutFrontPanel::slotSetPanelFont(QFont newPanelFont) {

  m_panelFont = newPanelFont; // nastavime font
  m_panelBoldFont = newPanelFont; // pozdeji zmenime

  m_panelBoldFont.setBold (true);
  m_panelFont.setBold (false); // Zrusime bold u panelu

  changePanelFont(); // zmeni pismo
  paintPanel();
  }

  
/*********************************************************************************************/
/*                                                                                           */
/*                         PRIVATE FUNCTIONS                                                 */
/*                                                                                           */
/*********************************************************************************************/
  
  
void KNutFrontPanel::setLCDPalette (void) {
  m_LCDPaletteOk = m_palette;
  m_LCDPaletteErr = m_palette;

  m_LCDPaletteOk.setColor(QPalette::Active,QPalette::Foreground,Qt::green);
  m_LCDPaletteOk.setColor(QPalette::Inactive,QPalette::Foreground,Qt::green);
  m_LCDPaletteErr.setColor(QPalette::Active,QPalette::Foreground,Qt::red);
  m_LCDPaletteErr.setColor(QPalette::Inactive,QPalette::Foreground,Qt::red);
  }

void KNutFrontPanel::setPalette (const QPalette & p){
  m_palette = p;
  setAllPalettes();
  }

      
void KNutFrontPanel::setAllPalettes (void){
  QColor myColor;

  QFrame::setPalette (m_palette);
  setLCDPalette();
  
  m_bulbsPalette = m_palette;

  m_frameOL->setPalette(m_bulbsPalette);

  // we must correct feature of KLed, KLed repaint background when color of item (function KLed::setColor()) is changed only
  // when we change palette backround only, KLed ignore this change
    
  myColor = m_frameOL->color();
  m_frameOL->setColor(m_helpColor); // black is neutral (unprofitable) color for us 
  m_frameOL->setColor(myColor); 

  myColor = m_frameOB->color();
  m_frameOB->setColor(m_helpColor);
  m_frameOB->setColor(myColor);
  
  myColor = m_frameLB->color();
  m_frameLB->setColor(m_helpColor);
  m_frameLB->setColor(myColor);

  myColor = m_frameOVER->color();
  m_frameOVER->setColor(m_helpColor);
  m_frameOVER->setColor(myColor);

  myColor = m_frameRB->color();
  m_frameRB->setColor(m_helpColor);
  m_frameRB->setColor(myColor);

  myColor = m_frameCAL->color();
  m_frameCAL->setColor(m_helpColor);
  m_frameCAL->setColor(myColor);
        
  
  if (m_LCDNumber1) {
    if (m_typeOfLCDColor)
      m_LCDNumber1->setPalette (m_LCDPaletteOk);
    else
      m_LCDNumber1->setPalette (m_LCDPaletteErr);
    }
  }

void KNutFrontPanel::changePanelFont (void) {
  m_textLabel1->setFont(m_panelFont);
  m_textLabel2->setFont(m_panelFont);
  m_textLabel3->setFont(m_panelFont);
  m_textLabel4->setFont(m_panelFont);
  m_textLabel5->setFont(m_panelFont);
  m_textLabel6->setFont(m_panelFont);

  m_textLabelManufac->setFont(m_panelFont);
  m_textLabelName->setFont (m_panelFont);
  m_textLabelSerial->setFont (m_panelFont);
  m_textLabelFirm->setFont (m_panelFont);
  m_textLabelRun->setFont (m_panelFont);
  m_textLabel7->setFont (m_panelBoldFont);
  m_textLabel8->setFont (m_panelBoldFont);
  m_textLabel9->setFont (m_panelBoldFont);
  m_textLabel10->setFont (m_panelBoldFont);
  m_textLabel11->setFont (m_panelBoldFont);
  m_textLabelRun->setFont (m_panelFont);
  }

  
#include "moc_knutfrontpanel.cpp"  
