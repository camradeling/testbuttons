cmake_minimum_required(VERSION 3.18)

project(testbuttons)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
link_directories(${CMAKE_BINARY_DIR})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Qt6_DIR "/home/camradeling/Qt/6.8.2/gcc_64")
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
add_subdirectory(submodules/modbusgw)
include_directories(submodules/modbusgw)
include_directories(submodules/modbusgw/submodules/channellib)
include_directories(submodules/modbusgw/submodules/channellib/messagequeue)
include_directories(submodules/modbusgw/submodules/threadlib)
include_directories(submodules/modbusgw/submodules/modbuslib)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(testbuttons
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        testbutton.h
        testbutton.cpp
        iniparser.h
        iniparser.cpp
    )
add_dependencies(testbuttons modbusgw_static)

# Define target properties for Android with Qt 6 as:
#    set_property(TARGET testbuttons APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(testbuttons SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(testbuttons
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(testbuttons PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(testbuttons PRIVATE
        libmodbusgw_static.a
        libChannelLib_static.a
        libProgramThread_static.a
        pthread
        mxml)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.testbuttons)
endif()

include(GNUInstallDirs)
install(TARGETS testbuttons
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(testbuttons)
endif()
