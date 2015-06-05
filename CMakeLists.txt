include (ExternalProject)
cmake_minimum_required(VERSION 3.2.2)
project(NoobWerkz)

# Tried to remove -fno-strict-aliasing, but to no effect.
set(CMAKE_CXX_FLAGS "-std=c++11 -fno-strict-aliasing -Wall ${CMAKE_CXX_FLAGS}")
set(CMAKE_C_FLAGS "-std=c99 -fno-strict-aliasing -Wall ${CMAKE_C_FLAGS}")

list(APPEND common_src
	common/Application.cpp
	common/StdioLogSystem.cpp
	common/DrawFunc.cpp
	common/UpdateFunc.cpp
	common/InitFunc.cpp
	common/EngineDesktop.cpp
	)

add_executable(NoobRepro ${common_src}) 

target_link_libraries(NoobRepro LINK_PUBLIC glfw ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES} RuntimeObjectSystem RuntimeCompiler dl) 
