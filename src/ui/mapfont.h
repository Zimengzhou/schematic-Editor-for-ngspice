#ifndef SZM_MAPFONT_H
#define SZM_MAPFONT_H
#include <SDL.h>
#include <SDL_image.h>
#include <cJSON.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct MapFont MapFont;
struct MapFont
{
    /*字体大小*/
    int size;
    /*字体文件纹理*/
    SDL_Texture *fontTexture;
    /*字体文件图片Surface*/
    SDL_Surface *fontSurface;
    /*字形定位Rectangle,数量依赖于文字数量*/
    SDL_Rect *glyphRegion;
    /*glyphRegion索引*/
    uint16_t UnicodeIndex[65536];
    /*含符号数量*/
    int number;
    SDL_Renderer *renderer;
};

MapFont* newMapFont(SDL_Renderer *renderer, const char * fontPNGfile);

SDL_Texture *MapFontUNICODEtexture(Uint16 *string, MapFont *mapFont, int r, int g, int b, int a);

void FreeMapFont(MapFont *mapFont);

//int MapFontGenerator(const char *pngFile, const char *fontJsonFile, const char *ttfFont, int size);

#ifdef __cplusplus
}
#endif
#endif

