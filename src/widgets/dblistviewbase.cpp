/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Jason Kivlighn (mizunoami44@users.sourceforge.net)                    *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/

#include "dblistviewbase.h"

#include <kapplication.h>
#include <kconfig.h>
#include <kglobal.h>

DBListViewBase::DBListViewBase( QWidget *parent, RecipeDB *db, int t ) : KListView(parent),
  database(db),
  curr_offset(0),
  total(t),
  lastElement(0)
{
	setSorting(-1);

	KConfig * config = KGlobal::config();config->setGroup( "Advanced" );
	curr_limit = config->readNumEntry( "Limit", -1 );
}

void DBListViewBase::keyPressEvent( QKeyEvent *k )
{
	if ( k->state() == Qt::ShiftButton ) {
		switch ( k->key() ) {
		case Qt::Key_N: {
			if ( curr_offset + curr_limit >= total ) {
				k->accept();
				return;
			}

			curr_offset += curr_limit;

			kapp->processEvents(); //if auto-repeating, user won't otherwise see change in the listview
			reload();
			k->accept();
			break;
		}
		case Qt::Key_P: {
			curr_offset -= curr_limit;
			if ( curr_offset >= 0 ) {
				kapp->processEvents(); //if auto-repeating, user won't otherwise see change in the listview
				reload();
			}
			else
				curr_offset = 0;
			k->accept();
			break;
		}
		default: break;
		}
	}

	k->ignore();
}

void DBListViewBase::reload()
{
	clear();

	if ( curr_offset + curr_limit < total )
		new QListViewItem(this,"Next >>");

	load(curr_limit,curr_offset);

	if ( curr_offset != 0 )
		new QListViewItem(this,"Prev <<");
	
}

void DBListViewBase::createElement( QListViewItem *it )
{
	Q_ASSERT(it);

	if ( lastElement )
		it->moveItem(lastElement);

	lastElement = it;
}

void DBListViewBase::elementCreated()
{
	total++;
}

void DBListViewBase::elementRemoved()
{
	total--;
}

bool DBListViewBase::handleElement( const QString &name )
{
	int c = sortColumn();
	if ( name < firstChild()->text(c) || name >= lastElement->text(c) ) {
		return false;
	}
	else
		return true;
}

#include "dblistviewbase.moc"
