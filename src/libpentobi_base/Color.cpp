//-----------------------------------------------------------------------------
/** @file libpentobi_base/Color.cpp
    @author Markus Enzenberger
    @copyright GNU General Public License version 3 or later */
//-----------------------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "Color.h"

#include <sstream>

namespace libpentobi_base {

//-----------------------------------------------------------------------------

ostream& operator<<(ostream& out, Color c)
{
    out << (c.to_int() + 1);
    return out;
}

//-----------------------------------------------------------------------------

} // namespace libpentobi_base
