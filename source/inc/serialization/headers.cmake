# Headers for the Pantin::serialization namespace

SET (
	Pantin_MOC_HDRS
	${Pantin_MOC_HDRS}
	
	${CMAKE_CURRENT_LIST_DIR}/3ds/3dsFileImporterWizard.hpp
	${CMAKE_CURRENT_LIST_DIR}/3ds/3dsImportTasklet.hpp
	${CMAKE_CURRENT_LIST_DIR}/3ds/3dsSearchingTasklet.hpp
	${CMAKE_CURRENT_LIST_DIR}/3ds/3dsSerializer.hpp
	${CMAKE_CURRENT_LIST_DIR}/3ds/3dsTasklet.hpp
	${CMAKE_CURRENT_LIST_DIR}/FileImporterWizard.hpp
	${CMAKE_CURRENT_LIST_DIR}/Serializer.hpp
)

SET (
	Pantin_HDRS
	${Pantin_HDRS}
	
	${CMAKE_CURRENT_LIST_DIR}/3ds/3dsStructures.hpp
)
