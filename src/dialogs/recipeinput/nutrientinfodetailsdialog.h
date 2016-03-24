/***************************************************************************
*   Copyright © 2016 José Manuel Santamaría Lema <panfaust@gmail.com>      *
*                                                                          *
*   This program is free software; you can redistribute it and/or modify   *
*   it under the terms of the GNU General Public License as published by   *
*   the Free Software Foundation; either version 2 of the License, or      *
*   (at your option) any later version.                                    *
****************************************************************************/

#ifndef NUTRIENTINFODETAILSDIALOG_H
#define NUTRIENTINFODETAILSDIALOG_H

#include <QDialog>

#include "nutrientinfostatuswidget.h"
#include "datablocks/ingredient.h"

#include <QUrl>

class RecipeDB;

namespace Ui {
	class NutrientInfoDetailsDialog;
}

class NutrientInfoDetailsDialog : public QDialog
{
	Q_OBJECT

public:
	NutrientInfoDetailsDialog( QWidget *parent );

	void clear();

	void addText( const QString & text );
	void displayText();

	void setDatabase( RecipeDB * database );

	static NutrientInfo::Status checkIngredientStatus(
		const Ingredient & ingredient,
		RecipeDB * database,
		QString * message );

signals:
	void updateRequested();

private slots:
	void helpButtonClickedSlot();

	void linkClickedSlot( const QUrl & link );

private:
	Ui::NutrientInfoDetailsDialog * ui;

	QString m_text;

	RecipeDB * m_database;
};

#endif