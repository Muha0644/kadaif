cmake_minimum_required(VERSION 3.5)

project(kadaif VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(QT_FEATURE_intelcet ON)

find_package(QT NAMES Qt6 COMPONENTS Widgets Gui Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(PROJECT_SOURCES
				main.cpp
				config.h
				mainwindow.cpp
				parser.cpp
				parser.h
				mainwindow.h
				mainwindow.ui
				dataclass.h
				dataclass.cpp
				loc/locstuff.h
				loc/locstuff.cpp
				loc/loceditor.h
				loc/loceditor.cpp
				loc/loceditor.ui
				sharedassets0.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(kadaif
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )

else()
    add_executable(kadaif
    ${PROJECT_SOURCES}
    )
endif()

target_link_libraries(kadaif PRIVATE Qt6::Gui Qt6::Core Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(kadaif PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(kadaif)
endif()
