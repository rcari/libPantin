CMAKE_MINIMUM_REQUIRED ( VERSION 2.8.3 )

# -- Qt 4.8.0 minimum --
FIND_PACKAGE( Qt4 4.8.0 COMPONENTS QtCore QtGui QtOpenGL REQUIRED )
SET( QT_USE_OPENGL TRUE )
INCLUDE( ${QT_USE_FILE} )

# -- OpenGL --
FIND_PACKAGE( OpenGL REQUIRED )

# -- GLEW --
SET( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/build" )
FIND_PACKAGE( GLEW REQUIRED )

INCLUDE( build/ModuleInfos.cmake )

PROJECT ( Pantin C CXX )

# Headers
INCLUDE_DIRECTORIES ( ${PANTIN_INCLUDES} )

### ------------- Pantin -------------
SET (
	Pantin_SRCS
	
	src/PantinApplication.cpp
	src/PantinEngine.cpp
	src/PantinInstancesManager.cpp
	src/PantinModule.cpp
)

INCLUDE ( src/actions/sources.cmake )
INCLUDE ( src/commands/sources.cmake )
INCLUDE ( src/dialogs/sources.cmake )
INCLUDE ( src/serialization/sources.cmake )
INCLUDE ( src/tasklets/sources.cmake )
#INCLUDE ( src/views/sources.cmake )

SET (
	Pantin_HDRS
	
	inc/PantinApplication.hpp
	inc/PantinBlockTypes.hpp
	inc/PantinExport.hpp
	inc/PantinModule.hpp
)

SET (
	Pantin_MOC_HDRS
	
	inc/PantinEngine.hpp
	inc/PantinInstancesManager.hpp
)

INCLUDE ( inc/actions/headers.cmake )
INCLUDE ( inc/commands/headers.cmake )
INCLUDE ( inc/dialogs/headers.cmake )
INCLUDE ( inc/serialization/headers.cmake )
INCLUDE ( inc/tasklets/headers.cmake )
#INCLUDE ( inc/views/headers.cmake )

### ------------- MOC --------------
QT4_WRAP_CPP( Pantin_MOC_SRCS ${Pantin_MOC_HDRS} )

# Target library: Pantin
add_library ( ${PANTIN_LIBRARY} SHARED ${Pantin_SRCS} ${Pantin_MOC_SRCS} ${Pantin_HDRS} ${Pantin_MOC_HDRS} )

# Mac OS specifics
if ( APPLE )
	set_target_properties ( 
		${PANTIN_LIBRARY} PROPERTIES
		COMPILE_DEFINITIONS "_K_BUILD_PANTIN;_PANTIN_VERSION=\"${PANTIN_VERSION_STRING}\";_K_MACX;GLEW_MX;${DEBUG_DEFINES}"
		COMPILE_FLAGS -fno-common
		LINK_FLAGS "-undefined dynamic_lookup"
		VERSION ${PANTIN_VERSION_STRING}
	)
	target_link_libraries ( ${PANTIN_LIBRARY} ${KORE_LIBRARY} ${NIGEL_LIBRARY} ${GOOEY_LIBRARY} ${GLARE_LIBRARY} ${GLEW_MX} ${OPENGL_LIBRARIES} ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTOPENGL_LIBRARY} )
# Windows specifics
elseif ( WIN32 )
	IF ( MSVC )
		SOURCE_GROUP ( "Header Files" FILES ${Pantin_HDRS} ${Pantin_MOC_HDRS} )
		SOURCE_GROUP ( "Source Files" FILES ${Pantin_SRCS} )
		SOURCE_GROUP ( "Moc Files" FILES ${Pantin_MOC_SRCS} )
	ENDIF ( MSVC )
	set_target_properties ( 
		${PANTIN_LIBRARY} PROPERTIES
		COMPILE_DEFINITIONS "_K_BUILD_PANTIN;_PANTIN_VERSION=\"${PANTIN_VERSION_STRING}\";_K_WIN32;GLEW_STATIC;GLEW_MX;${DEBUG_DEFINES}"
	)
	target_link_libraries ( ${PANTIN_LIBRARY} ${KORE_LIBRARY} ${NIGEL_LIBRARY} ${GOOEY_LIBRARY} ${GLARE_LIBRARY} ${GLEW_MX} ${OPENGL_LIBRARIES} ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTOPENGL_LIBRARY} )
# Linux specifics
elseif ( UNIX )
	set_target_properties ( 
		${PANTIN_LIBRARY} PROPERTIES
		COMPILE_DEFINITIONS "_K_BUILD_PANTIN;_PANTIN_VERSION=\"${PANTIN_VERSION_STRING}\";_K_UNIX;GLEW_MX;${DEBUG_DEFINES}"
		VERSION ${PANTIN_VERSION_STRING}
	)
	target_link_libraries ( ${PANTIN_LIBRARY} ${KORE_LIBRARY} ${NIGEL_LIBRARY} ${GOOEY_LIBRARY} ${GLARE_LIBRARY} ${GLEW_MX} ${OPENGL_LIBRARIES} ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTOPENGL_LIBRARY} )
endif ( APPLE )

# Documentation
if ( DOXYGEN_FOUND )
	set( DOXYGEN_OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/../doc/html )
	add_custom_command (
		OUTPUT ${DOXYGEN_OUTPUT}
		COMMAND ${CMAKE_COMMAND} -E echo_append "Building Pantin Documentation..."
		COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/../doc/Doxyfile
		COMMAND ${CMAKE_COMMAND} -E echo "Done."
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../doc
	 )
	add_custom_target ( pantin-doc DEPENDS ${DOXYGEN_OUTPUT} )
endif ( DOXYGEN_FOUND )
