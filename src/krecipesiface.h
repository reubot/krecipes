/***************************************************************************
*   Copyright (C) 2003 by                                                 *
*   Unai Garro (ugarro@users.sourceforge.net)                             *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/

#ifndef _KRECIPESIFACE_H_
#define _KRECIPESIFACE_H_

#include <dcopobject.h>
#include <dcopref.h>

#include <qvaluelist.h>

class KrecipesIface : virtual public DCOPObject
{
	K_DCOP
public:

k_dcop:
	virtual DCOPRef currentDatabase() const = 0;
	virtual void reload() = 0;
	
	virtual void exportRecipes( const QValueList<int> &ids ) = 0;
};

#endif // _KRECIPESIFACE_H_
