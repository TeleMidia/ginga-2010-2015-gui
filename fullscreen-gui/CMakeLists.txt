cmake_minimum_required (VERSION 3.5)

project(ginga-fullscreen-gui)

set (CMAKE_INCLUDE_CURRENTDIR ON)
set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTORCC ON)

find_package (Qt5Xml REQUIRED)
find_package (Qt5Widgets REQUIRED)

include_directories(
  .
  include
)

set(GINGA_GUI_FULLSCREEN_SOURCES
  src/main.cpp
  src/mainwindow.cpp
  src/page.cpp
  src/pagexmlparser.cpp
  src/menuitem.cpp
  src/gingaproxy.cpp
  src/gingapage.cpp
  src/infobar.cpp
  src/useraccountpage.cpp
  src/defaultrichmenuitem.cpp
  src/comborichmenuitem.cpp
  src/userpreferences.cpp
  src/ippage.cpp
)

set(GINGA_GUI_FULLSCREEN_HEADERS
  include/mainwindow.h
  include/page.h
  include/util.h
  include/pagexmlparser.h
  include/menuitem.h
  include/gingaproxy.h 
  include/gingapage.h
  include/infobar.h
  include/useraccountpage.h
  include/richmenuitem.h
  include/defaultrichmenuitem.h
  include/comborichmenuitem.h
  include/userpreferences.h
  include/ippage.h
)

set(GINGA_GUI_FULLSCREEN_RESOURCES
  Resources.qrc
)

add_executable(gingagui-fullscreen
  ${GINGA_GUI_FULLSCREEN_HEADERS}
  ${GINGA_GUI_FULLSCREEN_SOURCES}
  ${UI_GENERATED_HEADERS}
  ${GINGA_GUI_FULLSCREEN_RESOURCES}
)

target_link_libraries(gingagui-fullscreen
  Qt5::Core
  Qt5::Xml
  Qt5::Widgets
)
