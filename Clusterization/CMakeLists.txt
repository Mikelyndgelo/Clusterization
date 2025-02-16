cmake_minimum_required(VERSION 3.5)

project(Clusterization VERSION 1.0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt5 REQUIRED COMPONENTS Widgets)

set(CLUSTERIZATION_H
    MainLogic.h
    MainView.h
)

set(CLUSTERIZATION_CPP
    main.cpp
    MainLogic.cpp
    MainView.cpp
)

add_executable(Clusterization
    WIN32
    ${CLUSTERIZATION_H}
    ${CLUSTERIZATION_CPP}
)

target_link_libraries(Clusterization Qt5::Widgets)
