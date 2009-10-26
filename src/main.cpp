/***************************************************************************
*   Copyright © 2003-2005 Unai Garro <ugarro@users.sourceforge.net>       *
*   Copyright © 2003-2005 Jason Kivlighn <jkivlighn@gmail.com>            *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/

#include "krecipes.h"

#include <iostream>

#include <kuniqueapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>

#include "convert_sqlite3.h"

static const char *version = "2.0-alpha4";

static KCmdLineOptions options;

int main( int argc, char **argv )
{
	options.add("convert-sqlite3", ki18n("Convert the current SQLite 2.x database to SQLite 3 and exit") , 0 );
	options.add( 0, KLocalizedString(), 0 );    


	KAboutData about( "krecipes", 0, ki18n( "Krecipes" ), version, ki18n( "The KDE Cookbook" ), KAboutData::License_GPL, ki18n( "(C) 2003 Unai Garro\n(C) 2004-2006 Jason Kivlighn"), ki18n("This product is RecipeML compatible.\nYou can get more information about this file format in:\nhttp://www.formatdata.com/recipeml" ), "http://krecipes.sourceforge.net/", "panfaust@gmail.com" );
	about.addAuthor( ki18n("Unai Garro"), KLocalizedString(), "ugarro@users.sourceforge.net", 0 );
	about.addAuthor( ki18n("Jason Kivlighn"), KLocalizedString(), "jkivlighn@gmail.com", 0 );
	about.addAuthor( ki18n("Cyril Bosselut"), KLocalizedString(), "bosselut@b1project.com", "http://b1project.com" );

	about.addCredit( ki18n("Colleen Beamer"), ki18n("Testing, bug reports, suggestions"), "colleen.beamer@gmail.com", 0 );
	about.addCredit( ki18n("Robert Wadley"), ki18n("Icons and artwork"), "rob@robntina.fastmail.us", 0 );
	about.addAuthor( ki18n("Daniel Sauvé"), ki18n("Porting to KDE4"), "megametres@gmail.com", "http://metres.homelinux.com" );

        about.addAuthor( ki18n("Laurent Montel"), ki18n("Porting to KDE4"), "montel@kde.org", 0 );
        about.addAuthor( ki18n("José Manuel Santamaría Lema"), ki18n("Porting to KDE4, current maintainer"), "panfaust@gmail.com", 0 );

	about.setTranslator( ki18n( "INSERT YOUR NAME HERE" ), ki18n( "INSERT YOUR EMAIL ADDRESS" ) );
	KCmdLineArgs::init( argc, argv, &about );
	KCmdLineArgs::addCmdLineOptions( options );
	KUniqueApplication::addCmdLineOptions();

	if ( !KUniqueApplication::start() ) {
		std::cerr << "Krecipes is already running!" << std::endl;
		return 0;
	}

	KUniqueApplication app;

	// see if we are starting with session management
	if ( app.isSessionRestored() ) {
		RESTORE( Krecipes );
	}
	else {
		// no session.. just start up normally
		KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

		QApplication::flush();

		if ( args->isSet("convert-sqlite3") ) {
			ConvertSQLite3 sqliteConverter;
			sqliteConverter.convert();
			return 0;
		}

		Krecipes * widget = new Krecipes;
		app.setTopWidget( widget );
		widget->show();

		args->clear();
	}

	return app.exec();
}

