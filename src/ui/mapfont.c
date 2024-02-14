#include "mapfont.h"
#include <stdio.h>

static char *appBasePath;
static char *PATH(const char *relativePath);
static int writeJsonFile(const char *filename, const char *strings);
/**
 * @brief 加载字体文件 font.json 以及对应的font.png 来创建MapFont结构体，使用FreeMapFont进行销毁
 * 
 * @param renderer
 * @param fontfile 字体图片文件名, 对应配置文件名应该与之相同
 * @return MapFont* 失败返回NULL
 */
MapFont* newMapFont(SDL_Renderer *renderer, const char* fontPNGfile)
{
    MapFont *mapFont = (MapFont *)calloc(1, sizeof(MapFont));
    mapFont->renderer = renderer;
    cJSON *fontJson = NULL;
    appBasePath = SDL_GetBasePath();
    char* fontPNGfilepath = PATH(fontPNGfile);
    mapFont->fontTexture = IMG_LoadTexture(renderer, fontPNGfilepath);
    if (mapFont->fontTexture == NULL) { SDL_Log("MapFont create faild: %s", SDL_GetError());return NULL;}
    // mapFont->fontSurface = IMG_Load(fontPNGfilepath);
    // if (mapFont->fontSurface == NULL) { SDL_Log("MapFont create faild: %s", SDL_GetError());return NULL;}

    char *jsonfilepath = (char *)calloc(strlen(fontPNGfilepath) + 5, sizeof(char));
    strcat(jsonfilepath, fontPNGfilepath);
    strcpy(jsonfilepath + strlen(jsonfilepath) - 3, "json");
    // int tempcount = SDL_GetTicks();
    char *jsonFile = (char *)SDL_LoadFile(jsonfilepath, NULL);
    // SDL_Log("\033[1;31m1.加载文件耗时: %d ms\033[0m", SDL_GetTicks() - tempcount);
    // tempcount = SDL_GetTicks();
    /*初始化json*/
    fontJson = cJSON_Parse(jsonFile);
    // SDL_Log("\033[1;32m2.解析json耗时: %d ms\033[0m", SDL_GetTicks() - tempcount);
    cJSON* size = cJSON_GetObjectItem(fontJson,"size");
    cJSON *number = cJSON_GetObjectItemCaseSensitive(fontJson, "number");
    if (!cJSON_IsNumber(size))
    {
        SDL_Log("\033[1;31;46mjson的size不是数字，请检查文件\033[0m");
        return NULL;
    }
    if (!cJSON_IsNumber(number))
    {
        SDL_Log("\033[1;31;46mjson的number不是数字，请检查文件\033[0m");
        return NULL;
    }
    mapFont->size = size->valueint;
    mapFont->number = number->valueint;

    cJSON *unicode_indexArray = cJSON_GetObjectItemCaseSensitive(fontJson,"Index");
    cJSON *element;
    int kkk = 0;
    cJSON_ArrayForEach(element,unicode_indexArray)
    {
        mapFont->UnicodeIndex[kkk] = element->valueint;
        kkk++;
    }

    /*字符坐标信息*/
    // tempcount = SDL_GetTicks();
    cJSON* textInfoArray = cJSON_GetObjectItemCaseSensitive(fontJson, "glyphRect");
    cJSON *glyphRegion = NULL;
    mapFont->glyphRegion = (SDL_Rect *)calloc(number->valueint, sizeof(SDL_Rect));
    uint16_t i = 0;
    cJSON_ArrayForEach(glyphRegion,textInfoArray)
    {

        cJSON *x = cJSON_GetObjectItemCaseSensitive(glyphRegion, "x");
        cJSON *y = cJSON_GetObjectItemCaseSensitive(glyphRegion, "y");
        cJSON *w = cJSON_GetObjectItemCaseSensitive(glyphRegion, "w");
        cJSON *h = cJSON_GetObjectItemCaseSensitive(glyphRegion, "h");
        mapFont->glyphRegion[i].x = x->valueint;
        mapFont->glyphRegion[i].y = y->valueint;
        mapFont->glyphRegion[i].w = w->valueint;
        mapFont->glyphRegion[i].h = h->valueint;
        i++;
    }
    // SDL_Log("\033[1;33m3.提取出大数组耗时: %d ms\033[0m", SDL_GetTicks() - tempcount);
    SDL_Log("MapFont create success: %s,共%d个字符", fontPNGfile,i);
    cJSON_Delete(fontJson);
    return mapFont;
}

