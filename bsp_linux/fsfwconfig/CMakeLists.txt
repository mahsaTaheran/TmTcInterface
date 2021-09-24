target_sources(${TARGET_NAME} PRIVATE
    ipc/missionMessageTypes.cpp
)

# Add include paths for the executable
target_include_directories(${TARGET_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)
