/*******************************************************************************************
*
*   To compile example, just press F5.
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com

*
********************************************************************************************/

#include <stdio.h>
#include <limits.h>
#include "raylib.h"
#include "raymath.h"
#include "lib/noise/noise1234.h"
#define PLAYER_HEIGHT 5.0f

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif


#define PLAYER_MOVEMENT_SENSITIVITY 3
#define CAMERA_MOUSE_MOVE_SENSITIVITY 0.001f

#define CHUNK_LENGTH 9

typedef struct {
   Vector3 cubeCenter;
   Vector3 cubeTopRight;
   Vector3 cubeTopLeft
}CHUNK;



//----------------------------------------------------------------------------------
// Local Variables Definition (local to this module)
//----------------------------------------------------------------------------------
Camera camera = { 0 };
Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };
Vector3 PlayerPosition = {0};
Vector3 PlayerPositionFloor = { 0 };
BoundingBox PlayerHitBox = {0};
BoundingBox GroundHitBox = {0};
static Vector2 lastMousePos;
static Vector3 movement = {0, -0.2f, 0};
static Vector3 rotation;
Texture2D DirtTexture;


Vector3* cubeArrayPos;

//----------------------------------------------------------------------------------
// Local Functions Declaration
//----------------------------------------------------------------------------------
//static void UpdateDrawFrame(void);          // Update and draw one frame

float noise(int x, int y) {
    int n;

    n = x + y * 57;
    n = (n << 13) ^ n;
    return (1.0 - ( (n * ((n * n * 15731) + 789221) +  1376312589) & 0x7fffffff) / 1073741824.0);
}






__int16_t PerlinNoise2D( float x, float y, float amp, __int32_t octaves, __int32_t px, __int32_t py )
{
  float noise = 0.f;
  for( int octave = 1; octave < octaves; octave *= 2 )
  {
    // Add in fractions of faster varying noise at lower amplitudes 
    // for higher octaves. Assuming x is normalized, WHEN octave==px
    // you get full period. Higher frequencies will go out and also meet period.
    noise += pnoise2( x*px*octave, y*py*octave, px, py ) / octave;
  }
  return USHRT_MAX/2.f + amp*noise;
}

void createChunk(float x, float y, float z){
    int i;
    int i2;
    for (i2 = 0; i2 < CHUNK_LENGTH * 2; i2 = i2 + 2) {
        for (i = 0; i < CHUNK_LENGTH * 2; i = i + 2) {
            //__uint16_t noise_gen = PerlinNoise2D(1, 1, 1, 1, 1,1 );
            //printf("noise : %d\n", noise_gen);
            int temp = 1;
            //for (int temp = 1; temp < noise_gen/10000; temp++){
            DrawCube((Vector3){x + (float)i , y * temp, z + (float)i2}, 2.0f, 2.0f, 2.0f, RED);
            DrawCubeWires((Vector3){x + (float)i, y * temp, z + (float)i2}, 2.0f, 2.0f, 2.0f, BLACK);
            DrawCubeTexture(DirtTexture, (Vector3){x + (float)i, y * temp, z + (float)i2},2.0f,2.0f,2.0f ,WHITE);
            DrawBoundingBox(PlayerHitBox, BLACK);
            DrawLine3D(PlayerPosition,PlayerPositionFloor, BLACK);
            //}        
        }
    }
}



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
    
    camera.position = (Vector3){ 10.0f, PLAYER_HEIGHT + 10.0f, 8.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    PlayerPosition = (Vector3){camera.position.x , camera.position.y, camera.position.z};
    PlayerPositionFloor = (Vector3){camera.position.x , camera.position.y - PLAYER_HEIGHT, camera.position.z};
    PlayerHitBox = (BoundingBox){PlayerPositionFloor, PlayerPosition};
    GroundHitBox = (BoundingBox){(Vector3){ 0.0f, 0.0f, 0.0f }, (Vector3){ 0.0f + 32.0f, 0.0f + 32.0f, 0.0f } };
    Image DirtTextureMap = LoadImage("textures/dirt.png");
    DirtTexture = LoadTextureFromImage(DirtTextureMap);
    
    SetCameraMode(camera, CAMERA_FIRST_PERSON);
    SetConfigFlags(FLAG_VSYNC_HINT);
    SetConfigFlags(FLAG_MSAA_4X_HINT);

    //--------------------------------------------------------------------------------------

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    lastMousePos = GetMousePosition();
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {  
        //UpdateCamera(&camera);
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

        camera.position.x += movement.x / PLAYER_MOVEMENT_SENSITIVITY;

        camera.position.y += movement.y;

        camera.position.z += movement.z / PLAYER_MOVEMENT_SENSITIVITY;

        // -------------------- Retarget stuff --------------------

        Matrix translation = MatrixTranslate(0, 0, (10));
        Matrix rotation2 = MatrixRotateXYZ((Vector3){ PI*2 - rotation.y, PI*2 - rotation.x, 0 });
        Matrix transform = MatrixMultiply(translation, rotation2);

        camera.target.x = camera.position.x - transform.m12;
        camera.target.y = camera.position.y - transform.m13;
        camera.target.z = camera.position.z - transform.m14;
        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(camera);
            DrawPlane((Vector3){ 0.0f, 0.0f, 0.0f }, (Vector2){ 32.0f, 32.0f }, LIGHTGRAY);
            Vector3 position;
                    position.x = 0;
                    position.y = 0;
                    position.z = 0;


            for (int i = 0; i < 10 * CHUNK_LENGTH; i +=  CHUNK_LENGTH){
                printf("i : %d\n", i);
                createChunk(cubePosition.x + i, cubePosition.y, cubePosition.z);
            }
            /*for (i2 = 0; i2 < 36; i2 = i2 + 2) {
            for (i = 0; i < 36; i = i + 2) {
            __uint16_t noise_gen = PerlinNoise2D(1, 1, 1, 1, 1,1 );
            printf("noise : %d\n", noise_gen);
            for (int temp = 1; temp < noise_gen/10000; temp++){
            DrawCube((Vector3){cubePosition.x + (float)i , cubePosition.y * temp, cubePosition.z + (float)i2}, 2.0f, 2.0f, 2.0f, RED);
            DrawCubeWires((Vector3){cubePosition.x + (float)i, cubePosition.y * temp, cubePosition.z + (float)i2}, 2.0f, 2.0f, 2.0f, BLACK);
            DrawCubeTexture(DirtTexture, (Vector3){cubePosition.x + (float)i, cubePosition.y * temp, cubePosition.z + (float)i2},2.0f,2.0f,2.0f ,WHITE);
            DrawBoundingBox(PlayerHitBox, BLACK);
            DrawLine3D(PlayerPosition,PlayerPositionFloor, BLACK);
            }
            
            }
            }*/
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