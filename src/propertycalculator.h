/***************************************************************************
 *   Copyright (C) 2003 by Unai Garro                                      *
 *   uga@ee.ed.ac.uk                                                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/


 #include <qptrlist.h>
 #include <recipedb.h>
  #include "elementlist.h"
 #include "ingredientpropertylist.h"
 #include "recipe.h"

void calculateIngredients(RecipeDB *database, IDList &ingList, QPtrList <int> &unitList, QPtrList <double> &amountList);

int autoConvert(RecipeDB *database,double amount1,int unit1,double amount2,int unit2, double &newAmount, int &newID);

void calculateProperties(Recipe* recipe,RecipeDB* database);


void addPropertyToList(RecipeDB *database,IngredientPropertyList &recipePropertyList,IngredientPropertyList &ingPropertyList,Ingredient &ing);
