cmake_minimum_required(VERSION 3.13 FATAL_ERROR)


set(PROJECT_NAME Hi_animation)
project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} 
	src/main.cpp
	src/window/window.h
	src/window/window.cpp
	src/window/events.h
	src/window/events.cpp
	src/graphics/ShaderCl.h
	src/graphics/ShaderCl.cpp
	#src/graphics/Texture.h
	#src/graphics/Texture.cpp
	src/loaders/png_loading.h
	src/loaders/png_loading.cpp
	src/window/Camera.h
	src/window/Camera.cpp
	#src/graphics/Mesh.h
	#src/graphics/Mesh.cpp


	#test fiels with new mothods fro loading 3d models
	#src/graphics/new_mesh_type.h
	#src/graphics/model.h

	src/graphics/main_model.h
	src/graphics/main_model.cpp
	src/graphics/basic_file_mesh.h
	src/graphics/basic_file_mesh.cpp

	disassemblade_code/disassemblade_code.txt
)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
# Подключение внешних библиотек - ПЕРЕМЕЩЕНО ВВЕРХ
add_subdirectory(external/glfw)
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_BINARY_DIR}/external/glad/include")
add_subdirectory(external/glad)
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_BINARY_DIR}/external/glad/include")
add_subdirectory(external/glm)
add_subdirectory(external/assimp)
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_BINARY_DIR}/external/glad/include")


# Подключение библиотек - теперь после add_subdirectory
target_link_libraries(${PROJECT_NAME} glfw glad glm assimp)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

# Опции для GLFW - рекомендуется оставить, даже если не используются примеры и тесты
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)