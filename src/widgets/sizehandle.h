/**********************************************************************
** Copyright (C) 2000 Trolltech AS.  All rights reserved.
**               2003 Jason Kivlighn <mizunoami44@users.sourceforge.net>
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.trolltech.com/gpl/ for GPL licensing information.
**
** Contact info@trolltech.com if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

#ifndef SIZEHANDLE_H
#define SIZEHANDLE_H

#include <qwidget.h>
#include <qintdict.h>

class QMouseEvent;
class DragArea;
class WidgetSelection;
class QPaintEvent;

class SizeHandle : public QWidget
{
    Q_OBJECT

public:
    enum Direction { LeftTop, Top, RightTop, Right, RightBottom, Bottom, LeftBottom, Left };

    SizeHandle( DragArea *parent, Direction d, WidgetSelection *s );
    void setWidget( QWidget *w );
    void setActive( bool a );
    void updateCursor();

    void setEnabled( bool ) {}
    
signals:
	void widgetGeometryChanged();

protected:
    void paintEvent( QPaintEvent *e );
    void mousePressEvent( QMouseEvent *e );
    void mouseMoveEvent( QMouseEvent *e );
    void mouseReleaseEvent( QMouseEvent *e );

private:
    void trySetGeometry( QWidget *w, int x, int y, int width, int height );
    void tryResize( QWidget *w, int width, int height );

private:
    QWidget *widget;
    Direction dir;
    QPoint oldPressPos;
    DragArea *dragArea;
    WidgetSelection *sel;
    QRect geom, origGeom;
    bool active;

};

class WidgetSelection
{
public:
    WidgetSelection( DragArea *parent/*, QPtrDict<WidgetSelection> *selDict*/ );

    void setWidget( QWidget *w/*, bool updateDict = TRUE*/ );
    bool isUsed() const;

    void updateGeometry();
    void hide();
    void show();
    void update();

    QWidget *widget() const;

protected:

    QIntDict<SizeHandle> handles;
    QWidget *wid;
    DragArea *dragArea;
    //QPtrDict<WidgetSelection> *selectionDict;
};

#endif
