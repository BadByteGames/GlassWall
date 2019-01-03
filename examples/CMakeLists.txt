add_executable(GlassWallExample src/main.cpp)

message(${GLASSWALL_INCLUDE})

target_include_directories(GlassWallExample
PUBLIC
	${GLASSWALL_INCLUDE}
PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${SDL2_INCLUDE_DIR}
	${GLEW_INCLUDE_DIR}
	${GLM_INCLUDE_DIR})

target_link_libraries(GlassWallExample
	GlassWall::GlassWall)
	
configure_file(glsl/debug.frag ${CMAKE_BINARY_DIR}/examples/debug.frag COPYONLY)
configure_file(glsl/debug.vert ${CMAKE_BINARY_DIR}/examples/debug.vert COPYONLY)

configure_file(resources/Floor.png ${CMAKE_BINARY_DIR}/examples/Floor.png COPYONLY)