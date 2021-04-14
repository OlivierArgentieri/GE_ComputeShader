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
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/GameEngine/src/engine/loaders/obj)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/GameEngine/src/engine/loaders/dds)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/GameEngine/src/engine/transform)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/GameEngine/src/engine/render/fbo)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/GameEngine/src/engine/render/render_view)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/GameEngine/src/engine/render/render_texture)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/GameEngine/src/engine/graphic_object/)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/GameEngine/src/editor/shader_scene)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/GameEngine/src/editor/shader_scene/sample_scene)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/GameEngine/src/editor/shader_scene/obj_scene)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/GameEngine/src/editor/shader_scene/obj_scene_test)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/GameEngine/src/editor/shader_scene/raytracing)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/GameEngine/src/editor/shader_scene/particles_scene)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/GameEngine/src/editor/)

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

# transform
FILE(GLOB TRANSFORM ${CMAKE_SOURCE_DIR}/GameEngine/src/engine/transform/*.*)

# fbo
FILE(GLOB FBO ${CMAKE_SOURCE_DIR}/GameEngine/src/engine/render/fbo/*.*)

# render view
FILE(GLOB RENDER_VIEW ${CMAKE_SOURCE_DIR}/GameEngine/src/engine/render/render_view/*.*)

# render texture
FILE(GLOB RENDER_TEXTURE ${CMAKE_SOURCE_DIR}/GameEngine/src/engine/render/render_texture/*.*)

# graphic_object
FILE(GLOB GRAPHIC_OBJECT ${CMAKE_SOURCE_DIR}/GameEngine/src/engine/graphic_object/*.*)

# game
FILE(GLOB EDITOR ${CMAKE_SOURCE_DIR}/GameEngine/src/editor/*.*)

# scenes
FILE(GLOB SHADER_SCENE ${CMAKE_SOURCE_DIR}/GameEngine/src/editor/shader_scene/*.*)
FILE(GLOB SAMPLE_SCENE ${CMAKE_SOURCE_DIR}/GameEngine/src/editor/shader_scene/sample_scene/*.*)
FILE(GLOB OBJ_SCENE ${CMAKE_SOURCE_DIR}/GameEngine/src/editor/shader_scene/obj_scene/*.*)
FILE(GLOB OBJ_SCENE_TEST ${CMAKE_SOURCE_DIR}/GameEngine/src/editor/shader_scene/obj_scene_test/*.*)
FILE(GLOB CS_TO_TEXTURE ${CMAKE_SOURCE_DIR}/GameEngine/src/editor/shader_scene/cs_to_texture/*.*)
FILE(GLOB RAYTRACING ${CMAKE_SOURCE_DIR}/GameEngine/src/editor/shader_scene/raytracing/*.*)
FILE(GLOB PARTICLES_SCENE ${CMAKE_SOURCE_DIR}/GameEngine/src/editor/shader_scene/particles_scene/*.*)


# ======== loaders ========
FILE(GLOB OBJ_LOADER ${CMAKE_SOURCE_DIR}/GameEngine/src/engine/loaders/obj/*.*)
FILE(GLOB DDS_LOADER ${CMAKE_SOURCE_DIR}/GameEngine/src/engine/loaders/dds/*.*)

ADD_EXECUTABLE(GameEngine
	${ENGINE_SOURCES}
	${ENGINE_HEADERS}
	${IMGUI}
	${GLM}
	${SHADER}
	${TEXTURE}
	${TIMER}
	${WINDOW}
	${EDITOR}
	${FBO}
	${RENDER_VIEW}
	${RENDER_TEXTURE}
	${GRAPHIC_OBJECT}
	${OBJ_LOADER}
	${DDS_LOADER}
	${TRANSFORM}
	${SHADER_SCENE}
	${SAMPLE_SCENE}	
	${OBJ_SCENE}
	${OBJ_SCENE_TEST}
	${CS_TO_TEXTURE}
	${RAYTRACING}
	${PARTICLES_SCENE}
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
SOURCE_GROUP(src/engine/loaders/dds FILES ${DDS_LOADER})
SOURCE_GROUP(src/engine/transform FILES ${TRANSFORM})
SOURCE_GROUP(src/engine/render/fbo FILES ${FBO})
SOURCE_GROUP(src/engine/render/render_view FILES ${RENDER_VIEW})
SOURCE_GROUP(src/engine/render/render_texture FILES ${RENDER_TEXTURE})
SOURCE_GROUP(src/engine/graphic_object/ FILES ${GRAPHIC_OBJECT})
SOURCE_GROUP(src/editor/shader_scene FILES ${SHADER_SCENE})
SOURCE_GROUP(src/editor/shader_scene/sample_scene FILES ${SAMPLE_SCENE})
SOURCE_GROUP(src/editor/shader_scene/obj_scene FILES ${OBJ_SCENE})
SOURCE_GROUP(src/editor/shader_scene/obj_scene_test FILES ${OBJ_SCENE_TEST})
SOURCE_GROUP(src/editor/shader_scene/cs_to_texture FILES ${CS_TO_TEXTURE})
SOURCE_GROUP(src/editor/shader_scene/raytracing FILES ${RAYTRACING})
SOURCE_GROUP(src/editor/shader_scene/particles_scene FILES ${PARTICLES_SCENE})
SOURCE_GROUP(src/editor/ FILES ${EDITOR})

# Compile option
TARGET_COMPILE_DEFINITIONS(GameEngine PUBLIC GLEW_STATIC)


FILE(COPY ${CMAKE_SOURCE_DIR}/GameEngine/assets DESTINATION ${CMAKE_BINARY_DIR}/GameEngine/src)
