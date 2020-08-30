/***************************************************************************
 *   Copyright (C) 2009 by Daniel Prynych,,,   *
 *   Daniel@prynych.cz   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include "knutclient.h"

#include <KLocalizedString>
#include <KAboutData>

#include <QApplication>
#include <QCommandLineParser>

#include <stdio.h>

static const char description[] =
    I18N_NOOP("Client for Network UPS Tool");

static const char version[] = "1.1.0";

int main(int argc, char **argv)
{

QApplication app(argc, argv);

KAboutData about(QStringLiteral( "knutclient"), i18n("KNutClient"), version ,
                     i18n( description ),
                     KAboutLicense::LGPL,
                     i18n("Copyright 2002 - 2020 Daniel Prynych"),
		     i18n("Zajecov, Czech Republic"),
                     QStringLiteral("http://www.knut.prynych.cz"),
		     QStringLiteral("Daniel@prynych.cz"));


  about.addAuthor( i18n("Daniel Prynych"), QString(), "Daniel@prynych.cz", "http://www.knut.cz/" );

  about.addCredit(i18n("Arnaud Quette"),QString(),"arnaud.quette@free.fr","https://networkupstools.org");
  about.addCredit( i18n("MGE"),QString(),0,"https://www.power-solutions.com/ups/mgeups");
  about.addCredit( i18n("BUZULUK a.s."),QString(),0,"http://www.buzuluk.eu");


  KAboutData::setApplicationData(about);


  QCommandLineParser options;
  options.addHelpOption();
  options.addVersionOption();

  QCommandLineOption noDockOption(QStringList() << "d" << "nodock", i18n("Don't dock in SysTray")); // QCoreApplication::translate("main", "Don't dock in SysTray"));
  options.addOption(noDockOption);


  about.setupCommandLine(&options);
  options.process(app);

  about.processCommandLine(&options);

  bool noDock = options.isSet(noDockOption);

  KNutClient *knutclient = new KNutClient(noDock);

  int returnCode = app.exec();
  delete knutclient;
  return returnCode;
}
