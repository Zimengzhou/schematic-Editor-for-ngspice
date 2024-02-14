cmake_minimum_required(VERSION 3.23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(MINGW_SDL
        VERSION 1.5)
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -save-temps=obj")
# set(CMAKE_C_FLAGS_Release "${CMAKE_C_FLAGS_Release} -Og")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Os")
set(CMAKE_BUILD_TYPE "Debug")
add_subdirectory(src bin)

    
    
    
    
    
    
    
    
    
    

