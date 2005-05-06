/***************************************************************************
*   Copyright (C) 2003 by                                                 *
*   Unai Garro (ugarro@users.sourceforge.net)                             *
*   Cyril Bosselut (bosselut@b1project.com)                               *
*   Jason Kivlighn (mizunoami44@users.sourceforge.net)                    *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/

#include "recipeviewdialog.h"

#include <qlayout.h>
#include <qstyle.h>

#include <kapplication.h>
#include <kcursor.h>
#include <kdebug.h>
#include <khtmlview.h>
#include <khtml_part.h>
#include <klocale.h>
#include <kmainwindow.h>
#include <kprogress.h>
#include <kstandarddirs.h>
#include <kstatusbar.h>

#include "mixednumber.h"
#include "backends/recipedb.h"
#include "exporters/htmlexporter.h"

RecipeViewDialog::RecipeViewDialog( QWidget *parent, RecipeDB *db, int recipeID ) : QVBox( parent ),
	database(db)
{
	// Initialize UI Elements
	recipeView = new KHTMLPart( this );

	connect( database, SIGNAL(recipeRemoved(int)), SLOT(recipeRemoved(int)) );

	tmp_filename = locateLocal( "tmp", "krecipes_recipe_view" );

	//----------Load  the recipe --------
	if ( recipeID != -1 )
		loadRecipe( recipeID );
}

RecipeViewDialog::~RecipeViewDialog()
{
	if ( recipe_loaded )
		removeOldFiles();
}

bool RecipeViewDialog::loadRecipe( int recipeID )
{
	QValueList<int> ids;
	ids.append( recipeID );
	return loadRecipes( ids );
}

bool RecipeViewDialog::loadRecipes( const QValueList<int> &ids )
{
	KApplication::setOverrideCursor( KCursor::waitCursor() );

	// Remove any files created by the last recipe loaded
	removeOldFiles();

	ids_loaded = ids; //need to save these ids in order to delete the html files later...make sure this comes after the call to removeOldFiles()
	recipe_loaded = ( ids.count() > 0 && ids[ 0 ] >= 0 );

	bool success = showRecipes( ids );

	KApplication::restoreOverrideCursor();
	return success;
}

bool RecipeViewDialog::showRecipes( const QValueList<int> &ids )
{
	KProgressDialog * progress_dialog = 0;

	if ( ids.count() > 1 )  //we don't want a progress bar coming up when there is only one recipe... it may show up during the splash screen
	{
		progress_dialog = new KProgressDialog( this, "open_progress_dialog", QString::null, i18n( "Opening recipes, please wait..." ), true );
		progress_dialog->resize( 240, 80 );
	}

	int sb_width = recipeView->view() ->style().pixelMetric( QStyle::PM_ScrollBarExtent ) + 4;
	HTMLExporter html_generator( database, tmp_filename + ".html", "html", parentWidget() ->width() - sb_width );

	RecipeList recipe_list;
	database->loadRecipes( &recipe_list, RecipeDB::All, ids );
	html_generator.exporter( recipe_list, progress_dialog ); //writes the generated HTML to 'tmp_filename+".html"'
	if ( progress_dialog && progress_dialog->wasCancelled() ) {
		delete progress_dialog;
		return false;
	}

	delete recipeView;              // Temporary workaround
	recipeView = new KHTMLPart( this ); // to avoid the problem of caching images of KHTMLPart

	KURL url;
	url.setPath( tmp_filename + ".html" );
	recipeView->openURL( url );
	recipeView->show();
	kdDebug() << "Opening URL: " << url.htmlURL() << endl;

	delete progress_dialog;
	return true;
}

void RecipeViewDialog::print( void )
{
	if ( recipe_loaded )
		recipeView->view() ->print();
}

void RecipeViewDialog::reload()
{
	loadRecipes( ids_loaded );
}

void RecipeViewDialog::removeOldFiles()
{
	if ( ids_loaded.count() > 0 ) {
		RecipeList recipe_list;
		database->loadRecipes( &recipe_list, RecipeDB::Title, ids_loaded );

		QStringList recipe_titles;
		for ( RecipeList::const_iterator it = recipe_list.begin(); it != recipe_list.end(); ++it )
			recipe_titles << ( *it ).title;

		HTMLExporter::removeHTMLFiles( tmp_filename, recipe_titles );
	}
}

void RecipeViewDialog::recipeRemoved( int id )
{
	//if the deleted recipe is loaded, clean the view up
	if ( ids_loaded.find(id) != ids_loaded.end() ) { 
		Recipe recipe; database->loadRecipe( &recipe, RecipeDB::Title, id );
		HTMLExporter::removeHTMLFiles( tmp_filename, recipe.title );
		ids_loaded.remove(id);
	}
}

#include "recipeviewdialog.moc"
