CMAKE_MINIMUM_REQUIRED( VERSION 3.0 )
PROJECT( SyncSmith )

#
#       Qt
#
SET( CMAKE_INCLUDE_CURRENT_DIR ON )
SET( CMAKE_AUTOMOC ON )
SET( CMAKE_AUTOUIC ON )

FIND_PACKAGE( Qt5Widgets REQUIRED )
FIND_PACKAGE( Qt5Multimedia REQUIRED )
FIND_PACKAGE( Qt5Network REQUIRED )

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
INCLUDE_DIRECTORIES( "${CMAKE_HOME_DIRECTORY}/src" "${CMAKE_HOME_DIRECTORY}/lib" )
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
    ui/MainWindow.ui
    ui/MainWindow.h
    ui/MainWindow.cpp
    ui/AddTrackDialog.ui
    ui/AddTrackDialog.h
    ui/AddTrackDialog.cpp
    ui/TrackItemDelegate.h
    ui/TrackItemDelegate.cpp
    ui/TrackHeaderItemDelegate.h
    ui/TrackHeaderItemDelegate.cpp
    ui/TimelineWidget.h
    ui/TimelineWidget.cpp
    ui/TrackListView.h
    ui/TrackListView.cpp
    ui/TrackHeaderListView.h
    ui/TrackHeaderListView.cpp
    ui/TrackListModel.h
    ui/TrackListModel.cpp
    ui/UIConstants.h
    ui/TrackHandle.h
    ui/FloatTrackEditor.h
    ui/FloatTrackEditor.cpp
    ui/TrackUtils.h
    ui/TrackUtils.cpp
    core/Track.h
    core/TrackType.h
    core/FloatTrack.h
    core/FloatTrack.cpp
    core/TrackKey.h
    core/SyncContext.h
    core/SyncContext.cpp
    core/SyncServer.h
    core/SyncServer.cpp
    core/SyncClient.h
    core/SyncClient.cpp
    core/SyncPacket.h )

ADD_SOURCE( LIB_SRC "lib" "" LIB_SRC_FILES
    ISyncSmithServer.h
    SyncSmith.h
    SyncSmith.cpp )

ADD_EXECUTABLE( ${PROJECT_NAME} WIN32 ${SRC_FILES} )
ADD_LIBRARY( ${PROJECT_NAME}_Connector SHARED ${SRC_FILES} ${LIB_SRC_FILES} )
TARGET_LINK_LIBRARIES( ${PROJECT_NAME} Qt5::Widgets Qt5::Multimedia Qt5::Network )
TARGET_LINK_LIBRARIES( ${PROJECT_NAME}_Connector Qt5::Widgets Qt5::Multimedia Qt5::Network )
