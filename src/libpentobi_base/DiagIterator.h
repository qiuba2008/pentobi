//-----------------------------------------------------------------------------
/** @file libpentobi_base/DiagIterator.h
    @author Markus Enzenberger
    @copyright GNU General Public License version 3 or later */
//-----------------------------------------------------------------------------

#ifndef LIBPENTOBI_BASE_DIAG_ITERATOR_H
#define LIBPENTOBI_BASE_DIAG_ITERATOR_H

#include "Board.h"
#include "libboardgame_util/NullTermList.h"

namespace libpentobi_base {

using libboardgame_util::NullTermList;

//-----------------------------------------------------------------------------

class DiagIterator
    : public NullTermList<Point, 9>::Iterator
{
public:
    DiagIterator(const Geometry& geo, Point p);

    DiagIterator(const Board& bd, Point p);
};

inline DiagIterator::DiagIterator(const Geometry& geo, Point p)
    : NullTermList<Point, 9>::Iterator(geo.get_diag(p))
{
}

inline DiagIterator::DiagIterator(const Board& bd, Point p)
    : NullTermList<Point, 9>::Iterator(bd.get_geometry().get_diag(p))
{
}

//-----------------------------------------------------------------------------

} // namespace libpentobi_base

#endif // LIBPENTOBI_BASE_DIAG_ITERATOR_H