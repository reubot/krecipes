/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Jason Kivlighn (mizunoami44@users.sourceforge.net)                    *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/

#ifndef INGREDIENTCOMBOBOX_H
#define INGREDIENTCOMBOBOX_H

#include <kcombobox.h>

#include <qmap.h>

#include "datablocks/element.h"

class QTimer;

class RecipeDB;
class ElementList;

class IngredientComboBox : public KComboBox
{
	Q_OBJECT

public:
	IngredientComboBox( bool, QWidget *parent, RecipeDB *db );

	void reload();
	int id( int row );
	int id( const QString &ing );

	void startLoad();
	void endLoad();

private slots:
	void createIngredient( const Element &element );
	void removeIngredient( int id );

	int findInsertionPoint( const QString &name );
	void loadMore();

private:
	RecipeDB *database;
	QMap<int, int> ingredientComboRows; // Contains the category id for every given row in the category combobox

	int loading_at;
	int ing_count;
	int load_limit;
	QTimer *load_timer;
};

#endif //INGREDIENTCOMBOBOX_H
