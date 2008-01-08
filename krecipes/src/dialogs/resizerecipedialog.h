/***************************************************************************
*   Copyright (C) 2003 by                                                 *
*   Unai Garro (ugarro@users.sourceforge.net)                             *
*   Cyril Bosselut (bosselut@b1project.com)                               *
*   Jason Kivlighn (jkivlighn@gmail.com)                                  *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/

#ifndef RESIZERECIPEDIALOG_H
#define RESIZERECIPEDIALOG_H

#include <kdialogbase.h>

class Recipe;

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class FractionInput;
class QButtonGroup;
class QFrame;
class QLabel;
class KIntNumInput;
class KLineEdit;
class QRadioButton;
class QPushButton;

/**
  *@author Jason Kivlighn
  */
class ResizeRecipeDialog : public KDialogBase
{
	Q_OBJECT

public:
	ResizeRecipeDialog( QWidget *parent, Recipe* );

protected slots:
	void accept();
	void activateCurrentOption( int );
	virtual void languageChange();

private:
	void resizeRecipe( double factor );

	Recipe *m_recipe;

	QButtonGroup* buttonGroup;
	QRadioButton* yieldRadioButton;
	QFrame* yieldFrame;
	QLabel* currentYieldLabel;
	QLabel* newYieldLabel;
	KLineEdit* currentYieldInput;
	FractionInput* newYieldInput;
	QRadioButton* factorRadioButton;
	QFrame* factorFrame;
	QLabel* factorLabel;
	FractionInput* factorInput;

	QVBoxLayout* buttonGroupLayout;
	QGridLayout* yieldFrameLayout;
	QHBoxLayout* factorFrameLayout;
};

#endif //RESIZERECIPEDIALOG_H
