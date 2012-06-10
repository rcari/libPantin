# Headers for the Pantin::actions namespace

SET (
	Pantin_MOC_HDRS
	${Pantin_MOC_HDRS}
	
)

SET (
	Pantin_HDRS
	${Pantin_HDRS}
	
	${CMAKE_CURRENT_LIST_DIR}/AddGeometryInstance.hpp
	${CMAKE_CURRENT_LIST_DIR}/AddNewGeometry.hpp
	${CMAKE_CURRENT_LIST_DIR}/AddTransformations.hpp
	${CMAKE_CURRENT_LIST_DIR}/DeleteInstanceReference.hpp
	${CMAKE_CURRENT_LIST_DIR}/LoadInstanceReference.hpp
	${CMAKE_CURRENT_LIST_DIR}/ProjectAddModel.hpp
	${CMAKE_CURRENT_LIST_DIR}/RemoveElement.hpp
	${CMAKE_CURRENT_LIST_DIR}/ResolveElementInstance.hpp
)
