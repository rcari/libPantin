# Headers for the Pantin::views namespace

SET (
	Pantin_MOC_HDRS
	${Pantin_MOC_HDRS}
	
        #${CMAKE_CURRENT_LIST_DIR}/elementEditor/ElementEditor.hpp
	
        #${CMAKE_CURRENT_LIST_DIR}/sceneView/GLView.hpp
        #${CMAKE_CURRENT_LIST_DIR}/sceneView/GLViewport.hpp
        #${CMAKE_CURRENT_LIST_DIR}/sceneView/GLViewportCube.hpp
        #${CMAKE_CURRENT_LIST_DIR}/sceneView/SceneView.hpp
        #	${CMAKE_CURRENT_LIST_DIR}/sceneView/tools/CameraControl.hpp
        #	${CMAKE_CURRENT_LIST_DIR}/sceneView/tools/CameraTrackball.hpp
        #	${CMAKE_CURRENT_LIST_DIR}/sceneView/tools/Tool.hpp
        #	${CMAKE_CURRENT_LIST_DIR}/sceneView/tools/ToolBox.hpp
	
	${CMAKE_CURRENT_LIST_DIR}/UndoView.hpp
)

SET (
	Pantin_HDRS
	${Pantin_HDRS}
	
        #${CMAKE_CURRENT_LIST_DIR}/elementEditor/ElementModel.hpp
	
        #${CMAKE_CURRENT_LIST_DIR}/sceneView/GLGraphicsScene.hpp
        #${CMAKE_CURRENT_LIST_DIR}/sceneView/GLViewportCamera.hpp
        #${CMAKE_CURRENT_LIST_DIR}/sceneView/SceneViewEngine.hpp
)
