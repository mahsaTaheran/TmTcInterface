target_sources(${TARGET_NAME} 
	PRIVATE
		main.cpp
)

add_subdirectory(core)
add_subdirectory(fsfwconfig)
add_subdirectory(utility)
add_subdirectory(test)

target_include_directories(${TARGET_NAME} 
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}
)
