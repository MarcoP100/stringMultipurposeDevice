cmake_minimum_required(VERSION 3.16)

project(StringMultipurposeDevice VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 6.2 COMPONENTS Quick VirtualKeyboard Svg REQUIRED)

qt_add_executable(appStringMultipurposeDevice
    main.cpp
)

qt_add_qml_module(appStringMultipurposeDevice
    URI StringMultipurposeDevice
    VERSION 1.0
    QML_FILES main.qml
    QML_FILES CanLoggerPage.qml
    QML_FILES MainPage.qml
    QML_FILES SwipeButtonArea.qml
    QML_FILES DynamometerOverCan.qml
    SOURCES wifimanager.h wifimanager.cpp
    RESOURCES resources/resources.qrc
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appStringMultipurposeDevice PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appStringMultipurposeDevice
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appStringMultipurposeDevice
    PRIVATE Qt6::Quick Qt6::VirtualKeyboard Qt6::Svg)

#qt_import_plugins(appStringMultipurposeDevice
#    INCLUDE Qt::QVirtualKeyboardPlugin
#)

include(GNUInstallDirs)
install(TARGETS appStringMultipurposeDevice
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})


