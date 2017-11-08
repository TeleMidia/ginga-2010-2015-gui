cmake_minimum_required (VERSION 3.2)
project(run_view)

set (CMAKE_INCLUDE_CURRENTDIR ON)
set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTORCC ON)

configure_file(run-view.json.in run-view.json)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

set (RUN_VIEW_SOURCES
  include/runviewplugin.h
  ../desktop-gui/include/gingaview.h
  ../desktop-gui/include/util.h
  ../desktop-gui/include/gingaproxy.h
  include/configdialog.h

  src/runviewplugin.cpp
  ../desktop-gui/src/gingaview.cpp
  ../desktop-gui/src/util.cpp
  ../desktop-gui/src/gingaproxy.cpp
  src/configdialog.cpp
  
  resources.qrc
)

qt5_wrap_ui(RUN_VIEW_UI_HEADERS
  ../desktop-gui/forms/runform.ui
)

add_library(run_view SHARED
  ${RUN_VIEW_SOURCES}
  ${RUN_VIEW_UI_HEADERS}
)

target_include_directories(run_view PRIVATE
  .
  include
  ../desktop-gui/include
  ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(run_view PRIVATE
  nclcomposer_core
  Qt5::Widgets
)

install(TARGETS run_view DESTINATION lib/nclcomposer/plugins)
