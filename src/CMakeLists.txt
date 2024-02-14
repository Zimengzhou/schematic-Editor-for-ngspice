include_directories(./include)
include_directories(./ui)

include(cmake/sdl2-config.cmake)
include(./cmake/sdl2-img-ttf-mixer.cmake)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} all_src)
aux_source_directory(include include_src)
aux_source_directory(ui UI_SRC)


#add_executable(testgeometry
#             ./test/testgesture.c)
#target_link_libraries(testgeometry PRIVATE 
#  SDL2::SDL2-static
#  SDL2::SDL2main 
#  SDL2::SDL2test)

add_executable(ZMPLC 
             ${all_src}
             ${include_src}
             ${UI_SRC})


target_link_libraries(ZMPLC PRIVATE 
                    SDL2::SDL2main 
                    SDL2::SDL2 
                    sdl2img
                    sdl2ttf
                    sdl2mixer)
