project(Test_MyBar)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_BUILD_TYPE Debug)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)

aux_source_directory(. SRC_LIST)


add_executable(${PROJECT_NAME}
    ${SRC_LIST}
    playerdefinitions.h
    player.h
    player.ui
    playercontroller.h
    streamcontroller.h
    timebarconstants.h
    timebar.h)

# Include QT
qt5_use_modules( ${PROJECT_NAME} LINK_PRIVATE Core Widgets Gui )

# Include libvlc
set(LIBVLC_DIR "${PROJECT_SOURCE_DIR}/../../../Libraries/libvlc/include")
include_directories(${LIBVLC_DIR})

set(LIBVLC_PATH "${PROJECT_SOURCE_DIR}/../../../Libraries/libvlc/libvlc.dll")
target_link_libraries(${PROJECT_NAME} ${LIBVLC_PATH})
