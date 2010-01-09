/***************************************************************************
*   Copyright © 2003 Unai Garro <ugarro@gmail.com>                        *
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

/**
@author Unai Garro
*/
class RecipeList: public Q3ValueList <Recipe>
{
public:
	RecipeList();
	~RecipeList();
};

#endif
