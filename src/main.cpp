#include "makecraft.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef PLATFORM_WEB
#include <limits.h>
#endif
#include "raylib.h"
#include "raymath.h"
#ifndef PLATFORM_WEB
#define CAMERA_IMPLEMENTATION
#endif
#define CAMERA_STANDALONE_WITH_RAYLIB
#include "rcamera.h"
#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"
#include "rlgl.h"
#include "utils/startswith.h"
#include "lua_api/lua_api.h"
#ifdef WIN32
#include <io.h>
#define F_OK 0
#define access _access
#else
#include <unistd.h>
#endif
#include <vector>
#include "map/chunk.h"
#include "map/block.h"
#include "map/generation.h"
#include "map/water.h"
#include "map/serialize.h"
#include "noise2.h"
#include "utils/list.h"
#include "textures.h"

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_RPI, PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif


#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
    static void UpdateDrawFrame(void);
    #define CAMERA_ROTATION_SPEED                           0.03f
#endif

#define PLAYER_MOVEMENT_SENSITIVITY 1.5
#define CAMERA_MOUSE_MOVE_SENSITIVITY 0.001f
#define CAMERA_MOVE_SPEED  0.5f

#define PLAYER_HEIGHT 1.0f

typedef struct {
    Camera camera;
} Player;

Player player; 



std::vector<Block> blockArray;
// contain Chunk values that are loaded
// TODO : will need not open the whole save file and lazy load it by having a way to calculate the place in the file where it will be
std::vector<Chunk> chunkArray;

//----------------------------------------------------------------------------------
// Local Variables Definition (local to this module)
//----------------------------------------------------------------------------------
Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };
Vector3 PlayerPosition = {0};
Vector3 PlayerPositionFloor = { 0 };
BoundingBox PlayerHitBox = {0};
BoundingBox GroundHitBox = {0};
static Vector2 lastMousePos;
//static Vector3 movement = {0, -0.2f, 0};
//static Vector3 rotation;
Shader shader;

Vector3* cubeArrayPos;

int mode = creative_mode;

bool exitWindow = false;
bool showHUD = false;
int pos_hud = 0;

//----------------------------------------------------------------------------------
// Local Functions Declaration
//----------------------------------------------------------------------------------
//static void UpdateDrawFrame(void);          // Update and draw one frame

bool isNumberAround(float nb, float nb2, float precision){
    return ((nb <= nb2 && nb >= nb2 - precision) || (nb >= nb2 && nb <= nb2 + precision));
}

bool isPlayerInCollisionWithBlock(Player player, Block block){
    if (isNumberAround(player.camera.position.x, block.x, 1.0f) && isNumberAround(player.camera.position.y, block.y, 1.0f) && isNumberAround(player.camera.position.z, block.z, 1.0f)){
        printf("collision\n");
        return true;
    }
    return false;
}


bool isPlayerInCollisionWithArrayBlock(Player player, std::vector<Block>& blockArray){
    for (int i = 0; i < blockArray.size(); i++) {
        if (isPlayerInCollisionWithBlock(player, blockArray.at(i))){
            return true;
        }
    }
    return false;
}

bool isPlayerOnTopOfBlock(Player player, Block block){
    if (isNumberAround(player.camera.position.x, block.x, 1.0f) && isNumberAround(player.camera.position.y - PLAYER_HEIGHT, block.y, 4.0f) && isNumberAround(player.camera.position.z, block.z, 1.0f)){
        printf("on top\n");
        return true;
    }
    return false;
}


bool isPlayerOnTopOfBlockArray(Player player, std::vector<Block>& blockArray){
    for (int i = 0; i < blockArray.size(); i++) {
        Block temp_block = blockArray.at(i);
        if (isPlayerOnTopOfBlock(player, temp_block) && temp_block.material != water_texture){
            return true;
        }
    }
    return false;
}

bool isPlayerOnTopOfBlockOfWater(Player player, std::vector<Block>& blockArray){
    for (int i = 0; i < blockArray.size(); i++) {
        Block temp_block = blockArray.at(i);
        if (isPlayerOnTopOfBlock(player, temp_block) && temp_block.material == water_texture){
            return true;
        }
    }
    return false;
}

