/***************************************************************************
 *   Copyright (C) 2003 by Unai Garro                                      *
 *   ugarro@users.sourceforge.net                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef SHOPPINGCALCULATOR_H
#define SHOPPINGCALCULATOR_H

#include "elementlist.h"
#include "ingredientlist.h"
#include "recipedb.h"

/**
@author Unai Garro
*/
void calculateShopping(ElementList *recipeList,IngredientList *ingredientList, RecipeDB *db);
void sum (IngredientList *totalIngredientList,IngredientList *newIngredientList, RecipeDB *db);
int autoConvertUnits(RecipeDB* database,double amount1,int unit1,double amount2,int unit2, double &newAmount, int &newID);
#endif
