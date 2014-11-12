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
#include <kapplication.h>
#include <kcmdlineargs.h>
#include <KDE/KLocale>
#include <KDE/KAboutData>
#include <KDE/KUniqueApplication>
#include <stdio.h>

static const char description[] =
    I18N_NOOP("Client for Network UPS Tool");

static const char version[] = "1.0.5";

int main(int argc, char **argv)
{
  KAboutData about("knutclient", 0, ki18n("KNutClient"), version, ki18n(description),
                     KAboutData::License_GPL, ki18n("(C) 2002 - 2013  Daniel Prynych"), 
                     ki18n("Zajecov, Czech Republic"), "http://www.knut.prynych.cz", "Daniel@prynych.cz");

  about.addAuthor( ki18n("Daniel Prynych"), KLocalizedString(), "Daniel@prynych.cz", "http://www.knut.cz/" );

  about.addCredit(ki18n("Arnaud Quette"),KLocalizedString(),"arnaud.quette@free.fr","http://www.networkupstools.org/");
  about.addCredit( ki18n("MGE"),KLocalizedString(),0,"http://opensource.mgeups.com");

  KCmdLineArgs::init(argc, argv, &about);
  KCmdLineOptions options;
  options.add("nodock", ki18n( "Don't dock in SysTray" ));
  KCmdLineArgs::addCmdLineOptions( options ); // Add our own options.

  KUniqueApplication::addCmdLineOptions();
  KCmdLineArgs* args = KCmdLineArgs::parsedArgs();
  bool noDock = args->isSet("dock");

 // main KUApp this is remark
  if (!KUniqueApplication::start()) {
    fprintf(stderr,"Sorry : KNutClient is already running!\n");
    exit(0);
    }
  KUniqueApplication app;

//  next line is for testing only.
//  I don't debug KUniqueApplication

//  KApplication app;
//
  KNutClient *knutclient = new KNutClient(!noDock);

  args->clear();

  int returnCode = app.exec();
  delete knutclient;
  return returnCode;
}
