cmake_minimum_required(VERSION 3.11.0)

project(GlassWall VERSION 1.0.0 LANGUAGES CXX)

find_package(SDL2 REQUIRED)
find_package(glew REQUIRED)
find_package(glm REQUIRED)
find_package(tinyxml2 REQUIRED)

find_package(OpenGL REQUIRED)

set(GLASSWALL_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(GLASSWALL_PUBLICHEADERS ${GLASSWALL_DIR}/include/world.h
							${GLASSWALL_DIR}/include/entity.h
							${GLASSWALL_DIR}/include/settings.h
							${GLASSWALL_DIR}/include/shaderprogram.h
							${GLASSWALL_DIR}/include/model.h
							${GLASSWALL_DIR}/include/camera.h
							${GLASSWALL_DIR}/include/textures.h
							${GLASSWALL_DIR}/include/inputmanager.h
							${GLASSWALL_DIR}/include/vertex.h
							${GLASSWALL_DIR}/include/component.h
							${GLASSWALL_DIR}/include/lighting.h
							${GLASSWALL_DIR}/include/fpscounter.h)

set(GLASSWALL_INCLUDE ${GLASSWALL_DIR}/include)
include(GNUInstallDirs)

set(GLASSWALL_SRC ${GLASSWALL_DIR}/src/world.cpp
				${GLASSWALL_DIR}/src/entity.cpp
				${GLASSWALL_DIR}/src/window.cpp
				${GLASSWALL_DIR}/src/renderengine.cpp
				${GLASSWALL_DIR}/src/shaderprogram.cpp
				${GLASSWALL_DIR}/src/model.cpp
				${GLASSWALL_DIR}/src/camera.cpp
				${GLASSWALL_DIR}/src/textures.cpp
				${GLASSWALL_DIR}/src/inputmanager.cpp
				${GLASSWALL_DIR}/src/colladaloader.cpp
				${GLASSWALL_DIR}/src/component.cpp
				${GLASSWALL_DIR}/src/lighting.cpp
				${GLASSWALL_DIR}/src/fpscounter.cpp)

add_library(GlassWall ${GLASSWALL_PUBLICHEADERS} ${GLASSWALL_SRC})

add_library(GlassWall::GlassWall ALIAS GlassWall)

target_include_directories(GlassWall
    PUBLIC 
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
		${SDL2_INCLUDE_DIR}
		${GLEW_INCLUDE_DIR}
		${GLM_INCLUDE_DIR}
		${tinyxml2_INCLUDE_DIR}
)

target_link_libraries(GlassWall 
PUBLIC
	SDL2::SDL2 
	SDL2::SDL2main
	${OPENGL_gl_LIBRARY}
	${GLEW_LIBRARIES}
	tinyxml2)

set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/GlassWall)

install(TARGETS GlassWall EXPORT GlassWallTargets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT GlassWallTargets 
FILE
	GlassWallTargets.cmake
NAMESPACE
	GlassWall::
DESTINATION 
	${INSTALL_CONFIGDIR})

	
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/GlassWallConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/GlassWallConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/GlassWallConfig.cmake
    INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

message(STATUS ${CMAKE_CURRENT_BINARY_DIR}/GlassWallConfig.cmake)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/GlassWallConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/GlassWallConfigVersion.cmake
    DESTINATION ${INSTALL_CONFIGDIR}
)

export(EXPORT GlassWallTargets FILE ${CMAKE_CURRENT_BINARY_DIR}/GlassWallTargets.cmake NAMESPACE GlassWall::)

#Register package in user's package registry
export(PACKAGE GlassWall)

add_subdirectory(examples)