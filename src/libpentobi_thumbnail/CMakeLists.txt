add_library(pentobi_thumbnail STATIC
  CreateThumbnail.h
  CreateThumbnail.cpp
)

target_compile_definitions(pentobi_thumbnail PRIVATE
    -DQT_DEPRECATED_WARNINGS
    -DQT_DISABLE_DEPRECATED_BEFORE=0x051000)

target_link_libraries(pentobi_thumbnail Qt5::Widgets)
