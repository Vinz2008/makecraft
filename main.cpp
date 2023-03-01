/*******************************************************************************************
*
*   To compile example, just press F5.
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com

*
********************************************************************************************/

#include "makecraft.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "raylib.h"
#include "raymath.h"
#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"
#include "lib/startswith.h"
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
#include "map/serialize.h"
#include "noise/noise2.h"

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_RPI, PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif


#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif


#define PLAYER_MOVEMENT_SENSITIVITY 1.5
#define CAMERA_MOUSE_MOVE_SENSITIVITY 0.001f

#define PLAYER_HEIGHT 5.0f

FILE* fp;
struct blockColumn {
    Block* blockArray;
    size_t used;
    size_t size;
};

typedef struct {
    Vector3 cubeCenter;
    Vector3 cubeTopRight;
    Vector3 cubeTopLeft;
    struct blockColumn* BlockColumn;
}CHUNK;

typedef struct {
    CHUNK* chunk;
    size_t size;
    size_t used;
} chunkList;



typedef struct {
    Camera camera;
}Player;

Player player; 



BlockArray* blockArray;

//----------------------------------------------------------------------------------
// Local Variables Definition (local to this module)
//----------------------------------------------------------------------------------
Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };
Vector3 PlayerPosition = {0};
Vector3 PlayerPositionFloor = { 0 };
BoundingBox PlayerHitBox = {0};
BoundingBox GroundHitBox = {0};
static Vector2 lastMousePos;
static Vector3 movement = {0, -0.2f, 0};
static Vector3 rotation;
Texture2D DirtTexture;
Texture2D StoneTexture;
Shader shader;

Vector3* cubeArrayPos;

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


bool isPlayerInCollisionWithArrayBlock(Player player, BlockArray blockArray){
    for (int i = 0; i < blockArray.used; i++) {
        if (isPlayerInCollisionWithBlock(player, blockArray.blockArray[i])){
            return true;
        }
    }
    return false;
}


