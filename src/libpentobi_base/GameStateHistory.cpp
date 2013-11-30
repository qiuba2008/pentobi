//----------------------------------------------------------------------------
/** @file libpentobi_base/GameStateHistory.cpp
    @author Markus Enzenberger <enz@users.sourceforge.net>
    @copyright GNU General Public License version 3 or later */
//----------------------------------------------------------------------------

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "GameStateHistory.h"

#include "BoardUtil.h"

namespace libpentobi_base {

using namespace std;
using boardutil::get_current_position_as_setup;

//----------------------------------------------------------------------------

void GameStateHistory::get_as_setup(Variant& variant, Setup& setup) const
{
    LIBBOARDGAME_ASSERT(is_valid());
    variant = m_variant;
    unique_ptr<Board> bd(new Board(variant));
    for (ColorMove mv : m_moves)
        bd->play(mv);
    get_current_position_as_setup(*bd, setup);
}

void GameStateHistory::init(const Board& bd)
{
    init(bd, bd.get_to_play());
}

void GameStateHistory::init(const Board& bd, Color to_play)
{
    m_is_valid = true;
    m_variant = bd.get_variant();
    m_nu_colors = bd.get_nu_colors();
    m_moves.clear();
    for (unsigned i = 0; i < bd.get_nu_moves(); ++i)
        m_moves.push_back(bd.get_move(i));
    m_to_play = to_play;
}

bool GameStateHistory::is_followup(const GameStateHistory& other,
                                   vector<Move>& sequence) const
{
    if (! m_is_valid || ! other.m_is_valid
        || m_variant != other.m_variant
        || m_moves.size() < other.m_moves.size())
        return false;
    for (unsigned i = 0; i < other.m_moves.size(); ++i)
        if (m_moves[i] != other.m_moves[i])
            return false;
    sequence.clear();
    Color to_play = other.m_to_play;
    for (size_t i = other.m_moves.size(); i < m_moves.size(); ++i)
    {
        auto mv = m_moves[i];
        if (mv.color != to_play)
            return false;
        sequence.push_back(mv.move);
        to_play = to_play.get_next(m_nu_colors);
    }
    if (to_play != m_to_play)
        return false;
    return true;
}

//----------------------------------------------------------------------------

} // namespace libpentobi_base
