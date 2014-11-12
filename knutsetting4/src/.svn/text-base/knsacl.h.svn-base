#ifndef KNSACL_H
#define KNSACL_H

#include "knscommondata.h"
#include "knsacladdress.h"


#include <KPushButton>
#include <KNumInput>
#include <QtGui/QLabel>


/**
     @author Daniel Prynych,Zajecov,Czech <Daniel@prynych.cz>
*/

class QString;
class QCheckBox;



class KNSAcl : public QLabel
{
Q_OBJECT
public:
  KNSAcl(KNSCommonData* commonData, QWidget *parent=0);

  ~KNSAcl();

  void loadItems( void );

public slots:

/**
 * Nastavy standartni hodnotu do widgetu grafickeho rozhrani a posle signal changed
 * Sets standars value to GUI widgets and send signal changed
 *
 * @since  0.1
 **/
  void setStandardSlot (void);

signals:

/**
 * signal je emitovan, kdyz nejaka zmena byla udelana
 * signal is emited, when same change was made
 *
 * @since  0.1
 **/
  void changed( void);

/**
 * signal je emitovan kdyz byla pridana nebo odebrana nejaka adresa
 * signal is emited when same address was added or deleted
 *
 * @since  0.1
 **/
  void addressChanged (void);

/**
 * signal je emitovan kdyz bylo prejmenovano jmeno adresy
 * signal is emited when name of address was renamed.
 *
 * @since  0.1
 **/
  void addressRenamed(const QString oldName, const QString name);

private slots:


/**
 * Emituje signal changed pokud je zmenen udaj v maxAgeEdit.
 * Emites signal changed when value of maxAgeEdit is changed.
 *
 * @since  0.1
 **/
  void maxAgeChangedSlot (int);


/**
 * Emituje signal changed pokud je maxAgeUsed zaskrtnut/odskrtnut.
 * Emites signal changed when maxAgeUsed is checked/unchecked.
 *
 * @since  0.1
 **/
  void maxAgeCheckedSlot ( bool );


/**
 * Emituje signal changed pokud je zmenen udaj v maxConnEdit.
 * Emites signal changed when value of maxConnEdit is changed.
 *
 * @since  0.1
 **/
  void maxConnChangedSlot (int);


/**
 * Emituje signal changed pokud je maxConnUsed zaskrtnut/odskrtnut.
 * Emites signal changed when maxConnUsed is checked/unchecked.
 *
 * @since  0.1
 **/
  void maxConnCheckedSlot ( bool );


/**
 * Opravi pristupu podle seznamu adres
 * Corrects accesses by address list
 *
 * @since  0.1
 **/
  void correctAccessSlot (void);

/**
 * Emituje signal changed.
 * Emites signal changed.
 *
 * @since  0.1
 **/
  void sendChangedSlot (void);





private:

  KNSACLAddress* m_listenListWidget;


  KNSCommonData* m_commonData;
  bool m_enableButton;

  KIntNumInput* m_maxAgeEdit;
  QCheckBox* m_maxAgeUsed;


  KIntNumInput* m_maxConnEdit;
  QCheckBox* m_maxConnUsed;


//---------------Functions----------------------------
//  bool addressIsExist (QString address, QString name);


};

#endif
