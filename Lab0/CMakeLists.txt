cmake_minimum_required(VERSION 3.8)
set(APP_NAME "Lab0")
project(${APP_NAME})

set(CMAKE_CXX_STANDARD 11)
set(INCLUDE_DIRS ../common ../common/Linux)
include_directories(${INCLUDE_DIRS})

file(GLOB SOURCE_FILES lab0.c ../common/*.c ../common/Linux/MicroGlut.c)

#message(WARNING "All include dirs: ${INCLUDE_DIRS}")
#message(WARNING "All source files: ${SOURCE_FILES}")

add_executable(Lab0 ${SOURCE_FILES})
target_link_libraries(${APP_NAME} ${OPENGL_gl_LIBRARY} X11 m)
