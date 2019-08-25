cmake_minimum_required(VERSION 3.14)
project(trygl)

set(CMAKE_CXX_STANDARD 14)
set(EXECUTABLE_OUTPUT_PATH ${trygl_SOURCE_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/build)
set(QT_HOME ~/Applications/Qt5.13.0/5.13.0/gcc_64)
# set(GLEW_HOME ~/Applications/glew-2.1.0/build)
set(CMAKE_CXX_FLAGS -fPIC)

include(cotire)

include_directories(${QT_HOME}/include)
link_directories(${QT_HOME}/lib)

# FIND OPENGL
#########################################################
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

add_executable(trygl main.cpp core/Root.cpp core/Root.h stdafx.cpp stdafx.h defines.h core/RenderWindow.cpp core/RenderWindow.h
        core/x/XAdapter.cpp core/x/XAdapter.h core/Viewport.cpp core/Viewport.h core/Layer.cpp core/Layer.h core/Camera.cpp core/Camera.h core/Scene.cpp core/Scene.h)
set_target_properties(trygl PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "stdafx.h")
cotire(trygl)
target_link_libraries(trygl GLEW ${OPENGL_LIBRARIES} Qt5Widgets Qt5Core Qt5Gui X11 GLX GL pthread)