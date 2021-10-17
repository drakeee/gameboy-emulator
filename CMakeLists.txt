# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("gameboy-emulator")
set(PROJECT_MODULE_NAME "gameboy-emulator")

include_directories(
	"${PROJECT_SOURCE_DIR}/src"
)

file(GLOB_RECURSE PROJECT_SOURCE_FILES "./src/*.h" "./src/*.hpp" "./src/*.cpp")
add_executable(${PROJECT_MODULE_NAME} ${PROJECT_SOURCE_FILES})