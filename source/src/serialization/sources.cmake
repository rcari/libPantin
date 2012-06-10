# Sources for namespace Pantin::serialization

SET (
	Pantin_SRCS
	${Pantin_SRCS}
	
	${CMAKE_CURRENT_LIST_DIR}/3ds/3dsFileImporterWizard.cpp
	${CMAKE_CURRENT_LIST_DIR}/3ds/3dsImportTasklet.cpp
	${CMAKE_CURRENT_LIST_DIR}/3ds/3dsSearchingTasklet.cpp
	${CMAKE_CURRENT_LIST_DIR}/3ds/3dsSerializer.cpp
	${CMAKE_CURRENT_LIST_DIR}/3ds/3dsTasklet.cpp
	${CMAKE_CURRENT_LIST_DIR}/FileImporterWizard.cpp
	${CMAKE_CURRENT_LIST_DIR}/Serializer.cpp
)
