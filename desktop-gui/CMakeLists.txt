cmake_minimum_required (VERSION 3.5)

project(ginga-desktop-gui)

set (CMAKE_INCLUDE_CURRENTDIR ON)
set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTORCC ON)

find_package (Qt5Widgets REQUIRED)
find_package (Qt5Network REQUIRED)

include_directories(
  .
  include
  "${CMAKE_BINARY_DIR}/desktop-gui"
)

set(GINGA_GUI_SOURCES
  src/main.cpp
  src/gingaproxy.cpp
  src/developerview.cpp
  src/util.cpp
  src/debugview.cpp
  src/debugobjectitem.cpp
  src/aboutdialog.cpp
  src/channelsdialog.cpp
  src/iptvtunerdialog.cpp
  src/mainwindow.cpp
  src/preferencesdialog.cpp
  src/gingaview.cpp
  src/catalog.cpp
  src/pbds.cpp
)

set(GINGA_GUI_HEADERS
  include/gingaproxy.h
  include/developerview.h
  include/util.h
  include/debugview.h
  include/debugobjectitem.h
  include/iptvtunerdialog.h
  include/aboutdialog.h
  include/channelsdialog.h
  include/mainwindow.h
  include/preferencesdialog.h
  include/gingaview.h
  include/catalog.h
  include/pbds.h
)

set(GINGA_GUI_RESOURCES
  application.qrc
)

set(GINGA_GUI_UI_SOURCES
  forms/aboutform.ui
  forms/channelsform.ui
  forms/environmentform.ui
  forms/iptvtunerform.ui
  forms/preferencesform.ui
  forms/runform.ui
)

qt5_wrap_ui(UI_GENERATED_HEADERS
  ${GINGA_GUI_UI_SOURCES}
)

add_executable(gingagui
  ${GINGA_GUI_HEADERS}
  ${GINGA_GUI_SOURCES}
  ${UI_GENERATED_HEADERS}
  ${GINGA_GUI_RESOURCES}
)

target_link_libraries(gingagui
  Qt5::Core
  Qt5::Widgets
)
