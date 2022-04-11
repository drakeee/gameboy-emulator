# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("gameboy-emulator")
set(PROJECT_MODULE_NAME "gameboy-emulator")

include_directories(
	"${PROJECT_SOURCE_DIR}/src",
	"${PROJECT_SOURCE_DIR}/vendors/glfw337/include"
	"${PROJECT_SOURCE_DIR}/vendors/sdl2/include"
)
link_directories(
	${PROJECT_SOURCE_DIR}/vendors/glfw337/lib
	${PROJECT_SOURCE_DIR}/vendors/sdl2/lib
)

file(GLOB_RECURSE PROJECT_SOURCE_FILES "./src/*.h" "./src/*.hpp" "./src/*.cpp")
add_executable(${PROJECT_MODULE_NAME} ${PROJECT_SOURCE_FILES})

target_link_libraries(${PROJECT_MODULE_NAME} glfw3 opengl32 x64/SDL2 x64/SDL2main)