//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------
int main(int argc, char* argv[]){
    int seed = NOISE_SEED;
    float frequency = NOISE_FREQUENCY;
    std::vector<float> farray = generate_noise(NB_BLOCK_NOISE, seed, frequency, 0, 0);
    write_noise_to_file(farray, NB_BLOCK_NOISE, "noise.txt");
    player.camera.fovy = 0;
    if (access("blockArray.tpl", F_OK) == 0){
        blockArray = tpl_deserialize_block_array("blockArray.tpl");
    } else {
        blockArray = {};
        for (int x = 0; x < NB_BLOCK_NOISE; x++){
            for (int z = 0; z < NB_BLOCK_NOISE; z++){
                float y = round(get_noise_data(farray, x, z, NB_BLOCK_NOISE));
                enum material texture = get_block_type(y);
                blockArray.push_back((Block){x*2.0f, y*2.0f, z*2.0f, texture});
            }
        }
        tpl_serialize_block_array(blockArray, "blockArray.tpl");
    }
    printf("blockArraysize : %ld\n", blockArray.size());
    //chunkArray = get_new_perlin_chunk_list(); // TODO : fix the segmentation fault
    char* filename_lua = "script.lua";
    printf("filename_lua : %s\n", filename_lua);
    if (argc > 0){
        for (int i = 0; i < argc; i++) {
            printf("%d : %s\n",i,  argv[i]);
            if (startswith("-s", argv[i]) == 1 || startswith("--script", argv[i]) == 1){
                printf("found in %s\n", argv[i]);
                printf("filename_lua = %s\n", argv[i+1]);
                filename_lua = argv[i+1];
            }
        }
    }

    printf("filename_lua : %s\n", filename_lua);
    if (access(filename_lua, F_OK) == 0){
        printf("run file lua : %s\n", filename_lua);
        runLuaFile(filename_lua);
    }
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1200;
    const int screenHeight = 675;

    InitWindow(screenWidth, screenHeight, "raylib");

    player.camera.position = (Vector3){ 10.0f, PLAYER_HEIGHT + 10.0f, 8.0f };
    player.camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    player.camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    player.camera.fovy = 60.0f;
    player.camera.projection = CAMERA_PERSPECTIVE;
    PlayerPosition = (Vector3){player.camera.position.x , player.camera.position.y, player.camera.position.z};
    PlayerPositionFloor = (Vector3){player.camera.position.x , player.camera.position.y - PLAYER_HEIGHT, player.camera.position.z};
    PlayerHitBox = (BoundingBox){PlayerPositionFloor, PlayerPosition};
    GroundHitBox = (BoundingBox){(Vector3){ 0.0f, 0.0f, 0.0f }, (Vector3){ 0.0f + 32.0f, 0.0f + 32.0f, 0.0f } };

    Rectangle textBox = {10, 10,  screenWidth/1.1, screenHeight/1.1};
    Ray ray = { 0 };
    RayCollision collision = { 0 };
    float** elevation;
    bool mouseOnText = false;
    int framesCounter = 0;
    int letterCount = 0;
    SetConfigFlags(FLAG_VSYNC_HINT);
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    //rlEnableBackfaceCulling();
    //rlDisableBackfaceCulling();
    elevation = (float**)malloc(sizeof(float*) * 1000);
    float falling_speed = 0;
    //--------------------------------------------------------------------------------------


    DisableCursor();
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    lastMousePos = GetMousePosition();
    SetExitKey(0);

    // Main game loop
    while (!exitWindow){   // Detect window close button or ESC key  
        //UpdateCamera(&camera);
        if (IsKeyPressed(KEY_V) || WindowShouldClose()) exitWindow = true;
        if (IsKeyPressed(KEY_ESCAPE)){
            showHUD = (showHUD) ? false : true;
            printf("showHUD : %d\n", showHUD);
        }
        
        if (!showHUD){

        Vector2 mousePositionDelta = GetMouseDelta();
        bool moveInWorldPlane = true;
        bool lockView = true;
        bool rotateAroundTarget = false;
        bool rotateUp = false;
        if (IsKeyDown(KEY_DOWN)) CameraPitch(&player.camera, -CAMERA_ROTATION_SPEED, lockView, rotateAroundTarget, rotateUp);
        if (IsKeyDown(KEY_UP)) CameraPitch(&player.camera, CAMERA_ROTATION_SPEED, lockView, rotateAroundTarget, rotateUp);
        if (IsKeyDown(KEY_RIGHT)) CameraYaw(&player.camera, -CAMERA_ROTATION_SPEED, rotateAroundTarget);
        if (IsKeyDown(KEY_LEFT)) CameraYaw(&player.camera, CAMERA_ROTATION_SPEED, rotateAroundTarget);
        if (IsKeyDown(KEY_Q)) CameraRoll(&player.camera, -CAMERA_ROTATION_SPEED);
        if (IsKeyDown(KEY_E)) CameraRoll(&player.camera, CAMERA_ROTATION_SPEED);

        CameraYaw(&player.camera, -mousePositionDelta.x*CAMERA_MOUSE_MOVE_SENSITIVITY, rotateAroundTarget);
        CameraPitch(&player.camera, -mousePositionDelta.y*CAMERA_MOUSE_MOVE_SENSITIVITY, lockView, rotateAroundTarget, rotateUp);
        
        // Camera movement
        // TODO : make movement slower when in water
        if (IsKeyDown(KEY_W)) CameraMoveForward(&player.camera, CAMERA_MOVE_SPEED, moveInWorldPlane);
        if (IsKeyDown(KEY_A)) CameraMoveRight(&player.camera, -CAMERA_MOVE_SPEED, moveInWorldPlane);
        if (IsKeyDown(KEY_S)) CameraMoveForward(&player.camera, -CAMERA_MOVE_SPEED, moveInWorldPlane);
        if (IsKeyDown(KEY_D)) CameraMoveRight(&player.camera, CAMERA_MOVE_SPEED, moveInWorldPlane);
        //if (IsKeyDown(KEY_SPACE)) CameraMoveUp(camera, CAMERA_MOVE_SPEED);
        //if (IsKeyDown(KEY_LEFT_CONTROL)) CameraMoveUp(camera, -CAMERA_MOVE_SPEED);              
        if (mode == creative_mode){
        if (IsKeyDown(KEY_LEFT_SHIFT))
            //movement.y -= 0.30f;
            CameraMoveUp(&player.camera, -0.30f);
        if (IsKeyDown(KEY_SPACE))
            //movement.y += 0.30f;
            CameraMoveUp(&player.camera, 0.30f);
        } else {
        CameraMoveUp(&player.camera, -falling_speed);
        if (isPlayerOnTopOfBlockOfWater(player, blockArray)){
            falling_speed /= 5.0f;
        }
        if (!isPlayerOnTopOfBlockArray(player, blockArray)){
            //CameraMoveUp(&player.camera, -0.17f); 
            falling_speed += 0.1f;
        } else {
            falling_speed = 0;
            if (IsKeyDown(KEY_SPACE)){
            //CameraMoveUp(&player.camera, 0.30f);
            // falling_speed -= 2.0f; // BIG JUMP (TODO : maybe add this as an effect or a parameter)
               falling_speed -= 0.9f;
            }
        }
        }

        if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
            ray = GetMouseRay(GetMousePosition(), player.camera);
            Mesh MeshCube = GenMeshCube(CUBE_SIZE, CUBE_SIZE, CUBE_SIZE);
            for (int i = 0; i < blockArray.size(); i++){
                //printf("i : %d, used : %d, size : %d\n", i, blockArray->used, blockArray->size);
                Block tempBlock = blockArray.at(i);
                RayCollision collision = GetRayCollisionMesh(ray, MeshCube, MatrixTranslate(tempBlock.x, tempBlock.y, tempBlock.z));
                if (collision.hit){
                    printf("ray collide with cube\n");
                    //blockArray = removeFromBlockArray(i, blockArray); // TODO : readd this when the mouse pointing will work
                    blockArray.at(i).material = water_texture;
                }
            }
        }
        
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            if (!collision.hit){
                ray = GetMouseRay(GetMousePosition(), player.camera);
                collision = GetRayCollisionBox(ray,
                            (BoundingBox){(Vector3){ cubePosition.x - CUBE_SIZE/2, cubePosition.y - CUBE_SIZE/2, cubePosition.z - CUBE_SIZE/2 },
                                          (Vector3){ cubePosition.x + CUBE_SIZE/2, cubePosition.y + CUBE_SIZE/2, cubePosition.z + CUBE_SIZE/2 }});
            }
            else collision.hit = false;
        }    

        } else {
            if (IsKeyPressed(KEY_UP)){
                pos_hud = (pos_hud-1 < 0) ? NUMBER_BUTTON_MENU-1 : pos_hud-1;
            } else if (IsKeyPressed(KEY_DOWN)){
                pos_hud = (pos_hud+1 > NUMBER_BUTTON_MENU-1) ? 0 : pos_hud+1;
            } else if (IsKeyPressed(KEY_ENTER)){
                if (pos_hud == 0){
                    showHUD = false;
                } else if (pos_hud == 1){
                    mode = (mode == creative_mode) ? survival_mode : creative_mode;
                } else if (pos_hud == 2){
                     exitWindow = true;
                }
            }
        }
        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(player.camera);
            DrawPlane((Vector3){ 0.0f, 0.0f, 0.0f }, (Vector2){ 32.0f, 32.0f }, LIGHTGRAY);

            for (float x = 0; x < NB_BLOCK_NOISE; x++){
                for (float y = 0; y < NB_BLOCK_NOISE; y++){
                    float znoise = round(get_noise_data(farray, x, y, NB_BLOCK_NOISE));
                    if (znoise < 17){
                    drawWater(blockArray, x*2.0f, y*2.0f, znoise+0.1, 10);
                    }
                }
            }

            for (int i = 0; i < blockArray.size(); i++){
                Block blocktemp = blockArray.at(i);
                drawBlock(blocktemp.x, blocktemp.y, blocktemp.z, blocktemp.material);
            }

        EndMode3D();
        DrawLine(screenWidth/2, screenHeight/2, screenWidth/2 , screenHeight/2 - screenHeight/20, GRAY);
        DrawLine(screenWidth/2 - screenWidth/40, screenHeight/2 - screenHeight/40, screenWidth/2 + screenWidth/40, screenHeight/2 - screenHeight/40, GRAY);
        DrawFPS(10, 10);
        if (showHUD == true){
            DrawRectangleRec(textBox, LIGHTGRAY);
            if (mouseOnText) DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, RED);
            else DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, DARKGRAY);
            DrawText(filename_lua, (int)textBox.x + 5, (int)textBox.y + 8, 40, MAROON);
            if (mouseOnText){
                if (letterCount < 9){
                    if (((framesCounter/20)%2) == 0) DrawText("_", (int)textBox.x + 8 + MeasureText(filename_lua, 40), (int)textBox.y + 12, 40, MAROON);
                }
                else DrawText("Press BACKSPACE to delete chars...", 230, 300, 20, GRAY);
            }
            DrawRectangle(screenWidth/4, screenHeight/4, screenWidth/8, screenHeight/8, (pos_hud == 0) ? BLACK : WHITE );
            DrawText("Continue", screenWidth/4, screenHeight/4, 25, GRAY);
            DrawRectangle(screenWidth/4, screenHeight/2.5, screenWidth/8, screenHeight/8, (pos_hud == 1) ? BLACK : WHITE);
            DrawText("Change Mode", screenWidth/4, screenHeight/2.5, 25, GRAY);
            DrawRectangle(screenWidth/4, screenHeight/1.75, screenWidth/8, screenHeight/8, (pos_hud == 2) ? BLACK : WHITE);
            DrawText("Exit", screenWidth/4, screenHeight/1.75, 25, GRAY);
        }
        

    EndDrawing();
    /*if (!CheckCollisionBoxes(PlayerHitBox, GroundHitBox)){
        for (i = 0; i < 20; i++) {
        camera.position = (Vector3){ camera.position.x, camera.position.y - 0.7f , camera.position.z};
        }
    };*/
    PlayerPosition = (Vector3){player.camera.position.x , player.camera.position.y, player.camera.position.z};
    PlayerPositionFloor = (Vector3){player.camera.position.x , player.camera.position.y - 10.0f, player.camera.position.z};
    PlayerHitBox = (BoundingBox){PlayerPositionFloor, PlayerPosition};
    GroundHitBox = (BoundingBox){(Vector3){ 0.0f, 0.0f, 0.0f }, (Vector3){ 0.0f + 32.0f, 0.0f + 32.0f, 0.0f } };

    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    tpl_serialize_block_array(blockArray, "blockArray.tpl");
    UnloadCachedTextures();
    CloseWindow();                  // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    free(elevation);
    return 0;
}
