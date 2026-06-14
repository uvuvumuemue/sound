cmake_minimum_required(VERSION 3.16)

project(sound LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Włączenie automatycznego generowania kodu przez Qt (meta-object compiler)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Wyszukiwanie pakietu Qt (moduł Widgets) - kompatybilne z Qt5 i Qt6
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network Multimedia)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network Multimedia)

# Definicja pliku wykonywalnego i jawne wskazanie WSZYSTKICH plików interfejsu
add_executable(sound WIN32
    main.cpp
    mainwindow.h
    mainwindow.cpp
    leftpanel.h
    leftpanel.cpp
    middlepanel.h
    middlepanel.cpp
    rightpanel.h
    rightpanel.cpp
    bottombar.h
    bottombar.cpp
    playlistmanager.h
    playlistmanager.cpp
    playlist.h playlist.cpp
    servermanager.h
    servermanager.cpp
    zapis.h zapis.cpp
    odczyt.h odczyt.cpp
)

# Konsolidacja (linkowanie) Twojej aplikacji z biblioteką Qt Widgets
target_link_libraries(sound PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::Multimedia)

# Sekcja instalacyjna (opcjonalna, generowana domyślnie przez Qt Creator)
include(GNUInstallDirs)
install(TARGETS sound
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)