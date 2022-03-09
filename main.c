/*******************************************************************************************
*
*   To compile example, just press F5.
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com

*
********************************************************************************************/

#include "raylib.h"
#define PLAYER_HEIGHT 5.0f

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

//----------------------------------------------------------------------------------
// Local Variables Definition (local to this module)
//----------------------------------------------------------------------------------
Camera camera = { 0 };
Vector3 cubePosition = { 0.0f, 1.5f, 0.0f };
Vector3 PlayerPosition = {0};
Vector3 PlayerPositionFloor = { 0 };
BoundingBox PlayerHitBox = {0};
BoundingBox GroundHitBox = {0};

//----------------------------------------------------------------------------------
// Local Functions Declaration
//----------------------------------------------------------------------------------
//static void UpdateDrawFrame(void);          // Update and draw one frame

//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------
int main() 
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1200;
    const int screenHeight = 675;
    int i;
    int i2;
    int i3;

    InitWindow(screenWidth, screenHeight, "raylib");
    
    camera.position = (Vector3){ 10.0f, PLAYER_HEIGHT + 2.0f, 8.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    PlayerPosition = (Vector3){camera.position.x , camera.position.y, camera.position.z};
    PlayerPositionFloor = (Vector3){camera.position.x , camera.position.y - PLAYER_HEIGHT, camera.position.z};
    PlayerHitBox = (BoundingBox){PlayerPositionFloor, PlayerPosition};
    GroundHitBox = (BoundingBox){(Vector3){ 0.0f, 0.0f, 0.0f }, (Vector3){ 0.0f + 32.0f, 0.0f + 32.0f, 0.0f } };
    Image DirtTextureMap = LoadImage("textures/dirt.png");
    Texture2D DirtTexture = LoadTextureFromImage(DirtTextureMap);
    
    SetCameraMode(camera, CAMERA_FIRST_PERSON);

    //--------------------------------------------------------------------------------------

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdateCamera(&camera);
        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(camera);
            DrawPlane((Vector3){ 0.0f, 0.0f, 0.0f }, (Vector2){ 32.0f, 32.0f }, LIGHTGRAY);
            for (i2 = 0; i2 < 36; i2 = i2 + 2) {
            for (i = 0; i < 36; i = i + 2) {
            DrawCube((Vector3){cubePosition.x + (float)i, cubePosition.y, cubePosition.z + (float)i2}, 2.0f, 2.0f, 2.0f, RED);
            DrawCubeWires((Vector3){cubePosition.x + (float)i, cubePosition.y, cubePosition.z + (float)i2}, 2.0f, 2.0f, 2.0f, BLACK);
            DrawCubeTexture(DirtTexture, (Vector3){cubePosition.x + (float)i, cubePosition.y, cubePosition.z + (float)i2},2.0f,2.0f,2.0f ,WHITE);
            DrawBoundingBox(PlayerHitBox, BLACK);
            DrawLine3D(PlayerPosition,PlayerPositionFloor, BLACK);
            }
            }
            /*for (i = 3; i < 27; i = i + 3) {
            DrawCube((Vector3){cubePosition.x + (float)i2, cubePosition.y, cubePosition.z  + (float)i}, 2.0f, 2.0f, 2.0f, RED);
            DrawCubeWires((Vector3){cubePosition.x + (float)i2, cubePosition.y, cubePosition.z  + (float)i}, 2.0f, 2.0f, 2.0f, BLACK); 
            }
            BoundingBox cubeBox = {cubePosition, (Vector3){cubePosition.x + 1.0f, cubePosition.y + 1.0f, cubePosition.z + 1.0f}};
            Ray rayPointerPlayer = {PlayerPosition, (Vector3){camera.position.x, 1.0f, camera.position.z}};
            DrawGrid(10, 1.0f);
            DrawRay(rayPointerPlayer, WHITE);
            GetRayCollisionBox(rayPointerPlayer,cubeBox);
            */

        EndMode3D();
        //camera.position.y -= 1;

        DrawFPS(10, 10);
        

    EndDrawing();
    /*if (!CheckCollisionBoxes(PlayerHitBox, GroundHitBox)){
        for (i = 0; i < 20; i++) {
        camera.position = (Vector3){ camera.position.x, camera.position.y - 0.7f , camera.position.z};
        }
    };*/
    PlayerPosition = (Vector3){camera.position.x , camera.position.y, camera.position.z};
    PlayerPositionFloor = (Vector3){camera.position.x , camera.position.y - 10.0f, camera.position.z};
    PlayerHitBox = (BoundingBox){PlayerPositionFloor, PlayerPosition};
    GroundHitBox = (BoundingBox){(Vector3){ 0.0f, 0.0f, 0.0f }, (Vector3){ 0.0f + 32.0f, 0.0f + 32.0f, 0.0f } };
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadImage(DirtTextureMap);
    UnloadTexture(DirtTexture);
    CloseWindow();                  // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

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