SDL_Texture *MapFontUNICODEtexture(Uint16 *string, MapFont *mapFont, int r, int g, int b, int a)
{
    int stringNum = wcslen(string);/*包括 \n 的个数*/
    int widthNum = 0,maxWidthNum = 0;
    int rowNum = 1;
    Uint16 *temp = string;
    while(*string)
    {
        /*换行,统计行数，更新最大宽度*/
        if (*string == 10)
        {
            rowNum++;
            if (widthNum>=maxWidthNum)
                maxWidthNum = widthNum;
            widthNum = 0;
        }    
        else
            widthNum = widthNum + 1;
        string++;
    }
    string = temp;
    if(widthNum>=maxWidthNum) maxWidthNum = widthNum;

    SDL_Texture *stringTexture = SDL_CreateTexture(mapFont->renderer, SDL_PIXELFORMAT_RGBA8888, 
                                         SDL_TEXTUREACCESS_TARGET, maxWidthNum*mapFont->size, rowNum*(mapFont->size+10));
    // SDL_Surface *stringSurface = SDL_CreateRGBSurfaceWithFormat(0,
                                // maxWidthNum * mapFont->size, rowNum * (mapFont->size + 10), 32, SDL_PIXELFORMAT_ARGB8888);
    SDL_SetTextureBlendMode(mapFont->fontTexture, SDL_BLENDMODE_NONE);
    SDL_Texture *saveTarget = SDL_GetRenderTarget(mapFont->renderer);
    SDL_SetRenderTarget(mapFont->renderer, stringTexture);
    SDL_SetTextureColorMod(mapFont->fontTexture, r,g,b);
    SDL_SetTextureAlphaMod(mapFont->fontTexture, a);
    SDL_SetRenderDrawColor(mapFont->renderer, 255, 255, 255, 0);
    SDL_RenderClear(mapFont->renderer);

    SDL_Rect dest = {0, 0, 0, 0};
    while (*temp)
    {
        if (*temp == 10)
        {
            dest.x = 0;
            dest.y = dest.h + dest.y;
        }
        else if (*temp == 32)
        {
            dest.x += dest.w;
        }
        else
        {
            int index = mapFont->UnicodeIndex[*temp];
            dest.w = mapFont->glyphRegion[index].w;
            dest.h = mapFont->glyphRegion[index].h;
            // SDL_BlitSurface(mapFont->fontSurface, &mapFont->glyphRegion[index], stringSurface, &dest);
            SDL_RenderCopy(mapFont->renderer, mapFont->fontTexture, &(mapFont->glyphRegion[index]), &dest);
            dest.x = dest.w + dest.x;
        }
        temp++;
    }
    SDL_SetRenderTarget(mapFont->renderer, saveTarget);
    SDL_SetTextureBlendMode(stringTexture, SDL_BLENDMODE_BLEND);

    return stringTexture;
}

/**
 * @brief free a MapFont.
 * 
 * @param mapFont 
 */
void FreeMapFont(MapFont *mapFont)
{
    SDL_DestroyTexture(mapFont->fontTexture);
    SDL_FreeSurface(mapFont->fontSurface);
    free(mapFont->glyphRegion);
    free(mapFont);
}


static int writeJsonFile(const char * filename, const char *strings)
{
    SDL_RWops *jsonFile = SDL_RWFromFile(filename, "w+");
    jsonFile->write(jsonFile, strings, strlen(strings), 1);
    SDL_RWclose(jsonFile);
    return 0;
}
/**
 * @brief 
 * 
 * @param relativePath 
 * @return char* 
 */
static char *PATH(const char* relativePath)
{
    char *path = (char *)calloc(strlen(appBasePath)+strlen(relativePath)+2,sizeof(char));
    strcat(path,appBasePath);
    strcat(path, relativePath);
    return path;
}

