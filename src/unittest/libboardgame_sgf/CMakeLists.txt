add_executable(unittest_libboardgame_sgf
  SgfNodeTest.cpp
  SgfTreeTest.cpp
  SgfUtilTest.cpp
  TreeReaderTest.cpp
)

target_link_libraries(unittest_libboardgame_sgf
  boardgame_test_main
  boardgame_test
  boardgame_sgf
  boardgame_util
  )

add_test(libboardgame_sgf unittest_libboardgame_sgf)
