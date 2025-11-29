cmake_minimum_required(VERSION 3.10)
project(Racing3D)

set(CMAKE_CXX_STANDARD 17)

# Source folders
file(GLOB_RECURSE SRC_FILES
    "src/*.cpp"
)

# Include folder
include_directories(include)

# Add executable
add_executable(Racing3D ${SRC_FILES})

# Link libraries (MinGW + FreeGLUT)
target_link_libraries(Racing3D
    freeglut
    opengl32
    glu32
)
