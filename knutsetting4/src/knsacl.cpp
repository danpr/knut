#include "knsacl.h"

#include <klocale.h>


#include <QtGui/QLayout>
#include <QtCore/QString>
#include <QtGui/QCheckBox>


KNSAcl::KNSAcl(KNSCommonData* commonData, QWidget *parent)
 : QLabel(parent), m_commonData(commonData) {

  KPushButton* standardButton;

  QVBoxLayout* topLayout = new QVBoxLayout(this);

  m_listenListWidget = new KNSACLAddress(m_commonData ,this);
  topLayout->addWidget(m_listenListWidget);
 

  QGroupBox* thirdWidget = new QGroupBox(this);

  QVBoxLayout* topAgeLayout = new QVBoxLayout(thirdWidget);
  QHBoxLayout* ageLayout = new QHBoxLayout();
  topAgeLayout->addLayout(ageLayout);
  QLabel* label1 = new QLabel (i18n("Maximal time for responding of driver (MAXAGE)")+" :",thirdWidget);
  ageLayout->addWidget ( label1 );
  
  QGroupBox* maxConnWidget = new QGroupBox(this);
  QVBoxLayout* topMaxConnLayout = new QVBoxLayout(maxConnWidget);
  QHBoxLayout* maxConnLayout = new QHBoxLayout();
  topMaxConnLayout->addLayout(maxConnLayout);
  QLabel* maxConnLabel = new QLabel (i18n("Maximal number allowed connections for ups system (MAXCONN)")+" :",maxConnWidget);
  maxConnLayout->addWidget ( maxConnLabel );


  m_maxAgeEdit =  new KIntNumInput(thirdWidget);
  m_maxAgeEdit->setMinimum(1);


  connect (m_maxAgeEdit, SIGNAL(valueChanged(int)), this, SLOT(maxAgeChangedSlot(int)));
  ageLayout->addWidget ( m_maxAgeEdit );

  m_maxAgeUsed = new QCheckBox(thirdWidget);
  ageLayout->addWidget ( m_maxAgeUsed );
  connect (m_maxAgeUsed, SIGNAL(toggled (bool)), this, SLOT (maxAgeCheckedSlot(bool)));
  m_maxAgeEdit->setEnabled(m_maxAgeUsed->isChecked());

  ageLayout->addStretch ( 10 );


  m_maxConnEdit =  new KIntNumInput(maxConnWidget);
  m_maxConnEdit->setMinimum(1);
  connect (m_maxConnEdit, SIGNAL(valueChanged(int)), this, SLOT(maxConnChangedSlot(int)));
  maxConnLayout->addWidget ( m_maxConnEdit );

  m_maxConnUsed = new QCheckBox(maxConnWidget);
  maxConnLayout->addWidget ( m_maxConnUsed );
  connect (m_maxConnUsed, SIGNAL(toggled (bool)), this, SLOT (maxConnCheckedSlot(bool)));
  m_maxConnEdit->setEnabled(m_maxConnUsed->isChecked());
  maxConnLayout->addStretch ( 10 );


  topLayout->addWidget(thirdWidget);
  topLayout->addWidget(maxConnWidget);


  QHBoxLayout* buttonLayout = new QHBoxLayout ();
  topLayout->addLayout(buttonLayout);
  standardButton = new KPushButton (i18n("Default of access"),this);

  buttonLayout->addStretch( 10 );
  buttonLayout->addWidget(standardButton);

  topLayout->addStretch ( 20 );


  connect (m_listenListWidget,SIGNAL(standardSetted()),this,SLOT(correctAccessSlot()));
  connect (m_listenListWidget,SIGNAL(changed()),this,SLOT(sendChangedSlot()));

  connect (standardButton,SIGNAL(clicked()),this,SLOT(setStandardSlot()));


  }


KNSAcl::~KNSAcl() {
  }

//-------------Public Functions-------------------------

void KNSAcl::loadItems( void ) {

  ACDataStruct* ACData;

  ACData = m_commonData->ACDataAddr();

  m_listenListWidget->loadItems();

  disconnect (m_maxAgeEdit, SIGNAL(valueChanged(int)), this, SLOT(maxAgeChangedSlot(int)));
  m_maxAgeEdit->setValue(ACData->maxAge);
  if (ACData->maxAgeFound) {
    m_maxAgeUsed->setChecked(true);
    m_maxAgeEdit->setEnabled(true);
    }
  else {
    m_maxAgeUsed->setChecked(false);
    m_maxAgeEdit->setEnabled(false);
    }
  connect (m_maxAgeEdit, SIGNAL(valueChanged(int)), this, SLOT(maxAgeChangedSlot(int)));


  disconnect (m_maxConnEdit, SIGNAL(valueChanged(int)), this, SLOT(maxConnChangedSlot(int)));
  m_maxConnEdit->setValue(ACData->maxConn);
  if (ACData->maxConnFound) {
    m_maxConnUsed->setChecked(true);
    m_maxConnEdit->setEnabled(true);
    }
  else {
    m_maxConnUsed->setChecked(false);
    m_maxConnEdit->setEnabled(false);
    }
  connect (m_maxConnEdit, SIGNAL(valueChanged(int)), this, SLOT(maxConnChangedSlot(int)));


  }



//-----------PRIVATE SLOTS ---------------------


void KNSAcl::maxAgeChangedSlot (int) {
  ACDataStruct* ACData;

  ACData = m_commonData->ACDataAddr();
  ACData->maxAge=m_maxAgeEdit->value();
  emit changed();
  }


void KNSAcl::maxAgeCheckedSlot ( bool ) {
  ACDataStruct* ACData;

  ACData = m_commonData->ACDataAddr();
  ACData->maxAgeFound=m_maxAgeUsed->isChecked();
  m_maxAgeEdit->setEnabled(m_maxAgeUsed->isChecked());
  emit changed();
  }



void KNSAcl::maxConnChangedSlot (int) {
  ACDataStruct* ACData;

  ACData = m_commonData->ACDataAddr();
  ACData->maxConn=m_maxConnEdit->value();
  emit changed();
  }



void KNSAcl::maxConnCheckedSlot ( bool ) {
  ACDataStruct* ACData;

  ACData = m_commonData->ACDataAddr();
  ACData->maxConnFound=m_maxConnUsed->isChecked();
  m_maxConnEdit->setEnabled(m_maxConnUsed->isChecked());
  emit changed();
  }


void KNSAcl::correctAccessSlot (void) {

  emit addressChanged();
  }


void KNSAcl::sendChangedSlot (void) {

  emit changed();
  }


//--------PUBLIC SLOTS-----------------------

void KNSAcl::setStandardSlot (void) {

  m_listenListWidget->standardButtonSlot();
// sets MaxAge
  m_maxAgeEdit->setValue(maxAgeDef);
  m_maxAgeUsed->setChecked(false);

  m_maxConnEdit->setValue(maxConnDef);
  m_maxConnUsed->setChecked(false);

  emit addressChanged();
  emit changed();
  }

#include "knsacl.moc"

