cmake_minimum_required (VERSION 3.2)
project(run_view)

set (CMAKE_INCLUDE_CURRENTDIR ON)
set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTORCC ON)

include_directories(
  .
  include
  ../desktop-gui/include
  "${CMAKE_BINARY_DIR}/src/plugins/gingagui-all/run-view"
)

find_package (Qt5Widgets REQUIRED)

set (RUN_VIEW_SOURCES
  include/runviewfactory.h
  include/runviewplugin.h
  ../desktop-gui/include/gingaview.h
  ../desktop-gui/include/util.h
  ../desktop-gui/include/gingaproxy.h
  include/configdialog.h

  src/runviewfactory.cpp
  src/runviewplugin.cpp
  ../desktop-gui/src/gingaview.cpp
  ../desktop-gui/src/util.cpp
  ../desktop-gui/src/gingaproxy.cpp
  src/configdialog.cpp
  
  resources.qrc
)

set(RUN_VIEW_UI
  ../desktop-gui/forms/runform.ui
)

qt5_wrap_ui(RUN_VIEW_UI_HEADERS
  ${RUN_VIEW_UI}
)

add_library(run_view
  SHARED
  ${RUN_VIEW_SOURCES}
  ${RUN_VIEW_UI_HEADERS}
)

target_link_libraries(run_view
  nclcomposer_core
  Qt5::Widgets
)

install (TARGETS run_view DESTINATION plugins)
