cmake_minimum_required(VERSION 3.24)
project(Klein LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Fetch GitHub dependency helper
macro(AddModule NAME REPO TAG)
    message(STATUS "Fetching ${NAME} @ ${TAG}")
    FetchContent_Declare(${NAME}
            GIT_REPOSITORY ${REPO}
            GIT_TAG        ${TAG}
            GIT_SHALLOW    TRUE
    )
    FetchContent_MakeAvailable(${NAME})
endmacro()

# Engine sources
file(GLOB_RECURSE ENGINE_SRC ${CMAKE_CURRENT_LIST_DIR}/Engine/*.cpp)

# Engine Static Library
add_library(Klein STATIC
        ${ENGINE_SRC}
)

# Executable
add_executable(KleinEngine main.cpp)
target_link_libraries(KleinEngine PRIVATE Klein)

# ====== Dependencies ======

# GLAD
AddModule(glad https://github.com/Dav1dde/glad.git 5bf3eda)
target_sources(Klein PRIVATE ${glad_SOURCE_DIR}/src/glad.c)
target_include_directories(Klein PUBLIC ${glad_SOURCE_DIR}/include)

# GLM
AddModule(GLM https://github.com/g-truc/glm.git 1.0.1)
target_include_directories(Klein PUBLIC ${glm_SOURCE_DIR})

# GLFW
AddModule(GLFW https://github.com/glfw/glfw.git 3.4)
target_link_libraries(Klein PUBLIC glfw)
target_include_directories(Klein PUBLIC ${glfw_SOURCE_DIR}/include)

# STB
AddModule(STBImage https://github.com/nothings/stb.git f1c79c0)
target_include_directories(Klein PUBLIC ${stbimage_SOURCE_DIR})

# ConcurrentQueue
AddModule(ConcurrentQueue https://github.com/cameron314/concurrentqueue.git master)
target_include_directories(Klein PUBLIC ${concurrentqueue_SOURCE_DIR})

# IMGUI
AddModule(IMGUI https://github.com/ocornut/imgui.git v1.91.1)
file(GLOB IMGUI_SRC
        ${imgui_SOURCE_DIR}/*.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)
add_library(imgui STATIC ${IMGUI_SRC})
target_link_libraries(imgui PUBLIC glfw)
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)
target_link_libraries(Klein PUBLIC imgui)

# ASSIMP
AddModule(ASSIMP https://github.com/assimp/assimp.git v5.3.1)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "" FORCE)
target_link_libraries(Klein PUBLIC assimp)

# Platform macros
if(WIN32)
    target_compile_definitions(Klein PUBLIC PLATFORM_WINDOWS)
elseif(APPLE)
    target_compile_definitions(Klein PUBLIC PLATFORM_MACOS)
elseif(UNIX)
    target_compile_definitions(Klein PUBLIC PLATFORM_LINUX)
endif()

# Local includes
target_include_directories(Klein PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/Engine/include
        ${CMAKE_CURRENT_LIST_DIR}/Engine/LogSystem
)

target_compile_features(Klein PUBLIC cxx_std_20)
