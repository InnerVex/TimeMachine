cmake_minimum_required(VERSION 2.8.12)


project(TimeMachine_database)


list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

find_package(Qt5Core REQUIRED)
include_directories( ${Qt5Core_INCLUDE_DIRS})

find_package(ODB REQUIRED COMPONENTS qt sqlite)

include_directories( ${CMAKE_BINARY_DIR} )
message( ${CMAKE_BINARY_DIR}  )


set(${PROJECT_NAME}_SOURCES
        driver.cpp
        database.h
        insert.cpp
        select.cpp)

set(${PROJECT_NAME}_HEADERS
        source.h
        path.h
        file.h
        timeStamp.h)

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

add_executable(${PROJECT_NAME}
        ${${PROJECT_NAME}_SOURCES}
        ${${PROJECT_NAME}_HEADERS})

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

qt5_use_modules(${PROJECT_NAME} Core)
message("ODB_SOURCES: " ${ODB_SOURCES_SOURCE})
message("ODB_HEADERS: " ${ODB_HEADERS_SOURCE})
