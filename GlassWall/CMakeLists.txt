set(GLASSWALL_DIR ${CMAKE_CURRENT_LIST_DIR})

set(GLASSWALL_PUBLICHEADERS ${GLASSWALL_DIR}/include/world.h
							${GLASSWALL_DIR}/include/entity.h)
set(GLASSWALL_INCLUDE ${GLASSWALL_DIR}/include)
include(GNUInstallDirs)

set(GLASSWALL_SRC ${GLASSWALL_DIR}/src/world.cpp
				${GLASSWALL_DIR}/src/entity.cpp
				${GLASSWALL_DIR}/src/window.cpp
				${GLASSWALL_DIR}/src/renderengine.cpp)

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
)

target_link_libraries(GlassWall 
PUBLIC
	SDL2::SDL2 
	SDL2::SDL2main
	${GLEW_LIBRARIES})

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
    ${CMAKE_CURRENT_BINARY_DIR}/GlassWall/GlassWallConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/GlassWall/GlassWallConfigVersion.cmake
    DESTINATION ${INSTALL_CONFIGDIR}
)

export(EXPORT GlassWallTargets FILE ${CMAKE_CURRENT_BINARY_DIR}/GlassWallTargets.cmake NAMESPACE GlassWall::)

#Register package in user's package registry
export(PACKAGE GlassWall)

add_subdirectory(examples)