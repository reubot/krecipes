 /**************************************************************************
 *   Copyright (C) 2003 by                                                 *
 *   Unai Garro (ugarro@users.sourceforge.net)                             *
 *   Cyril Bosselut (bosselut@b1project.com)                               *
 *   Jason Kivlighn(mizunoami44@users.sourceforge.net)                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef SELECTRECIPEDIALOG_H
#define SELECTRECIPEDIALOG_H


#include <qwidget.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qcursor.h>
#include <qintdict.h>

#include <kcombobox.h>
#include <klineedit.h>
#include <klistview.h>
#include <kiconloader.h>
#include <kdialog.h>
#include <kfiledialog.h>
#include <kpopupmenu.h>

#include "recipeactionshandler.h"

class QHBox;

class RecipeDB;
class ElementList;
class Recipe;
class AdvancedSearchDialog;
class CategoryTree;
class RecipeFilter;
class RecipeListView;

/**
@author Unai Garro
*/
class SelectRecipeDialog : public QWidget
{
Q_OBJECT
public:
    SelectRecipeDialog(QWidget *parent, RecipeDB *db);
    ~SelectRecipeDialog();


  //Public Methods
  void getCurrentRecipe( Recipe *recipe );

private:

  // Widgets
  QGridLayout *layout;
  QHBox *searchBar;
  RecipeListView* recipeListView;
  QHBox *buttonBar;
  QPushButton *openButton;
  QPushButton *removeButton;
  QPushButton *editButton;
  KIconLoader *il;
  QLabel *searchLabel;
  KLineEdit *searchBox;
  KComboBox *categoryBox;
  AdvancedSearchDialog *advancedSearch;
  // Internal Data
  RecipeDB *database;
  ElementList *recipeList;
  //ElementList *categoryList; // Stores the list of categories corresponding to "recipeList"
  QIntDict <int> categoryComboRows; // Contains the category id for every given row in the category combobox
  RecipeActionsHandler *actionHandler;
  RecipeFilter *recipeFilter;

  // Internal Methods
  void loadCategoryCombo(void);

signals:
  void recipeSelected(int id, int action);
  void recipesSelected(const QValueList<int> &ids, int action);
  void recipeSelected(bool);

private slots:
  void filterComboCategory(int row);
  void showEvent(QShowEvent*);
public slots:
  void haveSelectedItems();
  void slotExportRecipe(){actionHandler->saveAs();}
  void reload(void);
};

#endif
