/***************************************************************************
*                                                                         *
*                           Copyright (C) 2003                            *
*                by Unai Garro (ugarro@users.sourceforge.net)             *
*                       Martin Imobersteg <imm@gmx.ch>                    *
*                          and opie project                               *
*                                                                         *
*                                                                         *
*    This code was originally developed by the opie project, on which     *
*                    Martin Imobersteg based his work.                    *
* This file is adds a small extension, necessary to perform some minimum  * 
* SQL actions                                                             *
*                                                                         *
*         (this project is different from that in qsqlite.sf.net)         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/


#include "krecqsqlitedb.h"
#include "krecqsqliteresult.h"

#include <qvaluelist.h>

#include <kdebug.h>

#include "config.h"
#if HAVE_SQLITE
#include <sqlite.h>
#elif HAVE_SQLITE3
#include <sqlite3.h>
#endif
#include <stdlib.h>

QSQLiteDB::QSQLiteDB( QObject *, const char * )
{}

bool QSQLiteDB::open( const QString &dbname )
{
	char * errmsg = 0;

#if HAVE_SQLITE

	m_db = sqlite_open( dbname.latin1(), 0, &errmsg );
#elif HAVE_SQLITE3

	int res = sqlite3_open( dbname.latin1(), &m_db );

	if ( res != SQLITE_OK )
		return false;
#endif

	if ( m_db == 0L ) {
		free( errmsg );
		return false;
	}
	return true;
}

void QSQLiteDB::close()
{
	if ( m_db ) {
#if HAVE_SQLITE
		sqlite_close( m_db );
#elif HAVE_SQLITE3

		sqlite3_close( m_db );
#endif

		m_db = 0L;
	}
}

QSQLiteResult QSQLiteDB::executeQuery( const QString &query, int *lastID )
{
	QSQLiteResult res;
	if ( !m_db ) {
		return res;
	}

	char *errmsg = 0;
#if HAVE_SQLITE

	if ( sqlite_exec( m_db, query.latin1(), &call_back, &res, &errmsg ) > 0 )
#elif HAVE_SQLITE3

	if ( sqlite3_exec( m_db, query.latin1(), &call_back, &res, &errmsg ) > 0 )
#endif

	{
		kdDebug() << "SQLite error: " << errmsg << endl <<
		"\t (Query: " << query << ")" << endl;
		res.setError( errmsg );
		res.setStatus( QSQLiteResult::Failure );
		free( errmsg );
	}

	if ( lastID ) {
#if HAVE_SQLITE
		* lastID = sqlite_last_insert_rowid( m_db );
#elif HAVE_SQLITE3

		*lastID = sqlite3_last_insert_rowid( m_db );
#endif

	}

	return res;
}

int QSQLiteDB::call_back( void* result, int argc, char** argv, char** columns )
{
	QSQLiteResult * res = ( QSQLiteResult* ) result;

	QMap<QString, QCString> tableString;
	QMap<int, QCString> tableInt;

	for ( int i = 0; i < argc; i++ ) {
		tableInt.insert( i, argv[ i ] );
		tableString.insert( columns[ i ],
		                    argv[ i ] );
	}

	QSQLiteResultRow row( tableString, tableInt );
	res->addRow( row );

	return 0;
}