//   int MapFontGenerator(const char* pngFile,const char* fontJsonFilename, const char* ttfFont, int size)
//   {
//       /*Unicode对应的Rect在fontInfoRect中的索引*/
//       int UnicodeIndex[65538];
//
//       /*文字在图片中的位置和大小信息*/
//       SDL_Rect fontInfoRect[21050];
//       int InfoIndex = 0;
//
//       appBasePath = SDL_GetBasePath();
//
//       SDL_Color color_white = {255, 255, 255, 255};
//       TTF_Font *ttffont = TTF_OpenFont(PATH(ttfFont), size);
//       TTF_Font *ttffont_en = TTF_OpenFont(PATH("font/HackRegularNerdFontCompleteMono.ttf"), size);
//
//       SDL_Surface *fontSurfaceEn = SDL_CreateRGBSurfaceWithFormat(0, 4000, 4900, 32, SDL_PIXELFORMAT_ARGB8888);
//       SDL_Rect destRect = {0, 0, 0, 0};
//       SDL_FillRect(fontSurfaceEn, NULL, SDL_MapRGBA(fontSurfaceEn->format, 255, 255, 255, 0));
//
//       /*添加字母和英文符号*/
//       for (uint16_t i = 33; i < 127; i++)
//       {
//           SDL_Surface *glyphASCII_surface = TTF_RenderGlyph_Blended(ttffont_en, i, color_white);
//           destRect.w = glyphASCII_surface->w;
//           destRect.h = glyphASCII_surface->h;
//           fontInfoRect[InfoIndex].x = destRect.x;
//           fontInfoRect[InfoIndex].y = destRect.y;
//           fontInfoRect[InfoIndex].w = destRect.w;
//           fontInfoRect[InfoIndex].h = destRect.h;
//           UnicodeIndex[i] = InfoIndex;
//           InfoIndex++;
//
//           SDL_BlitSurface(glyphASCII_surface, NULL, fontSurfaceEn, &destRect);
//           destRect.x += destRect.w;
//           if (destRect.x>=3980)
//           {
//               destRect.x = 0;
//               destRect.y += destRect.h+5;
//           }
//           SDL_FreeSurface(glyphASCII_surface);
//       }
//       /*添加一些中文标点符号 ，。？！：；“”‘’（）【】《》·、- —— */
//       wchar_t cn_char[] = L"，。？！：；“”‘’（）【】《》·、—";
//       for (uint16_t i = 0; i < wcslen(cn_char); i++)
//       {
//           uint16_t unicode_num = cn_char[i];
//
//           SDL_Surface *glyphASCII_surface = TTF_RenderGlyph_Blended(ttffont, unicode_num, color_white);
//           destRect.w = glyphASCII_surface->w;
//           destRect.h = glyphASCII_surface->h;
//           fontInfoRect[InfoIndex].x = destRect.x;
//           fontInfoRect[InfoIndex].y = destRect.y;
//           fontInfoRect[InfoIndex].w = destRect.w;
//           fontInfoRect[InfoIndex].h = destRect.h;
//           UnicodeIndex[unicode_num] = InfoIndex;
//           InfoIndex++;
//
//           SDL_BlitSurface(glyphASCII_surface, NULL, fontSurfaceEn, &destRect);
//           destRect.x += destRect.w;
//           if (destRect.x>=3980)
//           {
//               destRect.x = 0;
//               destRect.y += destRect.h;
//           }
//           SDL_FreeSurface(glyphASCII_surface);
//       }
//       /*换行*/
//       destRect.x = 0;
//       destRect.y += destRect.h + 10;
//       /*添加汉字*/
//       for (uint16_t i = 0x4E00; i < 0x9fa6; i++)
//       {
//           SDL_Surface *glyph_surface = TTF_RenderGlyph_Blended(ttffont, i, color_white);
//
//           destRect.w = glyph_surface->w;
//           destRect.h = glyph_surface->h;
//           fontInfoRect[InfoIndex].x = destRect.x;
//           fontInfoRect[InfoIndex].y = destRect.y;
//           fontInfoRect[InfoIndex].w = destRect.w;
//           fontInfoRect[InfoIndex].h = destRect.h;
//           UnicodeIndex[i] = InfoIndex;
//           InfoIndex++;
//           
//           SDL_BlitSurface(glyph_surface, NULL, fontSurfaceEn, &destRect);
//           
//           destRect.x += destRect.w;
//           if (destRect.x>=3980)
//           {
//               destRect.x = 0;
//               destRect.y += destRect.h;
//           }
//           SDL_FreeSurface(glyph_surface);
//       }
//
//       cJSON *fontJson = cJSON_CreateObject();
//       cJSON_AddNumberToObject(fontJson,"size",size);          /*size*/
//       cJSON_AddNumberToObject(fontJson, "number", 21050);     /*number*/
//       cJSON *UnicodeIndexCJ = cJSON_CreateIntArray(UnicodeIndex, 65536);
//       cJSON_AddItemToObject(fontJson, "Index", UnicodeIndexCJ);
//       cJSON *glyphRegionRects = cJSON_AddArrayToObject(fontJson, "glyphRect");
//       for (int i = 0; i < 21050; i++)
//       {
//           cJSON *rect = cJSON_CreateObject();
//           cJSON_AddNumberToObject(rect, "x", fontInfoRect[i].x);
//           cJSON_AddNumberToObject(rect, "y", fontInfoRect[i].y);
//           cJSON_AddNumberToObject(rect, "w", fontInfoRect[i].w);
//           cJSON_AddNumberToObject(rect, "h", fontInfoRect[i].h);
//           cJSON_AddItemToArray(glyphRegionRects, rect);
//       }
//
//       writeJsonFile(PATH(fontJsonFilename), cJSON_Print(fontJson));
//       cJSON_Delete(fontJson);
//       // SDL_Log("%s", PATH(fontJsonFilename));
//       IMG_SavePNG(fontSurfaceEn, PATH(pngFile));
//       SDL_FreeSurface(fontSurfaceEn);
//       TTF_CloseFont(ttffont);
//       TTF_CloseFont(ttffont_en);
//       return 0;
//   }
