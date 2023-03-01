#include "lua_api.h"
#include "../makecraft.h"
#include "../map/block.h"
#include "../map/chunk.h"
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

extern BlockArray* blockArray;

int lua_printTest(lua_State *L){
    FILE* fp2 = fopen("log2.txt", "w");
    fprintf(fp2, "test\n");
    printf("test\n");
    return 1;
}

int lua_createChunk(lua_State *L){
    float x_chunk = luaL_checknumber(L, 1);
    float y_chunk = luaL_checknumber(L, 2);
    float z_chunk = luaL_checknumber(L, 3);
    createChunk(blockArray, x_chunk, y_chunk, z_chunk);
    return 1;
}

int lua_createBlock(lua_State *L){
    float x_block = (float)luaL_checknumber(L, 1);
    float y_block = (float)luaL_checknumber(L, 2);
    float z_block = (float)luaL_checknumber(L, 3);
    printf("x_block float : %f\n", x_block);
    printf("y_block float : %f\n", y_block);
    printf("z_block float : %f\n", z_block);
    createBlock(blockArray, (float)x_block, (float)y_block, (float)z_block, dirt_texture);
    return 1;
}

void runLuaFile(char* filename){
    FILE* fp = fopen("log.txt", "w");
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    fprintf(fp, "started lua\n");
    lua_pushcfunction(L, lua_printTest);
    lua_setglobal(L, "makecraftPrintTest");
    lua_pushcfunction(L, lua_createBlock);
    lua_setglobal(L, "createBlock");
    lua_pushcfunction(L, lua_createChunk);
    lua_setglobal(L, "createChunk");
    if (luaL_dofile(L, filename) == LUA_OK) {
        lua_pop(L, lua_gettop(L));
    }
    lua_close(L);
    fclose(fp);
}
