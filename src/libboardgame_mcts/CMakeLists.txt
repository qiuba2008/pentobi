# This library contains only header files with templates. The empty target
# exists only to add the headers to IDE project files.
add_custom_target(boardgame_mcts SOURCES
  Atomic.h
  LastGoodReply.h
  Node.h
  PlayerMove.h
  SearchBase.h
  Tree.h
  TreeUtil.h
)
