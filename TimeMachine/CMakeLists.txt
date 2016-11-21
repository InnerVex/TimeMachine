cmake_minimum_required(VERSION 2.8.12)
project(TimeMachine)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
#set(CMAKE_BUILD_TYPE Debug)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)
include_directories( ${Qt5Core_INCLUDE_DIRS})

find_package(ODB REQUIRED COMPONENTS qt sqlite)

include_directories( ${CMAKE_BINARY_DIR} )
message( ${CMAKE_BINARY_DIR}  )

include(FindLIBVLC)

#Sources, Headers, UI and Resource files
set(${PROJECT_NAME}_SOURCES
    insert.cpp
    select.cpp
    driver.cpp
    database.h
    main.cpp
    player.cpp
    playercontroller.cpp
    streamcontroller.cpp
    timebarconstants.h
    timebar.cpp
    main_old.cpp
    slicer.cpp
    parser.cpp)
set(${PROJECT_NAME}_HEADERS
    insert.h
    select.h
    source.h
    path.h
    file.h
    timeStamp.h
    playerdefinitions.h
    player.h
    playercontroller.h
    streamcontroller.h
    timebarconstants.h
    timebar.h
    slicer.h
    parser.h)
set(UI
    player.ui)
set(RESOURCES
    playerresources.qrc)
qt5_add_resources(UI_RESOURCES_RCC ${RESOURCES})

#ODB
ODB_WRAP( ODB_SOURCES_SOURCE ODB_HEADERS_SOURCE sqlite qt timeMachine source.h )
set(${PROJECT_NAME}_SOURCES ${${PROJECT_NAME}_SOURCES} ${ODB_SOURCES_SOURCE})
set(${PROJECT_NAME}_HEADERS ${${PROJECT_NAME}_HEADERS} ${ODB_HEADERS_SOURCE})

ODB_WRAP( ODB_SOURCES_PATH ODB_HEADERS_PATH sqlite qt timeMachine path.h )
set(${PROJECT_NAME}_SOURCES ${${PROJECT_NAME}_SOURCES} ${ODB_SOURCES_PATH})
set(${PROJECT_NAME}_HEADERS ${${PROJECT_NAME}_HEADERS} ${ODB_HEADERS_PATH})

ODB_WRAP( ODB_SOURCES_FILE ODB_HEADERS_FILE sqlite qt timeMachine file.h )
set(${PROJECT_NAME}_SOURCES ${${PROJECT_NAME}_SOURCES} ${ODB_SOURCES_FILE})
set(${PROJECT_NAME}_HEADERS ${${PROJECT_NAME}_HEADERS} ${ODB_HEADERS_FILE})

ODB_WRAP( ODB_SOURCES_TIMESTAMP ODB_HEADERS_TIMESTAMP sqlite qt timeMachine timestamp.h )
set(${PROJECT_NAME}_SOURCES ${${PROJECT_NAME}_SOURCES} ${ODB_SOURCES_TIMESTAMP})
set(${PROJECT_NAME}_HEADERS ${${PROJECT_NAME}_HEADERS} ${ODB_HEADERS_TIMESTAMP})

#Target
add_executable(${PROJECT_NAME}
    ${RESOURCES}
    ${UI_RESOURCES_RCC}
    ${${PROJECT_NAME}_SOURCES}
    ${${PROJECT_NAME}_HEADERS}
    ${UI})

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${ODB_INCLUDE_DIR}
        ${ODB_COMPILE_OUTPUT_DIR}
        ${ODB_QT_INCLUDE_DIR}
        ${ODB_SQLITE_INCLUDE_DIR}
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_BINARY_DIR})


target_link_libraries(${PROJECT_NAME}
          ${ODB_CORE_LIBRARY} ${ODB_SQLITE_LIBRARY} ${ODB_QT_LIBRARY})

target_compile_definitions(${PROJECT_NAME}
        PRIVATE
                DATABASE_SQLITE)

qt5_use_modules(${PROJECT_NAME} Core Widgets Gui)
message("ODB_SOURCES: " ${ODB_SOURCES_SOURCE})
message("ODB_HEADERS: " ${ODB_HEADERS_SOURCE})

include_directories(${LIBVLC_INCLUDE_DIR})
include_directories(${LIBVLC_PLUGINS_DIR})
target_link_libraries(${PROJECT_NAME} ${LIBVLC_LIBRARY})
target_link_libraries(${PROJECT_NAME} ${LIBVLC_CORE_LIBRARY})
