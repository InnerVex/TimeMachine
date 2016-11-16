project(Test_MyBar)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_BUILD_TYPE Debug)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)

#Add sources, headers and ui files
aux_source_directory(. SRC_LIST)
set(HEADERS
    playerdefinitions.h
    player.h
    playercontroller.h
    streamcontroller.h
    timebarconstants.h
    timebar.h)
set(UI
    player.ui)
set(RESOURCES
    playerresources.qrc)

qt5_add_resources(UI_RESOURCES_RCC ${RESOURCES})

#Add Target
add_executable(${PROJECT_NAME}
    ${RESOURCES}
    ${UI_RESOURCES_RCC}
    ${SRC_LIST}
    ${HEADERS}
    ${UI})

#C++ minimum version
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)

# Include QT
qt5_use_modules( ${PROJECT_NAME} LINK_PRIVATE Core Widgets Gui )

# Include libvlc
set(LIBVLC_DIR "${PROJECT_SOURCE_DIR}/../../../Libraries/libvlc/include")
include_directories(${LIBVLC_DIR})
set(LIBVLC_PATH "${PROJECT_SOURCE_DIR}/../../../Libraries/libvlc/libvlc.dll")
target_link_libraries(${PROJECT_NAME} ${LIBVLC_PATH})

MESSAGE ("DIR" ${LIBVLC_DIR})
