# - Find ODB library
# Find the native ODB includes and library
# This module defines
#  ODB_INCLUDE_DIR, where to find Url.h, etc.
#  ODB_LIBRARIES, libraries to link against to use ODB client C++.
#  ODB_FOUND, If false, do not try to use ODB client C++.
# also defined, but not for general use are
#  ODB_CORE_LIBRARY, where to find the core ODB library.
#  ODB_SQLITE_LIBRARY, where to find the sqlite ODB library.
#  ODB_QT_LIBRARY, where to find the qt profile for the ODB library.
 
function(ODB_WRAP SRCS HDRS DATABASE PROFILE SCHEMA_NAME) # ARGS
MESSAGE(STATUS "ODB database: " ${DATABASE})
MESSAGE(STATUS "ODB profile: " ${PROFILE})
set(${SRCS})
set(${HDRS})

get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
message( ${CMAKE_CURRENT_SOURCE_DIR})
message(${CMAKE_CURRENT_BINARY_DIR})

foreach(dir ${dirs})
  set( current_include_directories ${current_include_directories} -I ${dir} )
endforeach()
foreach(_odb_h ${ARGN})
    get_filename_component(_abs_odb_h ${_odb_h} ABSOLUTE)
    get_filename_component(_we_odb_h ${_odb_h} NAME_WE)

    list(APPEND ${SRCS} "${CMAKE_CURRENT_BINARY_DIR}/${_we_odb_h}-odb.cpp")
    list(APPEND ${HDRS} "${CMAKE_CURRENT_BINARY_DIR}/${_we_odb_h}-odb.h")
#    list(APPEND ${HDRS} "${CMAKE_CURRENT_BINARY_DIR}/${_we_odb_h}-odb.i")
 
    add_custom_command(
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/o${_we_odb_h}-odb.cpp"
           "${CMAKE_CURRENT_BINARY_DIR}/${_we_odb_h}-odb.h"
           "${CMAKE_CURRENT_BINARY_DIR}/${_we_odb_h}-odb.i"
    COMMAND ${ODB_COMPILER}
    ARGS -d ${DATABASE} -p ${PROFILE}
    -I ${ODB_INCLUDE_DIR} ${current_include_directories} -I ${CMAKE_BINARY_DIR}/proto
    --hxx-suffix .h
    --ixx-suffix .i
    --cxx-suffix .cpp
    --output-dir ${CMAKE_CURRENT_BINARY_DIR}
    --generate-query
    --generate-schema
    --sqlite-override-null
    --schema-name ${SCHEMA_NAME}
    --generate-session
    -x -std=c++11
    ${_abs_odb_h}
    DEPENDS ${_abs_odb_h}
    COMMENT "Running C++ ODB compiler on ${_odb_h}")
endforeach()
set_source_files_properties(${${SRCS}} ${${HDRS}} PROPERTIES GENERATED TRUE)
set(${SRCS} ${${SRCS}} PARENT_SCOPE)
set(${HDRS} ${${HDRS}} PARENT_SCOPE)
endfunction()
 
FIND_PROGRAM(ODB_COMPILER odb
PATHS

/bin
/usr/bin
/opt/bin
/opt/odb/bin
)
 
FIND_PATH(ODB_INCLUDE_DIR odb/core.hxx
PATHS
/usr/include
/usr/local/include
/opt/odb/include
/opt/include
)

FIND_LIBRARY(ODB_CORE_LIBRARY odb
PATHS
/usr/lib/
/usr/lib64/
/usr/local/lib/
/usr/local/lib64/
/opt/odb/lib
/opt/lib
)

FIND_PATH(ODB_SQLITE_INCLUDE_DIR odb/sqlite/database.hxx
PATHS
/usr/include
/usr/local/include
/opt/odb/include
/opt/include
)

 
FIND_LIBRARY(ODB_SQLITE_LIBRARY odb-sqlite
PATHS
/usr/lib/
/usr/lib64/
/usr/local/lib/
/usr/local/lib64/
/opt/odb/lib
/opt/lib
)

FIND_PATH(ODB_QT_INCLUDE_DIR odb/qt/exception.hxx
PATHS
/usr/include
/usr/local/include
/opt/odb/include
/opt/include
)


FIND_LIBRARY(ODB_QT_LIBRARY odb-qt
PATHS
/usr/lib/
/usr/lib64/
/usr/local/lib/
/usr/local/lib64/
/opt/odb/lib
/opt/lib
)

 
# handle the QUIETLY and REQUIRED arguments and set ODB_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(ODB  DEFAULT_MSG
    ODB_CORE_LIBRARY ODB_SQLITE_LIBRARY ODB_QT_LIBRARY
    ODB_INCLUDE_DIR ODB_COMPILER)
#
IF(ODB_FOUND)
    SET(ODB_LIBRARIES ${ODB_CORE_LIBRARY} )
    MESSAGE(STATUS "Found ODB Compiler (ODB_COMPILER = ${ODB_COMPILER})")
    MESSAGE(STATUS "Found ODB Include folder (ODB_INCLUDE_DIR = ${ODB_INCLUDE_DIR})")
    MESSAGE(STATUS "Found ODB Core Libraries (ODB_LIBRARIES = ${ODB_LIBRARIES})")
    MESSAGE(STATUS "Found ODB SQLite Library (ODB_SQLITE_LIBRARY = ${ODB_SQLITE_LIBRARY})")
ENDIF()
#
MARK_AS_ADVANCED(ODB_INCLUDE_DIR ODB_LIBRARIES)
