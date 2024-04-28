cmake_minimum_required(VERSION 3.6) # FetchContent is available in 3.11+
project(kMusicPlayer)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Dependencies
set(RAYLIB_VERSION 4.5.0)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
    include(FetchContent)
    FetchContent_Declare(
            raylib
            DOWNLOAD_EXTRACT_TIMESTAMP OFF
            URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
        set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE) # build raylib as a static library
        add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
    endif()
endif()

#if(MSVC)
#    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
#else()
#    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mwindows")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows")
#endif()

# Our Project
add_executable(${PROJECT_NAME} main.c include/tinyfiledialogs.c resources.rc)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)


# Link the static version of pthread
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
if(THREADS_HAVE_PTHREAD_ARG)
    target_compile_options(PUBLIC ${PROJECT_NAME} "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
    target_link_libraries(${PROJECT_NAME} "${CMAKE_THREAD_LIBS_INIT}")
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")

target_link_libraries(${PROJECT_NAME} raylib)




# Web Configurations
if (${PLATFORM} STREQUAL "Web")
    # Tell Emscripten to build an example.html file.
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
endif()

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()