//-----------------------------------------------------------------------------
/** @file libboardgame_base/CoordPoint.cpp
    @author Markus Enzenberger
    @copyright GNU General Public License version 3 or later */
//-----------------------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "CoordPoint.h"

#include <iostream>

namespace libboardgame_base {

//-----------------------------------------------------------------------------

ostream& operator<<(ostream& out, CoordPoint p)
{
    if (! p.is_null())
        out << '(' << p.x << ',' << p.y << ')';
    else
        out << "NULL";
    return out;
}

//-----------------------------------------------------------------------------

} // namespace libboardgame_base
