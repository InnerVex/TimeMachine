cmake_minimum_required(VERSION 2.8.12)


project(TimeMachine_database)


list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")
####
find_package(Qt5Core)
include_directories(${Qt5Core_INCLUDES})
add_definitions(${Qt5Core_DEFINITIONS})
set(CMAKE_CXX_FLAGS "${Qt5Core_EXECUTABLE_COMPILE_FLAGS}")
####
find_package(ODB REQUIRED COMPONENTS qt sqlite)
include(${ODB_USE_FILE})
set(${PROJECT_NAME}_SOURCES
        driver.cpp
        database.h)

set(${PROJECT_NAME}_ODB_HEADERS
        person.h
        source.h
        path.h
        file.h
        timeStamp.h)

odb_compile(${PROJECT_NAME}_SOURCES FILES ${${PROJECT_NAME}_ODB_HEADERS} DB sqlite GENERATE_QUERY GENERATE_SESSION GENERATE_SCHEMA )

add_executable(${PROJECT_NAME}
        ${${PROJECT_NAME}_SOURCES}
        ${${PROJECT_NAME}_ODB_HEADERS})

target_include_directories(${PROJECT_NAME}
        PRIVATE
                ${ODB_INCLUDE_DIR}
                ${ODB_COMPILE_OUTPUT_DIR})


target_link_libraries(${PROJECT_NAME}
          ${ODB_CORE_LIBRARY} ${ODB_SQLITE_LIBRARY} )

target_compile_definitions(${PROJECT_NAME}
        PRIVATE
                DATABASE_SQLITE)
####
qt5_use_modules(${PROJECT_NAME} Core)