//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------
int main(int argc, char* argv[]){
    std::vector<float> farray = generate_noise(NB_BLOCK_NOISE, 164647, 0.05);
    write_noise_to_file(farray, NB_BLOCK_NOISE, "noise.txt");
    float fl = get_noise_data(farray, 5, 4, NB_BLOCK_NOISE);

    fp = fopen("log.txt", "w");
    player.camera.fovy= 0;
    blockArray = (BlockArray*)malloc(sizeof(BlockArray));
    initBlockArray(blockArray, NB_BLOCK_NOISE);
    for (int x = 0; x < NB_BLOCK_NOISE; x++){
        for (int y = 0; y < NB_BLOCK_NOISE; y++){
        int z = get_noise_data(farray, x, y, NB_BLOCK_NOISE);
        int texture = dirt_texture;
        if (z < 10 || z > 30){
            texture = stone_texture;
        }
        addToBlockArray(blockArray, (Block){x, y, z, texture});
    }
    }
    printf("blockArraysize : %d\n", blockArray->used);
    serialize_block_array(blockArray, "blockArray.bin");
    char* filename_lua = "script.lua";
    printf("filename_lua : %s\n", filename_lua);
    if (argc > 0){
        for (int i = 0; i < argc; i++) {
            printf("%d : %s\n",i,  argv[i]);
            if (startswith("-s", argv[i]) == 1 || startswith("--script", argv[i]) == 1){
                printf("found in %s\n", argv[i]);
                printf("filename_lua = %s\n", argv[i+1]);
                filename_lua = argv[i+1];
                //i++; 
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
    int i;
    int i2;
    int i3;

    InitWindow(screenWidth, screenHeight, "raylib");
    shader = LoadShader(TextFormat("shaders/glsl%i/lighting.vs", GLSL_VERSION), TextFormat("shaders/glsl%i/lighting.fs", GLSL_VERSION));
    shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
    int ambientLoc = GetShaderLocation(shader, "ambient");
    float locIndex[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    SetShaderValue(shader, ambientLoc, locIndex, SHADER_UNIFORM_VEC4);
    player.camera.position = (Vector3){ 10.0f, PLAYER_HEIGHT + 10.0f, 8.0f };
    player.camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    player.camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    player.camera.fovy = 60.0f;
    player.camera.projection = CAMERA_PERSPECTIVE;
    PlayerPosition = (Vector3){player.camera.position.x , player.camera.position.y, player.camera.position.z};
    PlayerPositionFloor = (Vector3){player.camera.position.x , player.camera.position.y - PLAYER_HEIGHT, player.camera.position.z};
    PlayerHitBox = (BoundingBox){PlayerPositionFloor, PlayerPosition};
    GroundHitBox = (BoundingBox){(Vector3){ 0.0f, 0.0f, 0.0f }, (Vector3){ 0.0f + 32.0f, 0.0f + 32.0f, 0.0f } };
    Image DirtTextureMap = LoadImage("textures/dirt.png");
    Image StoneTextureMap = LoadImage("textures/stone.png");
    DirtTexture = LoadTextureFromImage(DirtTextureMap);
    StoneTexture = LoadTextureFromImage(StoneTextureMap);
    Rectangle textBox = {10, 10,  screenWidth/10, screenHeight/5};
    Ray ray = { 0 };
    RayCollision collision = { 0 };
    float** elevation;
    bool mouseOnText = false;
    int framesCounter = 0;
    int letterCount = 0;
    //char filename_lua[10] = "\0";
    SetCameraMode(player.camera, CAMERA_FIRST_PERSON);
    SetConfigFlags(FLAG_VSYNC_HINT);
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    elevation = (float**)malloc(sizeof(float*) * 1000);

    //--------------------------------------------------------------------------------------

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    lastMousePos = GetMousePosition();
    bool exitWindow = false;
    bool showHUD = false;
    SetExitKey(0);

    // Main game loop
    while (!exitWindow){   // Detect window close button or ESC key  
        //UpdateCamera(&camera);
        if (IsKeyPressed(KEY_V) || WindowShouldClose()) exitWindow = true;
        if (IsKeyPressed(KEY_ESCAPE)){
            showHUD = (showHUD == true) ? false : true;
            printf("showHUD : %d\n", showHUD);
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        }
        /*if (CheckCollisionPointRec(GetMousePosition(), textBox) && showHUD == true) mouseOnText = true;
        else mouseOnText = false;
        if (mouseOnText){
            SetMouseCursor(MOUSE_CURSOR_IBEAM);
            int key = GetCharPressed();
            while (key > 0)
            {
                // NOTE: Only allow keys in range [32..125]
                if ((key >= 32) && (key <= 125) && (letterCount < 9))
                {
                    filename_lua[letterCount] = (char)key;
                    filename_lua[letterCount+1] = '\0'; // Add null terminator at the end of the string.
                    letterCount++;
                }

                key = GetCharPressed();  // Check next character in the queue
            }

            if (IsKeyPressed(KEY_BACKSPACE))
            {
                letterCount--;
                if (letterCount < 0) letterCount = 0;
                filename_lua[letterCount] = '\0';
            }
        }
        else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

        if (mouseOnText) framesCounter++;
        else framesCounter = 0;*/
        Vector2 mouseMovement = Vector2Subtract(GetMousePosition(), lastMousePos);
        rotation.x += (mouseMovement.x*-CAMERA_MOUSE_MOVE_SENSITIVITY);
        rotation.y += (mouseMovement.y*-CAMERA_MOUSE_MOVE_SENSITIVITY);
        lastMousePos = GetMousePosition();
        float direction[] = { IsKeyDown(KEY_W),
                              IsKeyDown(KEY_S),
                              IsKeyDown(KEY_D),
                              IsKeyDown(KEY_A)};
        movement = (Vector3){0, 0, 0};
        movement.x = (sinf(rotation.x)*direction[1] -
                      sinf(rotation.x)*direction[0] -
                      cosf(rotation.x)*direction[3] +
                      cosf(rotation.x)*direction[2])/PLAYER_MOVEMENT_SENSITIVITY;

        movement.z = (cosf(rotation.x)*direction[1] -
                      cosf(rotation.x)*direction[0] +
                      sinf(rotation.x)*direction[3] -
                      sinf(rotation.x)*direction[2])/PLAYER_MOVEMENT_SENSITIVITY;

        if (IsKeyDown(KEY_LEFT_SHIFT))
            movement.y -= 0.12f;
        if (IsKeyDown(KEY_SPACE))
            movement.y += 0.12f;
        
        movement.y -= 0.06f;

        player.camera.position.x += movement.x / PLAYER_MOVEMENT_SENSITIVITY;

        player.camera.position.y += movement.y;

        player.camera.position.z += movement.z / PLAYER_MOVEMENT_SENSITIVITY;

        // -------------------- Retarget stuff --------------------

        Matrix translation = MatrixTranslate(0, 0, (10));
        Matrix rotation2 = MatrixRotateXYZ((Vector3){ PI*2 - rotation.y, PI*2 - rotation.x, 0 });
        Matrix transform = MatrixMultiply(translation, rotation2);

        player.camera.target.x = player.camera.position.x - transform.m12;
        player.camera.target.y = player.camera.position.y - transform.m13;
        player.camera.target.z = player.camera.position.z - transform.m14;

        isPlayerInCollisionWithArrayBlock(player, *blockArray);


        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            if (!collision.hit){
                ray = GetMouseRay(GetMousePosition(), player.camera);
                collision = GetRayCollisionBox(ray,
                            (BoundingBox){(Vector3){ cubePosition.x - CUBE_SIZE/2, cubePosition.y - CUBE_SIZE/2, cubePosition.z - CUBE_SIZE/2 },
                                          (Vector3){ cubePosition.x + CUBE_SIZE/2, cubePosition.y + CUBE_SIZE/2, cubePosition.z + CUBE_SIZE/2 }});
            }
            else collision.hit = false;
        }    
        
        BeginDrawing();
        ClearBackground(RAYWHITE);
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
        }

        BeginMode3D(player.camera);
            DrawPlane((Vector3){ 0.0f, 0.0f, 0.0f }, (Vector2){ 32.0f, 32.0f }, LIGHTGRAY);
            Vector3 position;
                    position.x = 0;
                    position.y = 0;
                    position.z = 0;

            for (float x = 0; x < NB_BLOCK_NOISE; x++){
                for (float y = 0; y < NB_BLOCK_NOISE; y++){
                    float znoise = round(get_noise_data(farray, x, y, NB_BLOCK_NOISE));
                    float mult = 2.0f;
                    int texture = dirt_texture;
                    if (znoise < 5 || znoise > 10){
                        texture = stone_texture;
                    }
                    //printf("create block x %f, y %f, z %f\n", x*mult, znoise*mult, y*mult);
                    Block* tempBlock = createBlock(blockArray, x*mult, znoise*mult, y*mult, texture);
                    //addToBlockArray(blockArray, *tempBlock);
                }
            }


            int height = 100;
            int width = 100;
            int frequency1 = 1;
            int frequency2 = 2;
            int frequency3 = 4;
            /*for (i = 3; i < 27; i = i + 3) {
            DrawCube((Vector3){cubePosition.x + (float)i2, cubePosition.y, cubePosition.z  + (float)i}, 2.0f, 2.0f, 2.0f, RED);
            DrawCubeWires((Vector3){cubePosition.x + (float)i2, cubePosition.y, cubePosition.z  + (float)i}, 2.0f, 2.0f, 2.0f, BLACK); 
            }
            BoundingBox cubeBox = {cubePosition, (Vector3){cubePosition.x + 1.0f, cubePosition.y + 1.0f, cubePosition.z + 1.0f}};
            Ray rayPointerPlayer = {PlayerPosition, (Vector3){camera.position.x, 1.0f, camera.position.z}};
            DrawGrid(10, 1.0f);    player.camera.position.y -= 1
            GetRayCollisionBox(rayPointerPlayer,cubeBox);
            */

        EndMode3D();
        //camera.position.y -= 1;
        DrawLine(screenWidth/2, screenHeight/2, screenWidth/2 , screenHeight/2 - screenHeight/20, GRAY);
        DrawLine(screenWidth/2 - screenWidth/40, screenHeight/2 - screenHeight/40, screenWidth/2 + screenWidth/40, screenHeight/2 - screenHeight/40, GRAY);
        DrawFPS(10, 10);
        

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
    float test[] = {1.2};
    serialize_array(test, sizeof(test[0]), 1, "test.bin");
    serialize_block_array(blockArray, "blockArray.bin");
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadImage(DirtTextureMap);
    UnloadTexture(DirtTexture);
    CloseWindow();                  // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    free(elevation);
    return 0;
}

// Update and draw game frame
/*static void UpdateDrawFrame(void)
{
    // Update
    //----------------------------------------------------------------------------------
    //UpdateCamera(&camera);
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

            DrawCube(cubePosition, 2.0f, 2.0f, 2.0f, RED);
            DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, MAROON);
            DrawGrid(10, 1.0f);

        EndMode3D();

        DrawText("This is a raylib example", 10, 40, 20, DARKGRAY);

        DrawFPS(10, 10);

    EndDrawing();
    //----------------------------------------------------------------------------------
}*/