add_executable(GlassWallExample src/main.cpp)

message(${GLASSWALL_INCLUDE})

target_include_directories(GlassWallExample
PUBLIC
	${GLASSWALL_INCLUDE}
PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(GlassWallExample
	GlassWall::GlassWall)