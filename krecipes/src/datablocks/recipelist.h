/***************************************************************************
*   Copyright (C) 2003 by Unai Garro ugarro@users.sourceforge.net         *
*                                                                         *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/

#ifndef RECIPELIST_H
#define RECIPELIST_H

#include "datablocks/recipe.h"

#include <qvaluelist.h>

/**
@author Unai Garro
*/
class RecipeList: public QValueList <Recipe>
{
public:
	RecipeList();
	~RecipeList();
};

#endif
