//-----------------------------------------------------------------------------
/** @file libboardgame_util/WallTime.h
    @author Markus Enzenberger <enz@users.sourceforge.net>
    @copyright GNU General Public License version 3 or later */
//-----------------------------------------------------------------------------

#ifndef LIBBOARDGAME_UTIL_WALL_TIME_H
#define LIBBOARDGAME_UTIL_WALL_TIME_H

#include <chrono>
#include "TimeSource.h"

namespace libboardgame_util {

//-----------------------------------------------------------------------------

/** Wall time.
    @ref libboardgame_doc_threadsafe_after_construction */
class WallTime
    : public TimeSource
{
public:
    WallTime();

    double operator()() override;

private:
    std::chrono::system_clock::time_point m_start;
};
//-----------------------------------------------------------------------------

} // namespace libboardgame_util

#endif // LIBBOARDGAME_UTIL_WALL_TIME_H
