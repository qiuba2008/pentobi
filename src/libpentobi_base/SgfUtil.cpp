//-----------------------------------------------------------------------------
/** @file libpentobi_base/SgfUtil.cpp
    @author Markus Enzenberger <enz@users.sourceforge.net>
    @copyright GNU General Public License version 3 or later */
//-----------------------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "SgfUtil.h"

#include "libboardgame_util/Assert.h"

namespace libpentobi_base {
namespace sgf_util {

//-----------------------------------------------------------------------------

const char* get_color_id(Variant variant, Color c)
{
    if (variant == Variant::duo || variant == Variant::junior)
        return (c == Color(0) ? "B" : "W");
    if (c == Color(0))
        return "1";
    if (c == Color(1))
        return "2";
    if (c == Color(2))
        return "3";
    LIBBOARDGAME_ASSERT(c == Color(3));
    return "4";
}

const char* get_setup_id(Variant variant, Color c)
{
    if (variant == Variant::duo || variant == Variant::junior)
        return (c == Color(0) ? "AB" : "AW");
    if (c == Color(0))
        return "A1";
    if (c == Color(1))
        return "A2";
    if (c == Color(2))
        return "A3";
    LIBBOARDGAME_ASSERT(c == Color(3));
    return "A4";
}

//-----------------------------------------------------------------------------

} // namespace sgf_util
} // namespace libpentobi_base
