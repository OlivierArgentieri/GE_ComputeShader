# Settings
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/Include/GL)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/Include/GLFW)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/Include/IMGUI)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/Include/glm)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/GameEngine/src/engine/shader)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/GameEngine/src/engine/texture)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/GameEngine/src/engine/timer)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/GameEngine/src/engine/window)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/GameEngine/src/engine/log)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/GameEngine/src/game/)

# Files
FILE(GLOB_RECURSE ENGINE_SOURCES *.cpp *.c)
FILE(GLOB_RECURSE ENGINE_HEADERS *.h *.hpp *.inl)

# imgui include folder
FILE(GLOB IMGUI ${CMAKE_SOURCE_DIR}/Include/ImGUI/*.*)

# glm
FILE(GLOB GLM ${CMAKE_SOURCE_DIR}/Include/glm/*.*)

# shader
FILE(GLOB SHADER ${CMAKE_SOURCE_DIR}/GameEngine/src/engine/shader/*.*)

# texture
FILE(GLOB TEXTURE ${CMAKE_SOURCE_DIR}/GameEngine/src/engine/texture/*.*)

# timer
FILE(GLOB TIMER ${CMAKE_SOURCE_DIR}/GameEngine/src/engine/timer/*.*)

# window
FILE(GLOB WINDOW ${CMAKE_SOURCE_DIR}/GameEngine/src/engine/window/*.*)

# log
FILE(GLOB LOG ${CMAKE_SOURCE_DIR}/GameEngine/src/engine/log/*.*)

# game
FILE(GLOB GAME ${CMAKE_SOURCE_DIR}/GameEngine/src/game/*.*)

# ======== loaders ========
FILE(GLOB OBJ_LOADER ${CMAKE_SOURCE_DIR}/GameEngine/src/engine/loaders/obj/*.*)

ADD_EXECUTABLE(GameEngine
	${ENGINE_SOURCES}
	${ENGINE_HEADERS}
	${IMGUI}
	${GLM}
	${SHADER}
	${TEXTURE}
	${TIMER}
	${WINDOW}
	${GAME}
	${OBJ_LOADER}
)
TARGET_LINK_LIBRARIES(GameEngine glfw3 opengl32 User32 Gdi32 Shell32 glew32s)

# Add filters
SOURCE_GROUP(Include/IMGUI FILES ${IMGUI})
SOURCE_GROUP(Include/GLM FILES ${GLM})
SOURCE_GROUP(src/engine/shader FILES ${SHADER})
SOURCE_GROUP(src/engine/texture FILES ${TEXTURE})
SOURCE_GROUP(src/engine/timer FILES ${TIMER})
SOURCE_GROUP(src/engine/window FILES ${WINDOW})
SOURCE_GROUP(src/engine/log FILES ${LOG})
SOURCE_GROUP(src/engine/loaders/obj FILES ${OBJ_LOADER})
SOURCE_GROUP(src/game/ FILES ${GAME})

# Compile option
TARGET_COMPILE_DEFINITIONS(GameEngine PUBLIC GLEW_STATIC)


