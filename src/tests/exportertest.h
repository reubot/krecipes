/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Jason Kivlighn (jkivlighn@gmail.com)                                  *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/

#ifndef EXPORTERTEST_H
#define EXPORTERTEST_H

#include <cmath>
#include <iostream>

#include <qstring.h>
#include <qfile.h>
#include <qtextstream.h>

#include "checks.h"

using std::cout;
using std::cerr;
using std::endl;

void check( BaseExporter &exporter, const RecipeList &recipeList )
{
	QFile file("test.txt");
	if ( file.open( IO_WriteOnly ) ) {
		QTextStream stream(&file);
		exporter.writeStream(stream,recipeList);
	}
	else {
		printf("Unable to open file for writing\n");
		exit(1);
	}

	file.close();
}

#endif