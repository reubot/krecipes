/***************************************************************************
*   Copyright © 2004 Jason Kivlighn <jkivlighn@gmail.com>                 *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/

#include "unit.h"

Unit::Unit() : type(Unit::Other), id( -1 )
{}

Unit::Unit( const QString &_name, const QString &_plural, int _id ) :
		type(Unit::Other),
		id( _id ),
		name( _name ),
		plural( _plural )
{}

Unit::Unit( const QString &_name, double amount ) : type(Unit::Other)
{
	if ( amount > 1 )
		plural = _name;
	else
		name = _name;
}

QString Unit::determineName( double amount, bool useAbbrev ) const
{
	if ( useAbbrev ) {
		QString unit = ( amount > 1 ) ? plural_abbrev : name_abbrev;
		if ( unit.isEmpty() )
			unit = ( amount > 1 ) ? plural : name;
		return unit;
	}
	else
		return ( amount > 1 ) ? plural : name;
}

bool Unit::operator==( const Unit &u ) const
{
	//treat QString() and "" as the same
	QString plural_test1 = u.plural.toLower();
	if ( plural_test1.isEmpty() )
		plural_test1 = "";

	QString plural_test2 = plural.toLower();
	if ( plural_test2.isEmpty() )
		plural_test2 = "";

	QString single_test1 = u.name.toLower();
	if ( single_test1.isEmpty() )
		single_test1 = "";

	QString single_test2 = name.toLower();
	if ( single_test2.isEmpty() )
		single_test2 = "";

	if ( plural_test1.isEmpty() && plural_test2.isEmpty() && single_test1.isEmpty() && single_test2.isEmpty() )
		return true;
	else if ( plural_test1.isEmpty() && plural_test2.isEmpty() )
		return single_test1 == single_test2;
	else if ( single_test1.isEmpty() && single_test2.isEmpty() )
		return plural_test1 == plural_test2;
	else
		return ( plural_test1 == plural_test2 || single_test1 == single_test2 );
}

bool Unit::operator<( const Unit &u ) const
{
	return ( QString::localeAwareCompare( name.toLower(), u.name.toLower() ) < 0 );
}
