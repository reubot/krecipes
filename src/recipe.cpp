/***************************************************************************
 *   Copyright (C) 2003 by Unai Garro                                      *
 *   ugarro@users.sourceforge.net                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "recipe.h"

Recipe::Recipe()
{
empty(); //Create & initialize the recipe empty originally
}

Recipe::Recipe(const Recipe &r)
    	{
    	recipeID=r.recipeID;
	persons=r.persons;
	title=r.title;
	instructions=r.instructions;
	photo=r.photo;
	ingList=r.ingList;
	categoryList=r.categoryList;
	authorList=r.authorList;
	}

Recipe::~Recipe()
{
}

void Recipe::empty(void)
{
recipeID=-1;
persons=1;
title=QString::null;
instructions=QString::null;
photo.resize(0,0); //Resizing any dimension to 0, the image becomes a null image
ingList.empty();
categoryList.clear();
authorList.clear();
}

Recipe & Recipe::operator=(const Recipe &r)
    	{
    	recipeID=r.recipeID;
	persons=r.persons;
	title=r.title;
	instructions=r.instructions;
	photo=r.photo;
	ingList=r.ingList;
	categoryList=r.categoryList;
	authorList=r.authorList;
	return *this;
	}
