# Sources for namespace Pantin::views

SET (
	Pantin_SRCS
	${Pantin_SRCS}
	
	${CMAKE_CURRENT_LIST_DIR}/elementEditor/ElementEditor.cpp
	${CMAKE_CURRENT_LIST_DIR}/elementEditor/ElementModel.cpp
	
	${CMAKE_CURRENT_LIST_DIR}/sceneView/GLGraphicsScene.cpp
	${CMAKE_CURRENT_LIST_DIR}/sceneView/GLView.cpp
	${CMAKE_CURRENT_LIST_DIR}/sceneView/GLViewport.cpp
	${CMAKE_CURRENT_LIST_DIR}/sceneView/GLViewportCamera.cpp
	${CMAKE_CURRENT_LIST_DIR}/sceneView/GLViewportCube.cpp
	${CMAKE_CURRENT_LIST_DIR}/sceneView/SceneView.cpp
	${CMAKE_CURRENT_LIST_DIR}/sceneView/SceneViewEngine.cpp
		${CMAKE_CURRENT_LIST_DIR}/sceneView/tools/CameraControl.cpp
		${CMAKE_CURRENT_LIST_DIR}/sceneView/tools/CameraTrackball.cpp
		${CMAKE_CURRENT_LIST_DIR}/sceneView/tools/Tool.cpp
		${CMAKE_CURRENT_LIST_DIR}/sceneView/tools/ToolBox.cpp
	
	${CMAKE_CURRENT_LIST_DIR}/undoView/UndoView.cpp
)
