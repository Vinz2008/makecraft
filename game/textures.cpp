#include "raylib.h"
#include "../makecraft.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <string>
#include <optional>

std::map<int, Texture2D*> cachedTextures;


struct _texturePath {
    int key;
    const char* path;
};

static struct _texturePath TexturePaths[] = {
    {water_texture, "textures/water.png"},
    {dirt_texture, "textures/dirt.png"},
    {stone_texture, "textures/stone.png"},
    {empty_texture, "textures/empty.png"}
};

void addToCachedTextures(Texture2D texture, int pos){
    cachedTextures[pos] = (Texture2D*)malloc(sizeof(Texture2D)); 
    memcpy(cachedTextures[pos], &texture, sizeof(Texture2D));
}


extern "C" Texture2D getBlockTexture(enum material m){
    int pos = (int)m;
    if (pos < 0){
        pos = -pos;
    }
    if (cachedTextures[pos] != NULL){
        return *cachedTextures[pos];
    }
    const char* path = "";
    for (int i = 0; i < sizeof(TexturePaths)/sizeof(struct _texturePath); i++){
        if (TexturePaths[i].key == m){
            path = TexturePaths[i].path;
        }
    }
    if (strcmp(path, "") == 0 && m != empty_texture){
        return getBlockTexture(empty_texture);
    }
    Image TempTextureMap = LoadImage(path);
    Texture2D generatedTexture = LoadTextureFromImage(TempTextureMap);
    addToCachedTextures(generatedTexture, pos);
    //UnloadImage(TempTextureMap);
    return generatedTexture;
}