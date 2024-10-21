cmake_minimum_required(VERSION 3.5)

project(FreeGM VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        resman.hpp resdef.hpp gameman.hpp
        ui/main/mainwindow.cpp ui/main/mainwindow.h ui/main/mainwindow.ui
        ui/editor/sprite/spriteeditor.h ui/editor/sprite/spriteeditor.cpp ui/editor/sprite/spriteeditor.ui
        ui/editor/constant/constanteditor.h ui/editor/constant/constanteditor.cpp ui/editor/constant/constanteditor.ui
        ui/editor/object/objecteditor.h ui/editor/object/objecteditor.cpp ui/editor/object/objecteditor.ui
        ui/editor/room/roomeditor.h ui/editor/room/roomeditor.cpp ui/editor/room/roomeditor.ui
        ui/editor/object/eventpicker.h ui/editor/object/eventpicker.cpp ui/editor/object/eventpicker.ui
        ui/editor/room/roomview.h ui/editor/room/roomview.cpp ui/editor/room/roomview.ui
        ui/main/compiledialog.h ui/main/compiledialog.cpp ui/main/compiledialog.ui
        ui/main/roomorderer.h ui/main/roomorderer.cpp ui/main/roomorderer.ui
        ui/editor/object/vardefiner.h ui/editor/object/vardefiner.cpp ui/editor/object/vardefiner.ui
        resources.qrc
        lib/tinyxml2.h lib/tinyxml2.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(FreeGM
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET FreeGM APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(FreeGM SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(FreeGM
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(FreeGM PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(FreeGM PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS FreeGM
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(FreeGM)
endif()
