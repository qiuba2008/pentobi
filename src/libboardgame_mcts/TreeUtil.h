//-----------------------------------------------------------------------------
/** @file libboardgame_mcts/TreeUtil.h
    @author Markus Enzenberger <enz@users.sourceforge.net>
    @copyright GNU General Public License version 3 or later */
//-----------------------------------------------------------------------------

#ifndef LIBBOARDGAME_MCTS_TREE_UTIL_H
#define LIBBOARDGAME_MCTS_TREE_UTIL_H

#include "ChildIterator.h"
#include "Tree.h"

namespace libboardgame_mcts {
namespace tree_util {

//-----------------------------------------------------------------------------

template<typename N>
const N* find_child(const Tree<N>& tree, const N& node, typename N::Move mv)
{
    for (ChildIterator<N> i(tree, node); i; ++i)
        if (i->get_move() == mv)
            return &(*i);
    return nullptr;
}

template<typename N, class S>
const N* find_node(const Tree<N>& tree, const S& sequence)
{
    auto node = &tree.get_root();
    for (auto mv : sequence)
        if ((node = find_child(tree, *node, mv)) == nullptr)
            break;
    return node;
}

//-----------------------------------------------------------------------------

} // namespace tree_util
} // namespace libboardgame_mcts

#endif // LIBBOARDGAME_MCTS_TREE_UTIL_H
