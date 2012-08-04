/***************************************************************************
*   Copyright © 2012 José Manuel Santamaría Lema <panfaust@gmail.com>     *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/

#include "kreauthorlistwidget.h"

#include <klocalizedstring.h>

#include <QStandardItemModel>

#include "datablocks/elementlist.h"
#include "backends/recipedb.h"


KreAuthorListWidget::KreAuthorListWidget( QWidget *parent, RecipeDB *db ):
	KreGenericListWidget( parent, db )
{
	QStringList horizontalLabels;
	horizontalLabels << "Id" << i18nc( "@title:column", "Author" );
	m_sourceModel->setHorizontalHeaderLabels( horizontalLabels );
}

void KreAuthorListWidget::createAuthor( const Element &author )
{
	//TODO
}

void KreAuthorListWidget::removeAuthor( int id )
{
	//TODO
}

void KreAuthorListWidget::load( int limit, int offset )
{
	ElementList authorList;
	m_database->loadAuthors( &authorList, limit, offset );

	ElementList::const_iterator author_it;
        for ( author_it = authorList.constBegin(); author_it != authorList.constEnd(); ++author_it ) {
                QStandardItem *itemId = new QStandardItem;
                itemId->setData( QVariant(author_it->id), Qt::EditRole );
                itemId->setEditable( false );
                QStandardItem *itemAuthor = new QStandardItem( author_it->name );
                itemAuthor->setEditable( false );
                QList<QStandardItem*> items;
                items << itemId << itemAuthor;
                m_sourceModel->appendRow( items );
	}
}

#include "kreauthorlistwidget.moc"
