set(SDL2_IMG_INCLUDE_PATH D:/Projects/C/3dlibs/SDL2_image-2.6.3/x86_64-w64-mingw32/include/SDL2)
set(SDL2_IMG_LIB_PATH D:/Projects/C/3dlibs/SDL2_image-2.6.3/x86_64-w64-mingw32/bin)

set(SDL2_TTF_INCLUDE_PATH D:/Projects/C/3dlibs/SDL2_ttf-2.20.2/x86_64-w64-mingw32/include/SDL2)
set(SDL2_TTF_LIB_PATH D:/Projects/C/3dlibs/SDL2_ttf-2.20.2/x86_64-w64-mingw32/bin)

set(SDL2_MIXER_INCLUDE_PATH D:/Projects/C/3dlibs/SDL2_mixer-2.6.3/x86_64-w64-mingw32/include/SDL2)
set(SDL2_MIXER_LIB_PATH D:/Projects/C/3dlibs/SDL2_mixer-2.6.3/x86_64-w64-mingw32/bin)


#sdl2img
add_library(sdl2img INTERFACE)
target_include_directories(sdl2img INTERFACE ${SDL2_IMG_INCLUDE_PATH})
target_link_directories(sdl2img INTERFACE ${SDL2_IMG_LIB_PATH})
target_link_libraries(sdl2img INTERFACE  SDL2_image)
#sdl2ttf
add_library(sdl2ttf INTERFACE)
target_include_directories(sdl2ttf INTERFACE ${SDL2_TTF_INCLUDE_PATH})
target_link_directories(sdl2ttf INTERFACE ${SDL2_TTF_LIB_PATH})
target_link_libraries(sdl2ttf INTERFACE  SDL2_ttf)
#sdl2mixer
add_library(sdl2mixer INTERFACE)
target_include_directories(sdl2mixer INTERFACE ${SDL2_MIXER_INCLUDE_PATH})
target_link_directories(sdl2mixer INTERFACE ${SDL2_MIXER_LIB_PATH})
target_link_libraries(sdl2mixer INTERFACE SDL2_mixer)

