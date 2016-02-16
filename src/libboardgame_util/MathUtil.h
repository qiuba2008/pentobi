//----------------------------------------------------------------------------
/** @file libboardgame_util/MathUtil.h
    @author Markus Enzenberger
    @copyright GNU General Public License version 3 or later */
//----------------------------------------------------------------------------

#ifndef LIBBOARDGAME_UTIL_MATH_UTIL_H
#define LIBBOARDGAME_UTIL_MATH_UTIL_H

#include <cmath>
#include <cstdint>
#include <limits>

namespace libboardgame_util {

using namespace std;

//-----------------------------------------------------------------------------

/** Fast approximation of exp(x).
    The error is less than 15% for abs(x) \< 10 */
template<typename T>
inline T fast_exp(T x)
{
    x = static_cast<T>(1) + x / static_cast<T>(256);
    x *= x;
    x *= x;
    x *= x;
    x *= x;
    x *= x;
    x *= x;
    x *= x;
    x *= x;
    return x;
}

//-----------------------------------------------------------------------------

} // namespace libboardgame_util

#endif // LIBBOARDGAME_UTIL_MATH_UTIL_H
