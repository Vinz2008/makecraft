#include "lua_api.h"
#include "../../makecraft.h"
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

FILE* fp;

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
    createChunk(x_chunk, y_chunk, z_chunk);
    return 1;
}

int lua_createBlock(lua_State *L){
    lua_Number x_block = luaL_checknumber(L, 1);
    lua_Number y_block = luaL_checknumber(L, 2);
    lua_Number z_block = luaL_checknumber(L, 3);
    createBlock(x_block, y_block, z_block);
    return 1;
}

void runLuaFile(char* filename){
    fp = fopen("log.txt", "w");
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
