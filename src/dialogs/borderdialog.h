/***************************************************************************
*   Copyright (C) 2004 by                                                 *
*   Jason Kivlighn (jkivlighn@gmail.com)                                  *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/

#ifndef BORDERDIALOG_H
#define BORDERDIALOG_H

#include <kdialog.h>
#include <kcolordialog.h>
#include <khuesaturationselect.h>
#include <kcolorvalueselector.h>


//Added by qt3to4:
#include <Q3VBoxLayout>
#include <Q3GridLayout>
#include <Q3HBoxLayout>
#include <QLabel>

class Q3VBoxLayout;
class Q3HBoxLayout;
class Q3GridLayout;
class Q3GroupBox;
class QLabel;
class K3ListBox;
class QSpinBox;
class KHTMLPart;

class KreBorder;

class BorderDialog : public KDialog
{
	Q_OBJECT

public:
	BorderDialog( const KreBorder &border, QWidget* parent = 0, const char* name = 0 );
	~BorderDialog();

	KreBorder border() const;

	Q3GroupBox* borderGroupBox;
	QLabel* styleLabel;
	K3ListBox* styleListBox;
	QLabel* colorLabel;
	KHueSaturationSelector* hsSelector;
	KColorValueSelector* valuePal;
	QLabel* widthLabel;
	QSpinBox* widthSpinBox;
	K3ListBox* widthListBox;
	KHTMLPart* borderPreview;

protected:
	Q3VBoxLayout* borderGroupBoxLayout;
	Q3HBoxLayout* layout4;
	Q3VBoxLayout* layout3;
	Q3VBoxLayout* layout2;
	Q3VBoxLayout* layout1;

protected slots:
	virtual void languageChange();
	void updatePreview();
	void updateSpinBox( int index );

	void slotHSChanged( int h, int s );
	void slotVChanged( int v );
	void setColor( const QColor &color );

private:
	void loadBorder( const KreBorder &border );
	void initListBoxs();

	QColor selColor;

};

#endif // BORDERDIALOG_H
