/***************************************************************************
 *   Copyright (C) 2003 by                                                 *
 *   Cyril Bosselut (bosselut@b1project.com)                               *
 *   Jason Kivlighn (mizunoami44@users.sourceforge.net)                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "kreexport.h"

#include <qfile.h>
#include <qstylesheet.h>

#include <kdebug.h>
#include <klocale.h>
#include <kmdcodec.h>
#include <ktar.h>
#include <ktempfile.h>
#include <kglobal.h>
#include <kstandarddirs.h>

KreExporter::KreExporter( const QString& filename, const QString &format ) :
  BaseExporter( filename, format )
{
}


KreExporter::~KreExporter()
{
}

void KreExporter::saveToFile( const RecipeList& recipes )
{
	if(format == "kreml")
	{
        	if ( file->open( IO_WriteOnly ) )
		{
			QTextStream stream( file );
			stream << createContent(recipes);
			file->close();
		}
	}
	else{
        // create a temporary .kre file
        QString kreml = createContent(recipes);
        int size = kreml.length();
        // compress and save file
        KTar* kre = new KTar(file->name(), "application/x-gzip");
        kre->open( IO_WriteOnly );
        kre->writeFile(filename+".kreml",getenv( "LOGNAME" ), "", size, kreml);
        kre->close();
	delete kre;
      }
}

/*!
    \fn KreManager::createContent()
 * return a QString containing XML encoded recipe
 */
 //TODO: use QDOM (see recipemlexporter.cpp)?
QString KreExporter::createContent( const RecipeList& recipes )
{
    QString xml;

    xml += "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n";
    xml += "<krecipes version=\""+krecipes_version()+"\" lang=\""+(KGlobal::locale())->country()+"\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:noNamespaceSchemaLocation=\"krecipes.xsd\">\n";

	RecipeList::const_iterator recipe_it;
	for ( recipe_it = recipes.begin(); recipe_it != recipes.end(); ++recipe_it )
	{

    xml +="<krecipes-recipe>\n";
    xml += "<krecipes-description>\n";
    xml += "<title>"+QStyleSheet::escape((*recipe_it).title.utf8())+"</title>\n";

	for ( ElementList::const_iterator author_it = (*recipe_it).authorList.begin(); author_it != (*recipe_it).authorList.end(); ++author_it )
		xml += "<author>"+QStyleSheet::escape((*author_it).name.utf8())+"</author>\n";

    xml += "<pictures>\n";
    xml += "<pic format=\"JPEG\" id=\"1\"><![CDATA["; //fixed id until we implement multiple photos ability
    KTempFile* fn = new KTempFile (locateLocal("tmp", "kre"), ".jpg", 0600);
    fn->setAutoDelete(true);
    (*recipe_it).photo.save(fn->name(), "JPEG");
    QByteArray data;
    if( fn ){
      data = (fn->file())->readAll();
      fn->close();
      xml += KCodecs::base64Encode(data, true);
    }
    delete fn;

    xml += "]]></pic>\n";
    xml += "</pictures>\n";
    xml += "<category>\n";

	for ( ElementList::const_iterator cat_it = (*recipe_it).categoryList.begin(); cat_it != (*recipe_it).categoryList.end(); ++cat_it )
		xml += "<cat>"+QStyleSheet::escape((*cat_it).name.utf8())+"</cat>\n";

    xml += "</category>\n";
    xml += "<serving>";
    xml += QString::number((*recipe_it).persons);
    xml += "</serving>\n";
    xml += "</krecipes-description>\n";
    xml += "<krecipes-ingredients>\n";

	for ( IngredientList::const_iterator ing_it = (*recipe_it).ingList.begin(); ing_it != (*recipe_it).ingList.end(); ++ing_it )
	{
      xml += "<ingredient>\n";
      xml += "<name>"+QStyleSheet::escape((*ing_it).name.utf8())+"</name>\n";
      xml += "<amount>";
      xml += QString::number((*ing_it).amount);
      xml += "</amount>\n";
      xml += "<unit>"+QStyleSheet::escape((*ing_it).units.utf8())+"</unit>\n";
      xml += "<prep>"+QStyleSheet::escape((*ing_it).prepMethod.utf8())+"</prep>\n";
      xml += "</ingredient>\n";
      /// @todo add ingredient properties
	}

    xml += "</krecipes-ingredients>\n";
    xml += "<krecipes-instructions>\n";
    xml += QStyleSheet::escape((*recipe_it).instructions.utf8());
    xml += "</krecipes-instructions>\n";
    xml += "</krecipes-recipe>\n";
    
    if ( progressBarCancelled() ) return QString::null;
    advanceProgressBar();
    }
    
    xml += "</krecipes>\n";

    return xml;
}

