CMAKE_MINIMUM_REQUIRED( VERSION 2.8.11 )
PROJECT( SyncSmith )

#
#       Qt
#
SET( CMAKE_INCLUDE_CURRENT_DIR ON )
SET( CMAKE_AUTOMOC ON )
SET( CMAKE_AUTOUIC ON )

FIND_PACKAGE( Qt5Widgets REQUIRED )
FIND_PACKAGE( Qt5Multimedia REQUIRED )

#
#	SyncSmith
#
SET( BIN_DIR ${CMAKE_HOME_DIRECTORY}/bin )
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${BIN_DIR}" )
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${BIN_DIR}" )
SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${BIN_DIR}" )
SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${BIN_DIR}" )
SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${BIN_DIR}" )
SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${BIN_DIR}" )

SET( CMAKE_DEBUG_POSTFIX "_Debug" )
SET( CMAKE_RELEASE_POSTFIX "_Release" )

IF( UNIX )
    SET( CMAKE_C_COMPILER "clang" )
    SET( CMAKE_CXX_COMPILER "clang++" )
    SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )
ENDIF()

SET( LIB_DIR ${CMAKE_HOME_DIRECTORY}/lib )
INCLUDE_DIRECTORIES( "${CMAKE_HOME_DIRECTORY}/src" "${CMAKE_HOME_DIRECTORY}/include" )
LINK_DIRECTORIES   ( "${BIN_DIR}" "${LIB_DIR}" )
ADD_DEFINITIONS	   ( "-D_UNICODE" "-DUNICODE" "-D_CRT_SECURE_NO_WARNINGS" )

SET( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG -D_CRTDBG_MAP_ALLOC" )

FUNCTION( ADD_SOURCE inVariableName inDirPrefix inSourceGroup outSourceList )
        SET( "${inVariableName}" ${ARGN} )
        FOREACH( filename ${${inVariableName}} )
                LIST( APPEND FILE_LIST ${inDirPrefix}/${filename}  )
        ENDFOREACH( filename )
        SET( "${outSourceList}" ${${outSourceList}} ${FILE_LIST} PARENT_SCOPE )
        SOURCE_GROUP( "${inSourceGroup}" FILES ${FILE_LIST} )
ENDFUNCTION( ADD_SOURCE )

#
#	Source Files
#
#->Main
ADD_SOURCE( MAIN_SRC "src" "" SRC_FILES
    Main.cpp
    MainWindow.ui
    MainWindow.h
    MainWindow.cpp
    TrackItemDelegate.h
    TrackItemDelegate.cpp
    TrackHeaderItemDelegate.h
    TrackHeaderItemDelegate.cpp
    TimelineWidget.h
    TimelineWidget.cpp
    TrackListView.h
    TrackListView.cpp
    TrackHeaderListView.h
    TrackHeaderListView.cpp
    Track.h
    TrackType.h
    TrackListModel.h
    TrackListModel.cpp
    FloatTrack.h
    FloatTrack.cpp
    TrackKey.h
    SyncSmithDevelopmentClient.h
    SyncSmithDevelopmentClient.cpp )

ADD_EXECUTABLE( ${PROJECT_NAME} WIN32 ${SRC_FILES} )
TARGET_LINK_LIBRARIES( ${PROJECT_NAME} Qt5::Widgets Qt5::Multimedia )
