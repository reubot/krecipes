/***************************************************************************
*   Copyright © 2003 Unai Garro <ugarro@gmail.com>                        *
*   Copyright © 2003 Cyril Bosselut <bosselut@b1project.com>              *
*   Copyright © 2003 Jason Kivlighn <jkivlighn@gmail.com>                 *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/

#include "recipeviewdialog.h"

#include <QFile>
#include <QPointer>
//Added by qt3to4:

#include <kapplication.h>
#include <kcursor.h>
#include <kdebug.h>
#include <khtmlview.h>
#include <khtml_part.h>
#include <klocale.h>
#include <kmainwindow.h>
#include <kprogressdialog.h>
#include <kstandarddirs.h>
#include <kstatusbar.h>
#include <kconfig.h>
#include <kglobal.h>
#include <kvbox.h>
#include <KTempDir>
#include <QFrame>
#include <QToolButton>
#include <QHBoxLayout>
#include <QTextDocument>

#include "datablocks/mixednumber.h"
#include "backends/recipedb.h"
#include "exporters/xsltexporter.h"
#include "recipeprintpreview.h"

RecipeViewDialog::RecipeViewDialog( QWidget *parent, RecipeDB *db, int recipeID ) : KVBox( parent ),
	database(db)
{
	// Initialize UI Elements
	khtmlpartBox = new KVBox( this );
	recipeView = new KHTMLPart( khtmlpartBox );

	connect( database, SIGNAL(recipeRemoved(int)), SLOT(recipeRemoved(int)) );

	m_tempdir = new KTempDir(KStandardDirs::locateLocal("tmp", "krecipes-data-view"));
	tmp_filename = m_tempdir->name() + "krecipes_recipe_view.html";

	// Functions Box
	QHBoxLayout* functionsLayout = new QHBoxLayout;

	functionsBox = new QFrame ( this );
	//functionsBox->setFrameStyle( QFrame::NoFrame );
	functionsBox->setLayout( functionsLayout );

	closeButton = new QToolButton;
	closeButton->setIcon( KIcon( "window-close" ) );

	closeButton->setText( i18nc( "@action:button", "Close" ) );
	closeButton->setToolTip( i18nc( "@info:tooltip", "Close" ) );
	closeButton->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );

	functionsLayout->layout()->addItem( new QSpacerItem( 10, 10, QSizePolicy::MinimumExpanding, QSizePolicy::Fixed ) );
	functionsLayout->addWidget( closeButton );

	//Connect the signals.
	connect ( closeButton, SIGNAL( clicked() ), this, SLOT( close() ) );
	
	//----------Load  the recipe --------
	if ( recipeID != -1 )
		loadRecipe( recipeID );
}

RecipeViewDialog::~RecipeViewDialog()
{
	delete m_tempdir;
}

bool RecipeViewDialog::loadRecipe( int recipeID )
{
	QList<int> ids;
	ids.append( recipeID );
	return loadRecipes( ids );
}

bool RecipeViewDialog::loadRecipes( const QList<int> &ids, const QString &layoutConfig )
{
	KApplication::setOverrideCursor( Qt::WaitCursor );

	// Remove any files created by the last recipe loaded
	delete m_tempdir;
	m_tempdir = new KTempDir(KStandardDirs::locateLocal("tmp", "krecipes-data-view"));
	tmp_filename = m_tempdir->name() + "krecipes_recipe_view.html";

	ids_loaded = ids; //need to save these ids in order to delete the html files later...make sure this comes after the call to removeOldFiles()
	recipe_loaded = ( ids.count() > 0 && ids[ 0 ] >= 0 );

	bool success = showRecipes( ids, layoutConfig );

	KApplication::restoreOverrideCursor();
	return success;
}

bool RecipeViewDialog::showRecipes( const QList<int> &ids, const QString &layoutConfig )
{
	KProgressDialog * progress_dialog = 0;

	if ( ids.count() > 1 )  //we don't want a progress bar coming up when there is only one recipe... it may show up during the splash screen
	{
		progress_dialog = new KProgressDialog( this, QString(), i18nc( "@title:window", "Opening recipes, please wait..." ) );
		progress_dialog->setObjectName( "open_progress_dialog" );
		progress_dialog->setModal( true );
		progress_dialog->resize( 240, 80 );
	}

	XSLTExporter html_generator( tmp_filename, "html" );
	if ( layoutConfig != QString() ) {
            KConfigGroup config(KGlobal::config(), "Page Setup" );
		QString styleFile = config.readEntry( layoutConfig+"Layout", KStandardDirs::locate( "appdata", "layouts/None.klo" ) );
		if ( !styleFile.isEmpty() && QFile::exists( styleFile ) )
			html_generator.setStyle( styleFile );

		QString templateFile = config.readEntry( layoutConfig+"Template", KStandardDirs::locate( "appdata", "layouts/Default.xsl" ) );
		if ( !templateFile.isEmpty() && QFile::exists( templateFile ) )
			html_generator.setTemplate( templateFile );
	}

	html_generator.exporter( ids, database, progress_dialog ); //writes the generated HTML to 'tmp_filename'
	if ( progress_dialog && progress_dialog->wasCancelled() ) {
		delete progress_dialog;
		return false;
	}

	delete recipeView;              // Temporary workaround
	recipeView = new KHTMLPart( khtmlpartBox ); // to avoid the problem of caching images of KHTMLPart

	//KDE4 port
	KParts::OpenUrlArguments argsUrl (recipeView->arguments());
	argsUrl.setReload( true ); // Don't use the cache
	recipeView->setArguments( argsUrl );
	
	KParts::BrowserArguments args;
	recipeView->browserExtension()->setBrowserArguments(args);

	KUrl url;
	url.setPath( tmp_filename );
	recipeView->openUrl( url );
	recipeView->show();
	kDebug() << "Opening URL: " << Qt::escape(url.prettyUrl()) ;

	delete progress_dialog;
	return true;
}

void RecipeViewDialog::print()
{
	if ( recipe_loaded ) {
		QPointer<RecipePrintPreview> preview = new RecipePrintPreview( this, database, ids_loaded );
		preview->exec();
		delete preview;
	}
}

void RecipeViewDialog::printNoPreview( void )
{
	if ( recipe_loaded ) {
		recipeView->view() ->print();
	}
}


void RecipeViewDialog::close( void )
{
	emit closeRecipeView();
}


void RecipeViewDialog::reload( const QString &layoutConfig )
{
	loadRecipes( ids_loaded, layoutConfig );
}

void RecipeViewDialog::showButtons()
{
	functionsBox->setVisible( true );
}

void RecipeViewDialog::hideButtons()
{
	functionsBox->setVisible( false );
}

void RecipeViewDialog::recipeRemoved( int id )
{
	//if the deleted recipe is loaded, clean the view up
	if ( ids_loaded.indexOf(id) != -1 ) {
		ids_loaded.removeAll(id);
		reload(); //FIXME: This is wrong because we don't know
			// here the proper value for 'layoutConfig' param
	}
}

#include "recipeviewdialog.moc"
